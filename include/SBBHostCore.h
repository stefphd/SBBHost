
#ifndef SBBHOSTCORE_H
#define SBBHOSTCORE_H

#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "Params.h"
#include "HostPortUDP.h"
#include "HostPortTCP.h"
#include "HostPort.h"
#include "Packet.h"
#include "LogData.h" 
#include "Timepp.h"
#include "File.h"
#include "tinymatwriter.h"
#include "SBBUtils.h"

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
#define sprintf_s(buf, len, ...) snprintf((buf), (len), __VA_ARGS__)
#endif

/*! \brief A class for the core of the SBB Host application.
    \details Class for the core management of the SBB Host Application. 
	This class is used for signal streaming and logging via the 
	the host port, and to store, save and open the logged-data files.
	\see SBBHostApp
    \author Stefano Lovato
    \date 2022
*/
class SBBHostCore {
private:
	/*! \brief Private routine to save *.csv format.
		\details Private function to save logged-data in *.csv format. The file must be open before and closed after.
		\param p_file Pointer to the File object used to save the file.
		\param p_logData Pointer to the LogData object to be saved.
		\return The number of written char.
	*/
	size_t save_dat_private(File* p_file, LogData<float>* p_logData); //save csv file routing (not in LogData b/c type-dependent) - not used in SBBHostApp and thus private
	
	/*! \brief Private routine to save *.mat file.
		\details Private function to save logged-data in *.mat (MATLAB) format. The file must not be open before and closed after.
		\param filename The name of the file to save (including the path)
		\param p_logData Pointet to the LogData object to be saved.
		\return true if success, false otherwise.
	*/
	bool save_mat_private(std::string filename, LogData<float>* p_logData); //save csv file routing (not in LogData b/c type-dependent) - not used in SBBHostApp and thus private

public: //almost all members are public b/c SBBHostApp should have (almost) the complete access to SBBHostCore
	//config variables - grouped in struct
	Params * p_params; //!< Pointer to the parameters structure created in SBBHostApp.

	//variables
	unsigned int numOfMissingPackets = 0; //!< Number of current missing packets.
	int id_count = 0; //!< Counter of logged-data ID.
	long long samples = -1; //!< Number of current logged sample.
	//size_t num_loggedData = 0;
	bool dolog = false; //!< true if do data logging.
	std::string tmp_name = ""; //!< Temporary name for the logged-data.

	//objects
	HostPort hostPort; //!< HostPort object.
	HostPortTCP hostPortTCP; //!< HostPortTCP object.
	HostPortUDP hostPortUDP; //!< HostPortUDP object.
	Packet<float, PACKET_SIZE> rx_packet; //!< Rx Packet object.
	Packet<float, PACKET_SIZE> tx_packet; //!< Tx Packet object
	Time time; //!< Time object.

	//pointers to object
	std::vector<LogData<float>*> logData; //!< Vector of pointers to LogData with base type float.
	File * p_file = { nullptr }; //!< Pointer to File object.

	//functions
	/*! \brief Constructor.
		\details Default constractor.
	*/
	SBBHostCore(); 

	/*! \brief Desctructor.
		\details Default destructor. It automatically destroies the host port.
	*/
	~SBBHostCore();

	/*! \brief Set the parameters.
		\details Function to set the pointer to the parameters structure.
		\param params Pointer to the parameter struct variable.
	*/
	void set_params(Params* params);

	/*! \brief Reset the data streaming.
		\details Function to reset the data streaming when finished.
	*/
	void reset_stream();

	/*! \brief Perform a stream step.
		\details Function to perform a stream step. Must be called continously.
		\param writeFlag The flag write the tx packet.
		\return An exit flag (see config.h for details).
	*/
	int stream(bool writeFlag);

	/*! \brief Begin the data logging.
		\details Function to begin the data logging with a given path. The log name is automatically 
		generated from the current system time.
		\param path The default path where to save the log when necessary.
	*/
	void beginlog(std::string path);

	/*! \brief Connect the host port.
		\details Function to connect the host port and begin the data streaming.
		\param logdata The flag to enable the data logging.
		\param log_path The string og the default path where to save the log when necessary,
		\return An exit flag (see config.h for details).
	*/
	int connect(bool logdata = false, std::string log_path = "");

	/*! \brief Disconnect the host port.
		\details Function to disconnect the host port and stop the data streaming.
	*/
	void disconnect();

	/*! \brief Save a log as a *.sbb file.
		\details Function to save a log as a *.sbb file by specifying its ID with the default names.
		\param id The ID of the logged-data to save.
		\return An exit flag (see config.h for details).
	*/
	int save(int id);

	/*! \brief Save a log as a *.sbb file.
		\details Function to save a log as a *.sbb file by specifying its ID with specific name and path.
		Name and path are assigned to the logged-data.
		\param id The ID of the logged-data to save.
		\param name The name of the file to save.
		\param path The path of the file to save.
		\return An exit flag (see config.h for details).
	*/
	int save(int id, std::string name, std::string path);

	/*! \brief Save a log as *.dat file.
		\details Function to save a log as a *.dat file by specifying its ID. The first column is the Sample, while the other 
		the signals. Values are separated by a comma.
		\param id The ID of the logged-data to save.
		\param str The path/name string of the file to save
		\return An exit flag (see config.h for details).
	*/
	int save_dat(int id, std::string str);

	/*! \brief Save a log as *.csv file.
		\details Function to save a log as a *.mat file by specifying its ID. The tinymatlibrary is used to save the *.mat file.
		\param id The ID of the logged-data to save.
		\param str The path/name string of the file to save.
		\return An exit flag (see config.h for details).
	*/
	int save_mat(int id, std::string str);

	/*! \brief Open a log from a *.sbb file.
		\details Function to open a *.sbb file and load the logged data.
		\param str The path/name string of the file to open.
		\return An exit flag (see config.h for details).
	*/
	int open(std::string str);

	/*! \brief Find a log using the ID.
		\details Function to find a log by specifying its ID.
		\param id The ID of the log to find.
		\return The corresponding index in the logData vector (NULL_ID if not found).
	*/
	int find(int id);

	/*! \brief Get the ID of the last unsaved log.
		\details Function to get the ID of the last unsaved logged data.
		\param id Pointer to the ID of the log.
		\return An exit flag (see config.h for details).
	*/
	int get_lastUnsaved(int *id);

	/*! \brief Check if all log are saved.
		\details Function to check if are logged data are saved.
		\return true if all logs have been saved, false otherwise.
	*/
	bool check_allSaved();

	/*! \brief Set the default path of all logs.
		\details Function to set the default path of all logs.
	*/
	void set_path_logs(std::string folder);

	/*! \brief Delete a log.
		\details Function to a log by specifying its ID.
		\param id The ID of the log to delete.
		\return true if success, false otherwise.
	*/
	bool delete_log(int id);

};


#endif

