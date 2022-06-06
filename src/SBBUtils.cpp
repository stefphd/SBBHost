#include "SBBUtils.h"

namespace utils {

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