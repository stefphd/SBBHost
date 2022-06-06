
#ifndef INIREADER_H
#define INIREADER_H

#include "File.h"
#include <string>
#include <vector>

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
#define sscanf_s sscanf //in linux use sscanf for sscanf_s
#endif

/*! \brief A class for configuration-file reading.
    \details Class for reading and parsing of configuration files (e.g. with *.ini extension).
	File format used is the following
	 ```
	# Example config file.
	# Use hash for comments.
	# Comments can be also put after the semicolon in the variable assignment.
	# Be careful to use two white spaces around '=', quotes around values and semicolon at the end of the statements.
	var0 = 'helloworld'; a varialbe var0 with value 'helloworld'
	var1 = '1';	a variable var1 with value '1'
	list0 = 'hello, ciao, salut, hola'; a 
	list1 = '0, 1, 2, 3'; a list of integers
	```
    \author Stefano Lovato
    \date 2022
*/
class IniReader {
public:
	/*! \brief Constructor.
		\details Default constructor for the file class.
		\param name The name of the configuration file to use. File is not open.
	*/
	IniReader(const char* name);

	/*! \brief Desctructor.
		\details Default destructor.
	*/
	~IniReader();

	/*! \brief Get integer variable.
		\details Function to get an integer variable. The configuration file is open and closed at the end of the operation.
		\param key Name of the variable.
		\param val Pointer to the int variable to store the value.
		\return true if success, false otherwise.
	*/
	bool getInt(const char* key, int* val);

	/*! \brief Get double variable.
		\details Function to get a double variable. The configuration file is open and closed at the end of the operation.
		\param key Name of the variable.
		\param val Pointer to the double variable to store the value.
		\return true if success, false otherwise.
	*/
	bool getDouble(const char* key, double* val);

	/*! \brief Get an hex number.
		\details Function to get a hex number as an unsigned int variable. The configuration file is open and closed at the end of the operation.
		\param key Name of the variable.
		\param val Pointer to the unsigned int variable to store the hex value.
		\return true if success, false otherwise.
	*/
	bool getHex(const char* key, unsigned int* val);

	/*! \brief Get string variable.
		\details Function to get a string variable. The configuration file is open and closed at the end of the operation.
		\param key Name of the variable.
		\param val Pointer to the std::string variable to store the string.
		\return true if success, false otherwise.
	*/
	bool getString(const char* key, std::string* val);

	/*! \brief Get a string vector.
		\details Function to get a vector of strings. The configuration file is open and closed at the end of the operation.
		\param key Name of the list.
		\param str_array Pointer to the std::vector<std::string> variable to store the strings.
		\return true if success, false otherwise.
	*/
	bool getStringArray(const char* key, std::vector<std::string>* str_array);

	/*! \brief Get a double vector.
		\details Function to get a vector of double. The configuration file is open and closed at the end of the operation.
		\param key Name of the list.
		\param double_array Pointer to the std::vector<std::double> variable to store the values.
		\return true if success, false otherwise.
	*/
	bool getDoubleArray(const char* key, std::vector<double>* double_array);

private:
	File * p_file; //!< Poinnter to the File object.
	std::string _name; //!< Name of the configuration file.
};

#endif
