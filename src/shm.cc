#include <sstream>

#if defined(_WIN32)
#include <Windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#endif

#include "bsw/shm.hh"
#include "bsw/strings/wchar.hh"

#include "bsw/warn/push.hh"
#include "bsw/warn/sign_conversion"

namespace bsw::memory_mapped_file {
	unsigned int mmf_granularity () {
#if defined(_WIN32)
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		return SystemInfo.dwAllocationGranularity;
#else
		return static_cast<int>(sysconf (_SC_PAGE_SIZE));
#endif
	}

	base_mmf::base_mmf ()
		:
		data_ (0),
		offset_ (0),
		mapped_size_ (0),
		file_size_ (0),
		granularity_ (mmf_granularity ()),
#if defined(_WIN32)
		file_handle_(INVALID_HANDLE_VALUE),
		file_mapping_handle_(INVALID_HANDLE_VALUE)
#else
		file_handle_ (-1)
#endif
	{
	}

	base_mmf::~base_mmf () {
		close ();
	}

	void base_mmf::close () {
		unmap ();
#if defined(_WIN32)
		::CloseHandle(file_handle_);
		file_handle_ = (void*)-1;
#else
		::close (file_handle_);
		file_handle_ = -1;
#endif
		file_size_ = 0;
	}

	void base_mmf::unmap () {
		if (data_) {
			char* real_data = data_
							  - (offset_ - offset_ / granularity_ * granularity_);
#if defined(_WIN32)
			::UnmapViewOfFile(real_data);

#else
			size_t real_mapped_size = mapped_size_ + (data_ - real_data);
			::munmap (const_cast<char*>(real_data), real_mapped_size);
#endif
		}
#if defined(_WIN32)
		if (file_mapping_handle_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(file_mapping_handle_);
			file_mapping_handle_ = INVALID_HANDLE_VALUE;
		}
#endif
		data_ = nullptr;
		offset_ = 0;
		mapped_size_ = 0;
	}

	uint64_t base_mmf::query_file_size_ () const {
#if defined(_WIN32)
		LARGE_INTEGER ul;
		DWORD high_size;
		ul.LowPart = GetFileSize(file_handle_, &high_size);
		ul.HighPart = high_size;
		return ul.QuadPart;
#else
		struct stat sbuf{};
		if (::fstat (file_handle_, &sbuf) == -1) { return 0; }
		return sbuf.st_size;
#endif
	}

	read_only_mmf::read_only_mmf (char const* pathname, bool map_all) {
		open (pathname, map_all);
	}

	read_only_mmf::read_only_mmf (wchar_t const* pathname, bool map_all) {
		open (pathname, map_all);
	}

	void read_only_mmf::open (char const* pathname, bool map_all) {
		if (!pathname) { return; }
		if (is_open ()) { close (); }
#if defined(_WIN32)
		file_handle_ = ::CreateFile(pathname, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file_handle_ == INVALID_HANDLE_VALUE) return;
#else
		file_handle_ = ::open (pathname, O_RDONLY);
		if (file_handle_ == -1) { return; }
#endif
		file_size_ = query_file_size_ ();
		if (map_all) { map (); }
	}

	void read_only_mmf::open (wchar_t const* pathname, bool map_all) {
		if (!pathname) { return; }
		if (is_open ()) { close (); }
#if defined(_WIN32)
		file_handle_ = ::CreateFileW(pathname, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file_handle_ == INVALID_HANDLE_VALUE) return;
#else
		auto pth = bsw::wstring_to_utf8 (pathname);
		file_handle_ = ::open (pth.c_str (), O_RDONLY);
		if (file_handle_ == -1) { return; }
#endif
		file_size_ = query_file_size_ ();
		if (map_all) { map (); }
	}

	void read_only_mmf::map (
		size_t offset, size_t requested_size) {
		unmap ();
		if (offset >= file_size_) { return; }
		auto mapping_size = requested_size && offset + requested_size
											  < file_size_ ? requested_size : file_size_ - offset;
		if (mapping_size <= 0) { return; }

#if defined(_WIN32)
		LARGE_INTEGER real_offset;
		real_offset.QuadPart = offset / granularity_ * granularity_;
		file_mapping_handle_ = ::CreateFileMapping(
			file_handle_, 0, PAGE_READONLY, (offset + mapping_size) >> 32,
			(offset + mapping_size) & 0xFFFFFFFF, 0);
		if (file_mapping_handle_ == INVALID_HANDLE_VALUE) return;
		char* real_data = static_cast<char*>(::MapViewOfFile(
			file_mapping_handle_, FILE_MAP_READ, real_offset.HighPart,
			real_offset.LowPart, offset - real_offset.QuadPart + mapping_size));
		if (!real_data) return;
#else
		uint64_t real_offset;
		real_offset = offset / granularity_ * granularity_;
		char* real_data = static_cast<char*>(::mmap (
			nullptr, offset - real_offset + mapping_size, PROT_READ, MAP_SHARED,
			file_handle_, real_offset));
		if (real_data == MAP_FAILED) { return; }
#endif
#if defined(_WIN32)
		data_ = real_data + (offset - real_offset.QuadPart);
#else
		data_ = real_data + (offset - real_offset);
#endif
		mapped_size_ = mapping_size;
		offset_ = offset;
	}

