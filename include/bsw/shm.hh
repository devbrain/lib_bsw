#ifndef BSW_SHM_HPP
#define BSW_SHM_HPP

#include <bsw/export.h>
#include <cstddef>
#include <cstdint>

namespace bsw::memory_mapped_file {
	unsigned int mmf_granularity ();

#if defined(_WIN32)
	typedef void* HANDLE;
#else
	typedef int HANDLE;
#endif

	class base_mmf {
	 public:
		explicit base_mmf ();
		~base_mmf ();

		[[nodiscard]] uint64_t offset () const { return offset_; }

		[[nodiscard]] size_t mapped_size () const { return mapped_size_; }

		[[nodiscard]] uint64_t file_size () const { return file_size_; }

		void unmap ();
		void close ();

		[[nodiscard]] bool is_open () const {
			return file_handle_ !=
				   #if defined(_WIN32)
				   (void*)
				   #endif
				   -1;
		}

		[[nodiscard]] HANDLE file_handle () const {
			return file_handle_;
		}

	 protected:
		[[nodiscard]] uint64_t query_file_size_ () const;
		char* data_;
		uint64_t offset_;
		size_t mapped_size_;
		uint64_t file_size_;
		int granularity_;
		HANDLE file_handle_;
#if defined(_WIN32)
		HANDLE file_mapping_handle_;
#endif
	};

	class read_only_mmf : public base_mmf {
	 public:
		explicit read_only_mmf (char const* pathname, bool map_all = true);
		explicit read_only_mmf (wchar_t const* pathname, bool map_all = true);
		void open (char const* pathname, bool map_all = true);
		void open (wchar_t const* pathname, bool map_all = true);

		[[nodiscard]] char const* data () const { return data_; }

		void map (size_t offset = 0, size_t size = 0);
	};

	enum mmf_exists_mode {
		if_exists_fail,
		if_exists_just_open,
		if_exists_map_all,
		if_exists_truncate,
	};

	enum mmf_doesnt_exist_mode {
		if_doesnt_exist_fail,
		if_doesnt_exist_create,
	};

	class writable_mmf : public base_mmf {
	 public:
		explicit writable_mmf (char const* pathname = nullptr,
							   mmf_exists_mode exists_mode = if_exists_fail,
							   mmf_doesnt_exist_mode doesnt_exist_mode = if_doesnt_exist_create);
		void open (char const* pathname,
				   mmf_exists_mode exists_mode = if_exists_fail,
				   mmf_doesnt_exist_mode doesnt_exist_mode = if_doesnt_exist_create);

		char* data () { return data_; }

		void map (size_t offset = 0, size_t size = 0);
		bool flush ();
	};
}

// ns bsw


#endif
