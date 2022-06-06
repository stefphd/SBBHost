
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <malloc.h>

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
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL //in linux use fopen for fopen_s
#endif

/*! \brief A class for file reading and writing.
    \details Class for file reading and writing wroking with Windows and Linux.
	Actually this is just a wrapper for the FILE type in the stdio C library.
    \author Stefano Lovato
    \date 2022
*/
class File {
public:
	/*! \brief Constructor.
		\details Default constructor. File is not open.
	*/
	File();

	/*! \brief Constructor.
		\details Constructor for the file class with opening.
		\param str The name of the file.
		\param mode The mode for the file opening. Refer to the stdio library for the meaning.
	*/
	File(const char* str, const char* mode);

	/*! \brief Desctructor.
		\details Default destructor. No parameter is necessary. It automatically closes the file if open.
	*/
	~File();

	/*! \brief Open the file.
		\details Function to open the file.
		\param str The name of the file.
		\param mode The mode for the file opening. Refer to the stdio library for the meaning.
		\return true if success, false otherwise.
	*/
	bool open(const char* str, const char* mode);

	/*! \brief Print a string.
		\details Function to print a string to the file.
		\param str The string to print.
		\return The number of written char (equal to the string length if success).
	*/
	size_t print(const char* str);

	/*! \brief Write a buffer.
		\details Function to write a buffer to the file.
		\param buf The pointer to the buffer.
		\param len The length of the buffer in bytes.
		\return The number of written char (equal to the buffer length if success).
	*/
	size_t write(void* buf, size_t len);

	/*! \brief Read a buffer.
		\details Function to read a buffer to the file.
		\param buf The pointer to the buffer.
		\param len The length of the buffer in bytes.
		\return The number of read char (equal to the buffer length if success).
	*/
	size_t read(void* buf, size_t len);

	/*! \brief Read all the file
		\details Function to read all the file into a buffer. Memory is automatically allocated according to the file size.
		\param p_buf The pointer of the pointer to the buffer.
		\return The number of read char (equal to the file size if sucess).
	*/
	size_t read(unsigned char** p_buf);

	/*! \brief Get a string.
		\details Function to get a string to the file.
		\param str The string used to store the char read.
		\param len The length of the string to read.
		\return true if success, false otherwise.
	*/
	bool gets(char* str, size_t len);

	/*! \brief Clone the file.
		\details Function to clone the file.
	*/
	void close();

	/*! \brief Check if open.
		\details Function to check if the file is open.
		\return true if the file is open, false otherwise
	*/
	bool isOpen() { return _isOpen; }

	/*! \brief Check end-of-file.
		\details Function to check if the end-of-file is reached.
		\return true if the end of file is reached, false otherwise.
	*/
	bool eof();
	operator bool() { return _isOpen; }

private:
	FILE* p_file; //!< Pointer to the C-type file
	bool _isOpen; //!< true if file is open

};

#endif