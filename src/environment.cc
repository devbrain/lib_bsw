//
// Created by igor on 3/2/24.
//

#if defined(_MSC_VER)
	#if !defined(_CRT_SECURE_NO_WARNINGS)
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif


#include <cstdio>
#include <cstdlib>
#include <bsw/environment.hh>
#include <bsw/predef.h>
#include <bsw/exception.hh>

#if defined(PREDEF_OS_WINDOWS)
class  EnvironmentImpl
{
public:
	typedef uint8_t NodeId[6]; /// Ethernet address.

	static std::string getImpl(const std::string& name);
	static bool hasImpl(const std::string& name);
	static void setImpl(const std::string& name, const std::string& value);
	static std::string osNameImpl();
	static std::string osDisplayNameImpl();
	static std::string osVersionImpl();
	static std::string osArchitectureImpl();
	static std::string nodeNameImpl();
	static void nodeIdImpl(NodeId& id);
	static unsigned processorCountImpl();
};

#include <bsw/strings/wchar.hh>
#include <vector>
#include <sstream>
#include <cstring>
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996)
#include <Windows.h>
#include <winsock2.h>
#include <wincrypt.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>



std::string EnvironmentImpl::getImpl(const std::string& name)
{
	std::wstring uname = bsw::utf8_to_wstring(name);
	
	DWORD len = GetEnvironmentVariableW(uname.c_str(), 0, 0);
	if (len == 0) {
		RAISE_EX("Can not find env. variable ", name);
	}
		
	std::vector<wchar_t> buffer(len + 1);
	GetEnvironmentVariableW(uname.c_str(), buffer.data(), len);
	buffer[len] = 0;
	std::string result = bsw::wstring_to_utf8(buffer.data());
	
	return result;
}


bool EnvironmentImpl::hasImpl(const std::string& name)
{
	std::wstring uname = bsw::utf8_to_wstring(name);
	DWORD len = GetEnvironmentVariableW(uname.c_str(), 0, 0);
	return len > 0;
}


void EnvironmentImpl::setImpl(const std::string& name, const std::string& value)
{
	std::wstring uname = bsw::utf8_to_wstring(name);
	std::wstring uvalue = bsw::utf8_to_wstring(value);
	
	if (SetEnvironmentVariableW(uname.c_str(), uvalue.c_str()) == 0) {
		RAISE_EX("cannot set environment variable: ", name);
	}
}


std::string EnvironmentImpl::osNameImpl()
{
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionEx(&vi) == 0) {
		RAISE_EX("Cannot get OS version information");
	}
	switch (vi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		return "Windows 3.x";
	case VER_PLATFORM_WIN32_WINDOWS:
		return vi.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
	case VER_PLATFORM_WIN32_NT:
		return "Windows NT";
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::osDisplayNameImpl()
{
	OSVERSIONINFOEX vi;	// OSVERSIONINFOEX is supported starting at Windows 2000
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionEx((OSVERSIONINFO*)&vi) == 0) {
		RAISE_EX("Cannot get OS version information");
	}
	switch (vi.dwMajorVersion)
	{
	case 10:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 10" : "Windows Server 2016";
		}
	case 6:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows Vista" : "Windows Server 2008";
		case 1:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 7" : "Windows Server 2008 R2";
		case 2:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 8" : "Windows Server 2012";
		case 3:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 8.1" : "Windows Server 2012 R2";
		default:
			return "Unknown";
		}
	case 5:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return "Windows 2000";
		case 1:
			return "Windows XP";
		case 2:
			return "Windows Server 2003/Windows Server 2003 R2";
		default:
			return "Unknown";
		}
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::osVersionImpl()
{
	OSVERSIONINFOW vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionExW(&vi) == 0) {
		RAISE_EX ("Cannot get OS version information");
	}
	std::ostringstream str;
	str << vi.dwMajorVersion << "." << vi.dwMinorVersion << " (Build " << (vi.dwBuildNumber & 0xFFFF);
	std::string version = bsw::wstring_to_utf8(vi.szCSDVersion);
	if (!version.empty()) str << ": " << version;
	str << ")";
	return str.str();
}


std::string EnvironmentImpl::osArchitectureImpl()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return "IA32";
	case PROCESSOR_ARCHITECTURE_MIPS:
		return "MIPS";
	case PROCESSOR_ARCHITECTURE_ALPHA:
		return "ALPHA";
	case PROCESSOR_ARCHITECTURE_PPC:
		return "PPC";
	case PROCESSOR_ARCHITECTURE_IA64:
		return "IA64";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64
	case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
		return "IA64/32";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64
	case PROCESSOR_ARCHITECTURE_AMD64:
		return "AMD64";
#endif
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::nodeNameImpl()
{
	wchar_t name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerNameW(name, &size) == 0) {
		RAISE_EX ("Cannot get computer name");
	}
	std::string result = bsw::wstring_to_utf8(name);
	return result;
}


