#include "SBBUtils.h"

namespace utils {
	
	int filecp(const char FileSource [], const char FileDestination [])
	{
		int   c;
		FILE *stream_R;
		FILE *stream_W; 

		stream_R = fopen (FileSource, "r");
		if (stream_R == NULL)
			return -1;
		stream_W = fopen (FileDestination, "w");   //create and write to file
		if (stream_W == NULL)
		{
			fclose (stream_R);
			return -2;
		}    
		while ((c = fgetc(stream_R)) != EOF)
			fputc (c, stream_W);
		fclose (stream_R);
		fclose (stream_W);

		return 0;
	}
	
	int getIP_and_subnetmask(uint32_t* ip, uint32_t* mask, std::string* ip_str, std::string* mask_str) {
		struct ifaddrs *ifap, *ifa;
		struct sockaddr_in *samask, *saip;
		char *addr;
		getifaddrs (&ifap);
		for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr->sa_family==AF_INET) {
				if (std::string(ifa->ifa_name) == "lo") { continue; } //skip local
				samask = (struct sockaddr_in *) ifa->ifa_netmask;
				saip = (struct sockaddr_in *) ifa->ifa_addr;
				if (ip) *ip = saip->sin_addr.s_addr;
				if (mask) *mask = samask->sin_addr.s_addr;
				if (ip_str) *ip_str = inet_ntoa(saip->sin_addr);
				if (mask_str) *mask_str = inet_ntoa(samask->sin_addr);
			freeifaddrs(ifap);
			return 0;
			}
		}
		freeifaddrs(ifap);
		return 1;
	}

	uint32_t get_remoteIP(uint32_t ip, uint32_t mask, std::string* str) {
		uint32_t remoteip = ip & mask;
		if (str) {
			sockaddr_in sa; 
			sa.sin_addr.s_addr = remoteip;
			*str = inet_ntoa(sa.sin_addr);
		}
		return remoteip;
	}

	uint32_t get_broadcastIP(uint32_t ip, uint32_t mask, std::string* str) {
		uint32_t broadcastip = ip | ~mask;
		if (str) {
			sockaddr_in sa; 
			sa.sin_addr.s_addr = broadcastip;
			*str = inet_ntoa(sa.sin_addr);
		}
		return broadcastip;
	}

	uint32_t ip_str2num(std::string ip_str) {
		in_addr ip;
		inet_aton(ip_str.c_str(), &ip);
		return (uint32_t) ip.s_addr;
	}

	std::string get_home_path() {
		#ifdef OS_WIN
		CHAR dir[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, dir);
		if (result == S_OK) {
			return (std::string)dir;
		}
		else {
			return (std::string)"C:\\"; //Document not found, return C:
		}
		#else //OS_LINUX
		return (std::string) getenv("HOME"); //working always?
		#endif
	}

	void beep(int mode) {
		#ifdef OS_WIN
		MessageBeep(mode);
		#else //OS_LINUX
		//no beep in linux
		#endif
	}

	bool set_env_var(std::string key, std::string val) {
		std::string cmd = key + "=" + val;
		#ifdef UTILS_WIN
		return _putenv(cmd.c_str());
		#else //UTILS_LINUX
		return putenv((char*) cmd.c_str());
		#endif
	}

	hhmmss_t sec2hhmmss(int secs) {
		hhmmss_t t;
		t.h = (secs / 3600) % 100;
		t.m = (secs / 60) % 60;
		t.s = secs % 60;
		return t;
	}

	float average(std::vector<float> const& v) {
		if (v.empty()) {
			return 0;
		}
		auto const count = static_cast<float>(v.size());
		return std::reduce(v.begin(), v.end()) / count;
	}

	float rms(std::vector<float> const& v) {
		if (v.empty()) {
			return 0;
		}
		float m = average(v);
		float accum = 0.0;
		std::for_each(std::begin(v), std::end(v), [&](const float d) {
			accum += (d - m) * (d - m); });
		return sqrt(accum / (v.size() - 1));
	}

	std::string hex2str(unsigned int v) {
	char str[260] = { 0 };
	sprintf_s(str, 260, "0x%X", v);
	return (std::string)str;
}

	bool str2hex(std::string str, unsigned int* p_v) {
		if (sscanf_s(str.c_str(), "0x%X", p_v) > 0) { 
			return true; 
		}
		return false;
	}

}