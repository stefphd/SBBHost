/*
* File for utility (non-class) functions
*/

#ifndef UTILS_H
#define UTILS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#ifndef OS_WIN
#define OS_WIN
#endif
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNUC__) || defined(__GNUG__) || defined(unix) || defined(__unix) || defined(__unix__)
#ifndef OS_LINUX
#define OS_LINUX
#endif
#else
#error OS not supported
#endif

#if defined(OS_WIN)
#pragma warning(disable : 4996)
#include <shlobj.h>  //for get document path in Win
#include <WinUser.h> //for beep
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(OS_LINUX)
#define sscanf_s sscanf //in linux use sscanf for sscanf_s
#define sprintf_s(buf, len, ...) snprintf((buf), (len), __VA_ARGS__)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#endif

#include <time.h>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <math.h>


/*! \brief A namespace providing a set of standard colors.
    \details Namespace providing a set of RGB colors accordingly to the standard MATLAB color scale.
    \author Stefano Lovato
    \date 2022
*/
namespace Colors {
	/*! \brief red tone of the set of colors
	*/
	constexpr double red[] =	{ 0,		0.85,	0.929,	0.494,	0.466,	0.301,	0.635 };
	
	/*! \brief green tone of the set of colors
	*/
	constexpr double green[] =	{ 0.447,	0.325,	0.694,	0.184,	0.674,	0.745,	0.078 };
	
	/*! \brief blue tone of the set of colors
	*/
	constexpr double blue[] =	{ 0.741,	0.098,	0.125,	0.556,	0.188,	0.933,	0.184 };
	
	/*! \brief number of color in the color set.
	*/
	constexpr int size = 7;
};

/*! \brief A namespace providing utility functions.
    \details Namespace providing a number of utility functions.
    \author Stefano Lovato
    \date 2022
*/
namespace utils {

	/*! \brief Copy a file.
		\details Robust function to copy file from source to destination.
		\param FileSource Char array with the source file path.
		\param FileDestination Char array with the destination file path.
		\return 0 if success, -1 and -2 if cannot open source or destination file.
	*/
	int filecp(const char FileSource [], const char FileDestination []);

	/*! \brief Get the local IP and subnet mask.
		\details Function to get the local IP address and subnet mask.
		\param ip Pointer where saving the local IP.
		\param mask Pointer where saving the subnet mask.
		\param ip_str Pointer where saving the local IP in string format (XXX.XXX.XXX.XXX).
		\param mask_str Pointer where saving the subnet mask in string format (XXX.XXX.XXX.XXX).
		\return 0 if success, 1 otherwise
	*/
	int getIP_and_subnetmask(uint32_t* ip = nullptr, uint32_t* mask = nullptr, std::string* ip_str = nullptr, std::string* mask_str = nullptr);

	/*! \brief Get the remote IP.
		\details Function to get the remote IP from the local IP and subnet mask.
		\param ip Local IP.
		\param mask Subnet mask.
		\param str Pointer where save the remote IP in string format (XXX.XXX.XXX.XXX).
		\return The remote IP.
	*/
	uint32_t get_remoteIP(uint32_t ip, uint32_t mask, std::string* str = nullptr);

	/*! \brief Get the broadcast IP.
		\details Function to get the broadcast IP from the local IP and subnet mask.
		\param ip Local IP.
		\param mask Subnet mask.
		\param str Pointer where save the broadcast IP in string format (XXX.XXX.XXX.XXX).
		\return The broadcast IP.
	*/
	uint32_t get_broadcastIP(uint32_t ip, uint32_t mask, std::string* str = nullptr);

	/*! \brief Convert IP from string to uint32.
		\details Function to convert IP from string to uint32.
		\param ip_str The IP.
		\return The IP.
	*/
	uint32_t ip_str2num(std::string ip_str);

	/*! \brief Convert HEX from unsigned int to string
		\details Function to convert a HEX value (as an unsigned int) to a string.
		\param v The HEX value as an unsigned int.
		\return The HEX value as a string.
	*/
	std::string hex2str(unsigned int v);

	/*! \brief Convert HEX from string to unsigned int
		\details Function to convert a HEX (as a string) to an unsigned int
		\param str The HEX value as an string.
		\param p_v The pointer to the unsigned int value.
		\return true if success, false otherwise.
	*/
	bool str2hex(std::string str, unsigned int *p_v);

	/*! \brief Get the average of a vector of float.
		\details Function to get the average of a std::vector<float>.
		\return The average as a float type.
	*/
	float average(std::vector<float> const& v);

	/*! \brief Get the RMS of a vector of float.
		\details Function to get the RMS (root-mean-squared) of a std::vector<float>.
		\return The RMS value as a float type.
	*/
	float rms(std::vector<float> const& v);
	
	/*! \brief Get the HOME path.
		\details Function to get a home path (i.e. HOME in Linux, Documents in Windows).
		If Documents is not found in Windows, C:\ is used.
		File separator used is consistent with the operative system.
		\return A string denoting the home path. 
	*/
	std::string get_home_path();

	/*! \brief Send a beep message.
		\details Function to send a beep message in Windows. This is just a wrapper of the Windows function `MessageBeep(mode)`.
		In Linux it does nothing.
		\param mode The beep mode to be used. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebeep for details.
	*/
	void beep(int mode);

	/*! \brief Set a local environment variable.
		\details Function to set a local environment variable. This is just a wrapper for the `putenv` function.
		\param key The name of the environment variable.
		\param val The value of the environment variable.
		\return true if success, false otherwise.
	*/
	bool set_env_var(std::string key, std::string val);

	/*! \brief A structure for handling elapsed time in hour, minute, second format.
	*/
	struct hhmmss_t {
		int s;
		int m;
		int h;
	};

	/*! \brief Convert elapsed seconds to hh::mm:ss format.
		\details Function to convert elapsed seconds in hours, minutes, and seconds format.
		\param secs The elapsed seconds.
		\return A `hhmmss_t` struct containing hours, minutes, and seconds.
	*/
	hhmmss_t sec2hhmmss(int secs);

};

#endif