void EnvironmentImpl::nodeIdImpl(NodeId& id)
{
	std::memset(&id, 0, sizeof(id));

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = 0;
	ULONG len    = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into len
	DWORD rc = GetAdaptersInfo(pAdapterInfo, &len);
	if (rc == ERROR_BUFFER_OVERFLOW)
	{
		delete [] reinterpret_cast<char*>(pAdapterInfo);
		pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
	}
	else if (rc != ERROR_SUCCESS)
	{
		return;
	}
	if (GetAdaptersInfo(pAdapterInfo, &len) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		bool found = false;
		while (pAdapter && !found)
		{
			if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->AddressLength == sizeof(id))
			{
				found = true;
				std::memcpy(&id, pAdapter->Address, pAdapter->AddressLength);
			}
			pAdapter = pAdapter->Next;
		}
	}
	delete [] reinterpret_cast<char*>(pAdapterInfo);
}


unsigned EnvironmentImpl::processorCountImpl()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

#else
#include <map>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <sys/utsname.h>
#include <sys/param.h>

#if defined(PREDEF_PLATFORM_BSD)
#include <sys/sysctl.h>
#elif defined(PREDEF_OS_HPUX)
#include <pthread.h>
#endif

class EnvironmentImpl {
 public:
	typedef uint8_t NodeId[6]; /// Ethernet address.

	static std::string getImpl (const std::string& name);
	static bool hasImpl (const std::string& name);
	static void setImpl (const std::string& name, const std::string& value);
	static std::string osNameImpl ();
	static std::string osDisplayNameImpl ();
	static std::string osVersionImpl ();
	static std::string osArchitectureImpl ();
	static std::string nodeNameImpl ();
	static void nodeIdImpl (NodeId& id);
	static unsigned processorCountImpl ();

 private:
	typedef std::map<std::string, std::string> StringMap;

	static StringMap _map;
	static std::mutex _mutex;
};

EnvironmentImpl::StringMap EnvironmentImpl::_map;
std::mutex EnvironmentImpl::_mutex;

std::string EnvironmentImpl::getImpl (const std::string& name) {
	std::lock_guard<std::mutex> lock (_mutex);

	const char* val = getenv (name.c_str ());
	if (val) {
		return val;
	} else
		RAISE_EX("Environment variable ", name, " not found");
}

bool EnvironmentImpl::hasImpl (const std::string& name) {
	std::lock_guard<std::mutex> lock (_mutex);

	return getenv (name.c_str ()) != nullptr;
}

void EnvironmentImpl::setImpl (const std::string& name, const std::string& value) {
	std::lock_guard<std::mutex> lock (_mutex);

	std::string var = name;
	var.append ("=");
	var.append (value);
	std::swap (_map[name], var);
	if (putenv ((char*)_map[name].c_str ())) {
		RAISE_EX("Cannot set env variable ", name);
	}
}

std::string EnvironmentImpl::osNameImpl () {
	struct utsname uts{};
	uname (&uts);
	return uts.sysname;
}

std::string EnvironmentImpl::osDisplayNameImpl () {
	return osNameImpl ();
}

std::string EnvironmentImpl::osVersionImpl () {
	struct utsname uts{};
	uname (&uts);
	return uts.release;
}

std::string EnvironmentImpl::osArchitectureImpl () {
	struct utsname uts{};
	uname (&uts);
	return uts.machine;
}

std::string EnvironmentImpl::nodeNameImpl () {
	struct utsname uts{};
	uname (&uts);
	return uts.nodename;
}

unsigned EnvironmentImpl::processorCountImpl () {
#if defined(_SC_NPROCESSORS_ONLN)
	int count = sysconf (_SC_NPROCESSORS_ONLN);
	if (count <= 0) { count = 1; }
	return static_cast<int>(count);
#elif defined(PREDEF_PLATFORM_BSD)
	unsigned count;
	std::size_t size = sizeof(count);
	if (sysctlbyname("hw.ncpu", &count, &size, 0, 0))
		return 1;
	else
		return count;
#elif defined(PREDEF_OS_HPUX)
	return pthread_num_processors_np();
#else
	return 1;
#endif
}





//
// nodeIdImpl
//
#if defined(PREDEF_PLATFORM_BSD) || defined(PREDEF_OS_QNX)
//
// BSD variants
//
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if_dl.h>

	void EnvironmentImpl::nodeIdImpl(NodeId& id)
	{
		std::memset(&id, 0, sizeof(id));
		struct ifaddrs* ifaphead;
		int rc = getifaddrs(&ifaphead);
		if (rc) return;

		for (struct ifaddrs* ifap = ifaphead; ifap; ifap = ifap->ifa_next)
		{
			if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_LINK)
			{
				struct sockaddr_dl* sdl = reinterpret_cast<struct sockaddr_dl*>(ifap->ifa_addr);
				caddr_t ap = LLADDR(sdl);
				int alen = sdl->sdl_alen;
				if (ap && alen > 0)
				{
					std::memcpy(&id, ap, sizeof(id));
					break;
				}
			}
		}
		freeifaddrs(ifaphead);
	}




#elif defined(__CYGWIN__) || defined(PREDEF_OS_LINUX)
//
// Linux, Cygwin
//
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#ifndef __CYGWIN__
#include <net/if_arp.h>

