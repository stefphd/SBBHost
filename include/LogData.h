
#ifndef LOGDATA_H
#define LOGDATA_H

#include <string>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include "File.h"
#include "Timepp.h"

/*! \brief A templated-class for managing logged-data.
    \details Templated-class for managing logged-data with used-defined type, used in SBBHostCore.
	\see SBBHostCore
	\tparam T The base type of the packet.
    \author Stefano Lovato
    \date 2022
*/
template<typename T>
class LogData {
public:

	//variables
	size_t num_samples; //!< Number of logged samples.
	size_t num_signals; //!< Number of logged signals.
	size_t max_num_samples; //!< Max number of logged samples (reserved for vectors).
	std::vector<std::vector<T>> data; //!< Vector of vector of type T (i.e. a matrix with num_signals rows and max_num_samples columns)
	std::vector<size_t> samples; //!< Vector of samples.
	std::vector<std::string> signals; //!< Vector of signal names.
	std::string name; //!< Name of the log.
	std::string path; //!< Path of the corresponding log file.
	std::string author; //!< Author of the log.
	Time* time; //!< Variable to store the initial time.
	int id = -1; //!< ID of the log.
	bool saved; //!< True if the log has been saved.
	/*! \brief Constructor.
		\details Default constructor of the logged data.
		\param str_name Th name of the logged data.
		\param str_path The path of the corresponding file (if any).
		\param signal_names The vector of signal names.
		\param ID The ID of the log.
		\param num_of_signals The number of signals of the log.
		\param num_of_samples The number of samples of the log.
	*/
	LogData(std::string str_name, std::string str_path, 
		std::vector<std::string> signal_names, int ID, 
		size_t num_of_signals, size_t num_of_samples, 
		Time* p_time = nullptr,  std::string theAuthor = "unknown") {
		name = str_name;
		path = str_path;
		saved = false;
		signals = signal_names;
		id = ID;
		time = p_time;
		author = theAuthor;
		num_samples = 0;
		max_num_samples = num_of_samples;
		num_signals = num_of_signals;
		data.resize(num_signals);
		for (int i = 0; i < num_signals; i++) data[i].reserve(max_num_samples);
		samples.reserve(max_num_samples);
	};

	/*! \brief Destructor.
		\details Default destructor. It clears the logged data.
	*/
	~LogData() {
		data.clear();
	};

	/*! \brief Get a single sample of a signal.
		\details Function to get a single sample of a given signal.
		\param i The index of the signal.
		\param j The index of the sample.
		\return The j-th sample of the i-th signal.
	*/
	T get(int i, int j) { return data[i][j]; } //return data (i,j), i.e. one element (i-th signal and j-th sample)
	
	/*! \brief Get all samples of a signal.
		\details Function to get all samples of a given signal.
		\param i The index of the signal.
		\return The vector containing the i-th signal data.
	*/
	std::vector<T> get(int i) { return data[i]; } //return data(i,:), i.e. all samples of i-th signal
	
	/*! \brief Push data to the log buffer.
		\details Function to push data to the log buffer.
		\param packet The pointer to the signal packet to pust
		\return The number of written elements.
	*/
	int push_back(T* packet) { //append packet data to buffer
		if (num_samples > (max_num_samples - 1)) { return 0; } //return 0 if max_num_samples reached
		for (int i = 0; i < num_signals; ++i) data[i].push_back(packet[i]);
		samples.push_back(++num_samples);
		return num_signals;
	};

	/*! \brief Get author name
		\details Function to get the author of the log.
		\return The name of the author.
	*/
	std::string getAuthor() {
		return author;
	}

	/*! \brief Get the date.
		\details Function to get the creation date and time of the log.
		\return The data as a formatted string.
	*/
	std::string getDate() {
		if (time) {
			return time->getData(false);
		}
		else {
			return (std::string) "0000-00-00 00:00:00";
		}
	}

	/*! \brief Save the log.
		\details Function to save the log to a file. The file must be open before. The file is not closed after the saving.
		\param p_file The pointer to the File object.
		\return The number of samples written to the file.
	*/
	size_t save(File* p_file) { //save to file, return the number of samples saved
		if (p_file == nullptr) { return 0; }
		T* buf = (T*)malloc(sizeof(T) * num_signals);
		if (buf == NULL) { free(buf);  return 0; }
		for (size_t j = 0; j < num_samples; ++j) {
			for (int i = 0; i < num_signals; ++i) buf[i] = get(i, j);
			if (num_signals * sizeof(T) != p_file->write((unsigned char*)buf, num_signals * sizeof(T))) {
				free(buf);
				return j;
			}
		}
		free(buf);
		saved = true;
		return num_samples;
	}
};

#endif