	writable_mmf::writable_mmf (char const* pathname,
								memory_mapped_file::mmf_exists_mode exists_mode,
								memory_mapped_file::mmf_doesnt_exist_mode doesnt_exist_mode) {
		open (pathname, exists_mode, doesnt_exist_mode);
	}

	void writable_mmf::open (char const* pathname,
							 memory_mapped_file::mmf_exists_mode exists_mode,
							 memory_mapped_file::mmf_doesnt_exist_mode doesnt_exist_mode) {
		if (!pathname) { return; }
		if (is_open ()) { close (); }
#if defined(_WIN32)
		int win_open_mode;

		switch (exists_mode)
		{
		case if_exists_just_open:
		case if_exists_map_all:
			win_open_mode = doesnt_exist_mode == if_doesnt_exist_create ?
				OPEN_ALWAYS : OPEN_EXISTING;
			break;
		case if_exists_truncate:
			win_open_mode = doesnt_exist_mode == if_doesnt_exist_create ?
				CREATE_ALWAYS : TRUNCATE_EXISTING;
			break;
		default:
			if (doesnt_exist_mode == if_doesnt_exist_create)
			{
				win_open_mode = CREATE_NEW;
			}
			else return;
		}

		file_handle_ = ::CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			win_open_mode, FILE_ATTRIBUTE_NORMAL, 0);
		if (file_handle_ == INVALID_HANDLE_VALUE) return;
#else
		int posix_open_mode = O_RDWR;

		switch (exists_mode) {
			case if_exists_just_open:
			case if_exists_map_all:
				posix_open_mode |= doesnt_exist_mode == if_doesnt_exist_create ?
								   O_CREAT : 0;
				break;
			case if_exists_truncate:
				posix_open_mode |= doesnt_exist_mode == if_doesnt_exist_create ?
								   O_TRUNC | O_CREAT : O_TRUNC;
				break;
			default:
				if (doesnt_exist_mode == if_doesnt_exist_create) {
					posix_open_mode |= O_EXCL | O_CREAT;
				} else { return; }
		}

		file_handle_ = ::open (pathname, posix_open_mode,
							   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (file_handle_ == -1) { return; }
#endif
		file_size_ = query_file_size_ ();
		if (exists_mode == if_exists_map_all && file_size_ > 0) { map (); }
	}

	void writable_mmf::map (size_t offset, size_t requested_size) {
		unmap ();
		if (offset > file_size_) { return; }
		size_t mapping_size = requested_size ?
							  requested_size : file_size_ - offset;

#if defined(_WIN32)
		LARGE_INTEGER real_offset;
		real_offset.QuadPart = offset / granularity_ * granularity_;
		LARGE_INTEGER max_size;
		max_size.QuadPart = offset + mapping_size;
		file_mapping_handle_ = ::CreateFileMappingA(
			file_handle_, 0, PAGE_READWRITE, max_size.HighPart,
			max_size.LowPart, 0);
		if (file_mapping_handle_ == INVALID_HANDLE_VALUE) return;
		char* real_data = static_cast<char*>(::MapViewOfFile(
			file_mapping_handle_, FILE_MAP_WRITE, real_offset.HighPart,
			real_offset.LowPart, offset - real_offset.QuadPart + mapping_size));
		if (!real_data) return;
#else
		uint64_t real_offset;
		real_offset = offset / granularity_ * granularity_;
		if (offset + mapping_size > file_size_) {
			if (-1 == ftruncate (file_handle_, offset + mapping_size)) { return; }
			file_size_ = offset + mapping_size;
		}
		char* real_data = static_cast<char*>(::mmap (
			nullptr, offset - real_offset + mapping_size, PROT_READ | PROT_WRITE, MAP_SHARED,
			file_handle_, real_offset));
		if (real_data == MAP_FAILED) { return; }
#endif
		if (offset + mapping_size > file_size_) {
			file_size_ = offset + mapping_size;
		}
#if defined(_WIN32)
		data_ = real_data + (offset - real_offset.QuadPart);
#else
		data_ = real_data + (offset - real_offset);
#endif
		mapped_size_ = mapping_size;
		offset_ = offset;
	}

	bool writable_mmf::flush () {
		if (data_) {
			char* real_data = data_
							  - (offset_ - offset_ / granularity_ * granularity_);
			size_t real_mapped_size = mapped_size_ + (data_ - real_data);
#if defined(_WIN32)

			return ::FlushViewOfFile(real_data, real_mapped_size) != 0
				&& FlushFileBuffers(file_handle_) != 0;
			if (::FlushViewOfFile(real_data, real_mapped_size) == 0) return false;

#else
			if (::msync (real_data, real_mapped_size, MS_SYNC) != 0) { return false; }
#endif
		}
#if defined(_WIN32)
		return FlushFileBuffers(file_handle_) != 0;
#else
		return true;
#endif
	}
}
// ns bsw
#include "bsw/warn/pop.hh"