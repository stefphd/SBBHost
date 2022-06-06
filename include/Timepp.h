
#ifndef TIME_H
#define TIME_H

#include <time.h>
#include <stdio.h>

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

#ifdef OS_LINUX
#define localtime_s(pTm,pT) (*(pTm))=*localtime(pT) //in linux use localtime for localtime_s
#define sprintf_s(buf, len, ...) snprintf((buf), (len), __VA_ARGS__)
#endif

/*
* Class for timing purpose
* This is actually just a C++ (i.e. class) wrapper for the C type tm
* Note that not all functions has been implemented yet
*/

/*! \brief A class to get system time.
    \details Class to get the system time in Windows and Linux. Actually this is just a 
	wrapper for the localtime function of the time library.
    \author Stefano Lovato
    \date 2022
*/
class Time {
private:
	struct tm tm; //!< Variable to store the current time
	static constexpr int STR_LEN = 256; //!< String length to use as buffer.

public:


	/*! \brief Constructor.
	\details Constructor for Time object. It automatically updates the time
	*/
	Time() {
		update_time();
	}

	/*! \brief Update the time.
	\details Function to update the time stored in tm (to be called every time).
	*/
	void update_time() {
		time_t t = time(NULL);
		localtime_s(&tm, &t);
	}

	/*! \brief Get the current year.
    \details Function to get the current year.
	\param update A flag to update the time before getting the value (default is true).
	\return The current year.
	*/
	int getYear(bool update = true) { if (update) update_time(); return (tm.tm_year + 1900); }

	/*! \brief Get the current month.
    \details Function to get the current month.
	\param update A flag to update the time before getting the value (default is true).
	\return The current month.
	*/
	int getMonth(bool update = true) { if (update) update_time(); return (tm.tm_mon + 1); }

	/*! \brief Get the current month day.
    \details Function to get the current month day (from 1).
	\param update A flag to update the time before getting the value (default is true).
	\return The current month day.
	*/
	int getDay(bool update = true) { if (update) update_time(); return (tm.tm_mday); }

	/*! \brief Get the current hour.
    \details Function to get the current hour.
	\param update A flag to update the time before getting the value (default is true).
	\return The current hour.
	*/
	int getHour(bool update = true) { if (update) update_time(); return (tm.tm_hour); }

	/*! \brief Get the current minute.
    \details Function to get the current minute.
	\param update A flag to update the time before getting the value (default is true).
	\return The current minute.
	*/
	int getMin(bool update = true) { if (update) update_time(); return (tm.tm_min); }

	/*! \brief Get the current second.
    \details Function to get the current second.
	\param update A flag to update the time before getting the value (default is true).
	\return The current second.
	*/
	int getSec(bool update = true) { if (update) update_time(); return (tm.tm_sec); }

	/*! \brief Get the current date-time.
    \details Function to get the current date-time with format
	YYYY-MM-DD hh::mm::ss.
	\param str A pointer to char to store the string.
	\param len The length of the char buffer.
	\param update A flag to update the time before getting the value (default is true).
	\return The number of written char.
	*/
	int getData(char* str, size_t len, bool update = true) { return sprintf_s(str, len, "%d-%02d-%02d %02d:%02d:%02d", getYear(update), getMonth(update), getDay(update), getHour(update), getMin(update), getSec(update)); }
	
	/*! \brief Get the current data-time.
	\details Function to get the current data-time with format
	YYYY-MM-DD hh::mm::ss.
	\param update A flag to update the time before getting the value (default is true).
	\return The date-time as a string.
	*/
	std::string getData(bool update = true) {
		char str[STR_LEN] = { 0 };
		getData(str, STR_LEN, update);
		return (std::string)str;
	}
};

#endif