#else // workaround for Cygwin, which does not have if_arp.h
#define ARPHRD_ETHER 1 /* Ethernet 10Mbps */
#endif
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

void EnvironmentImpl::nodeIdImpl (NodeId& id) {
	std::memset (&id, 0, sizeof (id));

	// ideally, the following code should be rewritten
	// to use netlink

	// first try to obtain the MAC address of eth0 using /sys/class/net
	int fd = open ("/sys/class/net/eth0/address", O_RDONLY);
	if (fd >= 0) {
		char buffer[18];
		int n = read (fd, buffer, 17);
		close (fd);
		if (n == 17) {
			buffer[n] = 0;
			if (std::sscanf (buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &id[0], &id[1], &id[2], &id[3], &id[4], &id[5])
				== 6) {
				return;
			}
		}
	}

	// if that did not work, search active interfaces
	int sock = socket (PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) { return; }

	// the following code is loosely based
	// on W. Richard Stevens, UNIX Network Programming, pp 434ff.
	int lastlen = 0;
	int len = 100 * sizeof (struct ifreq);
	struct ifconf ifc;
	char* buf = 0;
	for (;;) {
		buf = new char[len];
		ifc.ifc_len = len;
		ifc.ifc_buf = buf;
		if (::ioctl (sock, SIOCGIFCONF, &ifc) < 0) {
			if (errno != EINVAL || lastlen != 0) {
				close (sock);
				delete[] buf;
				return;
			}
		} else {
			if (ifc.ifc_len == lastlen) {
				break;
			}
			lastlen = ifc.ifc_len;
		}
		len += 10 * sizeof (struct ifreq);
		delete[] buf;
	}
	for (const char* ptr = buf; ptr < buf + ifc.ifc_len;) {
		const struct ifreq* ifr = reinterpret_cast<const struct ifreq*>(ptr);
		int rc = ioctl (sock, SIOCGIFHWADDR, ifr);
		if (rc != -1) {
			const struct sockaddr* sa = reinterpret_cast<const struct sockaddr*>(&ifr->ifr_hwaddr);
			if (sa->sa_family == ARPHRD_ETHER) {
				std::memcpy (&id, sa->sa_data, sizeof (id));
				break;
			}
		}
		ptr += sizeof (struct ifreq);
	}
	close (sock);
	delete[] buf;
}

#elif defined(PREDEF_PLATFORM_UNIX)
//
// General Unix
//
#include <sys/ioctl.h>
#if defined(sun) || defined(__sun)
#include <sys/sockio.h>
#endif
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <unistd.h>


void EnvironmentImpl::nodeIdImpl(NodeId& id)
{
	std::memset(&id, 0, sizeof(id));
	char name[MAXHOSTNAMELEN];
	if (gethostname(name, sizeof(name)))
		return;

	struct hostent* pHost = gethostbyname(name);
	if (!pHost) return;

	int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1) return;

	struct arpreq ar;
	std::memset(&ar, 0, sizeof(ar));
	struct sockaddr_in* pAddr = reinterpret_cast<struct sockaddr_in*>(&ar.arp_pa);
	pAddr->sin_family = AF_INET;
	std::memcpy(&pAddr->sin_addr, *pHost->h_addr_list, sizeof(struct in_addr));
	int rc = ioctl(s, SIOCGARP, &ar);
	close(s);
	if (rc < 0) return;
	std::memcpy(&id, ar.arp_ha.sa_data, sizeof(id));
}


#endif

#endif

namespace bsw {
	std::string Environment::get (const std::string& name) {
		return EnvironmentImpl::getImpl (name);
	}

	std::string Environment::get (const std::string& name, const std::string& defaultValue) {
		if (has (name)) {
			return get (name);
		} else {
			return defaultValue;
		}
	}

	bool Environment::has (const std::string& name) {
		return EnvironmentImpl::hasImpl (name);
	}

	void Environment::set (const std::string& name, const std::string& value) {
		EnvironmentImpl::setImpl (name, value);
	}

	std::string Environment::osName () {
		return EnvironmentImpl::osNameImpl ();
	}

	std::string Environment::osDisplayName () {
		return EnvironmentImpl::osDisplayNameImpl ();
	}

	std::string Environment::osVersion () {
		return EnvironmentImpl::osVersionImpl ();
	}

	std::string Environment::osArchitecture () {
		return EnvironmentImpl::osArchitectureImpl ();
	}

	std::string Environment::nodeName () {
		return EnvironmentImpl::nodeNameImpl ();
	}

	std::string Environment::nodeId () {
		NodeId id;
		nodeId (id);
		char result[18];
		std::sprintf (result, "%02x:%02x:%02x:%02x:%02x:%02x",
					  id[0],
					  id[1],
					  id[2],
					  id[3],
					  id[4],
					  id[5]);
		return std::string (result);
	}

	void Environment::nodeId (NodeId& id) {
		return EnvironmentImpl::nodeIdImpl (id);
	}

	unsigned Environment::processorCount () {
		return EnvironmentImpl::processorCountImpl ();
	}

}