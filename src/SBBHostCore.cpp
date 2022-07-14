#include "SBBHostCore.h"

SBBHostCore::SBBHostCore() {
}

SBBHostCore::~SBBHostCore() {
	hostPort.~HostPort();
	//if (p_ringBuf != nullptr) p_ringBuf->~RingBuffer();
	/*for (int i = 0; i < MAX_LOGGED_SAMPLES; i++) {
		if (p_logData[i] != nullptr) p_logData[i]->~LogData(); 
	}*/
}

void SBBHostCore::set_params(Params *params) {
	p_params = params;
}

int SBBHostCore::connect(bool logdata, std::string log_path) {
	if (p_params->conn_type == Params::SERIAL)  {
		if (p_params->port == NULL_PORT) { //port is empty, try connection if only one serial port found
			std::vector<int> portsAvailable = Serial::get_availableSerialPorts(MAX_PORT - 1);
			if (!portsAvailable.size()) { return EXIT_NOSERIALPORT; } //no serial port found, return
			else if (portsAvailable.size() > 1) { return EXIT_MULTIPLEPORT; } //multiple serial port found, return
			p_params->port = portsAvailable[0]; //assign the first serial port (only one is found)
		}

		hostPort.begin(p_params->port, p_params->baud, p_params->header, p_params->terminator, p_params->timeout); //begin host port
		if (!hostPort) { return EXIT_UNABLECONNECT; }
	}
	else if (p_params->conn_type == Params::SOCKET) {
		hostPortSocket.begin(p_params->socket_ip.c_str(), p_params->socket_port, p_params->header, p_params->terminator, p_params->timeout); //begin host port socket	
		if (!hostPortSocket) { return EXIT_UNABLECONNECT; }
	} 
	else {
		return EXIT_UNEXPECTED;
	}
	dolog = logdata;
	if (dolog) beginlog(log_path); //begin log
	return EXIT_CONNECTIONOK;  //all is ok
}
 
void SBBHostCore::disconnect() {
	hostPort.close();
	hostPortSocket.close();
}

void SBBHostCore::reset_stream() {
	numOfMissingPackets = 0;
	samples = 0;
	dolog = false;
	tmp_name = ""; 
}

int SBBHostCore::stream(bool writeFlag) {
	if (p_params->conn_type == Params::SERIAL)  {
		if (!hostPort.read((unsigned char*)&rx_packet.value, p_params->num_of_signals*sizeof(float))) { //read serial
			numOfMissingPackets++; //increase num of missing packets
			if (numOfMissingPackets > p_params->max_missing_packets) { return EXIT_LOSTCONNECTION; } //if max num of missing packets reached return EXIT_LOSTCONNECTION
			return EXIT_MISSINGPACKET; //otherwise return EXIT_MISSINGPACKET
		}
		if (writeFlag) hostPort.write((unsigned char*)&tx_packet.value, p_params->num_of_controls * sizeof(float)); //write only if writeFlag=true and read success
	}
	else if (p_params->conn_type == Params::SOCKET) {
		if (!hostPortSocket.read((unsigned char*)&rx_packet.value, p_params->num_of_signals*sizeof(float))) { //read serial
			numOfMissingPackets++; //increase num of missing packets
			if (numOfMissingPackets > p_params->max_missing_packets) { return EXIT_LOSTCONNECTION; } //if max num of missing packets reached return EXIT_LOSTCONNECTION
			return EXIT_MISSINGPACKET; //otherwise return EXIT_MISSINGPACKET
		}
		if (writeFlag) hostPortSocket.write((unsigned char*)&tx_packet.value, p_params->num_of_controls * sizeof(float)); //write only if writeFlag=true and read success
	}
	else {
		return EXIT_UNEXPECTED;
	}
	if (dolog) {
		if (logData.back()->push_back(rx_packet.value) == 0) { return EXIT_MAXLOGGEDDATA; }
	}
	numOfMissingPackets = 0; //all ok, reset numOfMissingPackets
	++samples;
	return EXIT_DEFAULT; //all ok, return EXIT_DEFAULT
}

void SBBHostCore::beginlog(std::string path) {
	//gen log name
	char buf[FILENAME_MAXSIZE] = { 0 };
	sprintf_s(buf, FILENAME_MAXSIZE, FILENAME_FORMAT_STRING, FILENAME_FORMAT_ENTRIES);
	tmp_name = buf;
	logData.push_back(new LogData<float>(tmp_name, path, p_params->signals, id_count++, PACKET_SIZE, MAX_LOGGED_SAMPLES,new Time(), getenv("USERNAME")));
}

int SBBHostCore::find(int id) {
	int i = 0;
	while (i < logData.size()) {
		if (id == logData[i]->id) { return i; }
		i++;
	}
	return NULL_ID;
}

int SBBHostCore::open(std::string str) {
	p_file = new File();
	if (!p_file->open(str.c_str(), "rb")) { return EXIT_OPENUNKNOWISS; }
	//read file
	uint8_t *data;
	size_t num_of_bytes = p_file->read(&data);
	//check file dim
	//printf("nun bytes: %d\n", num_of_bytes);
	if (num_of_bytes == 0) {
		free(data); //important
		p_file->close();
		return EXIT_INVALIDFILE;
	}
	if (num_of_bytes % (PACKET_SIZE*sizeof(float))) {
		free(data); //important
		p_file->close();
		return EXIT_INVALIDFILE;
	}
	//get log name
	int i;
	for (i = str.size()-1; i >= 0; --i) {
		if (str.at(i) == FILE_SEP) { break; }
	}
	std::string logname = str.substr(i+1);
	logname = logname.substr(0, logname.size()-4);
	std::string logpath = str.substr(0, i);
	//read data
	//put data in logBuffer
	size_t num_samples = num_of_bytes/(PACKET_SIZE*sizeof(float));
	logData.push_back(new LogData<float>(logname, logpath, p_params->signals, id_count++, PACKET_SIZE, num_samples));
	for (int i = 0; i < num_samples; ++i) { // add each logged sample
		logData.back()->push_back((float*) (data + sizeof(float)*PACKET_SIZE*i));
	}
	logData.back()->saved = true; //assumed saved when opening file
	//free and close
	free(data); //important
	p_file->close();
	return EXIT_DEFAULT;
}

int SBBHostCore::save(int id) {
	int i = find(id);
	if (i == -1) { return EXIT_SAVEUNKNOWISS; }
	std::string str = logData[i]->path + FILE_SEP + logData[i]->name + FILE_EXT;

	p_file = new File();
	if (!p_file->open(str.c_str(), "wb")) { return EXIT_SAVEUNKNOWISS; }

	if (logData[i]->save(p_file) != logData[i]->num_samples) {
		p_file->close();
		return EXIT_SAVEUNKNOWISS;
	}

	p_file->close();

	logData[i]->saved = true;

	return EXIT_DEFAULT;
}

int SBBHostCore::save(int id, std::string name, std::string path) { 
	
	int i = find(id);
	if (i == -1) { return EXIT_SAVEUNKNOWISS; }

	logData[i]->name = name;
	logData[i]->path = path;

	return save(id);
}

int SBBHostCore::save_dat(int id, std::string str) {

	int i = find(id);
	if (i == -1) { return EXIT_SAVEUNKNOWISS; }

	p_file = new File();
	if (!p_file->open(str.c_str(), "w")) { return EXIT_SAVEUNKNOWISS; }

	if (save_dat_private(p_file, logData[i]) != logData[i]->num_samples) {
		p_file->close();
		return EXIT_SAVEUNKNOWISS;
	}

	p_file->close();
	return EXIT_DEFAULT;
}

int SBBHostCore::save_mat(int id, std::string str) {

	int i = find(id);
	if (i == -1) { return EXIT_SAVEUNKNOWISS; }

	if (!save_mat_private(str, logData[i])) { return EXIT_SAVEUNKNOWISS; }

	return EXIT_DEFAULT;
}

size_t SBBHostCore::save_dat_private(File* p_file, LogData<float>* p_logData){
	if (p_file == nullptr) { return 0; }
	{   //print header
		//info
		std::string num_sample_str;
		std::string num_signal_str;
		sprintf_s(num_sample_str.data(), 256, "%lu", (unsigned long)p_logData->num_samples);
		sprintf_s(num_signal_str.data(), 256, "%lu", (unsigned long)p_params->num_of_signals);
		p_file->print("SBB log file\n");
		p_file->print("Created by: "), p_file->print(p_logData->getAuthor().c_str()), p_file->print("\n");
		p_file->print("Created at: "), p_file->print(p_logData->getDate().c_str()), p_file->print("\n");
		p_file->print("Number of samples: "), p_file->print(num_sample_str.c_str()), p_file->print("\n");
		p_file->print("Number of signals: "), p_file->print(num_signal_str.c_str()), p_file->print("\n");
		p_file->print("Thanks to: "), p_file->print(LIST_AUTHOR), p_file->print("\n");
		p_file->print("\n");

		//signal names
		char str[4096] = { 0 };
		size_t counter_char = 0;
		for (int i = 0; i < p_params->num_of_signals; ++i) {
			if (i == (p_params->num_of_signals - 1)) { //last signal, new line used
				size_t char_written = sprintf_s(str+counter_char, 4096-counter_char, "%s\n", p_logData->signals[i].c_str());
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
			else if (i == -1) { //first value is sample (corrisponding to -1 here)
				size_t char_written = sprintf_s(str + counter_char, 4096 - counter_char, "%s\t", "Sample");
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
			else { //other signals, comma used
				size_t char_written = sprintf_s(str+counter_char, 4096-counter_char, "%s\t", p_logData->signals[i].c_str());
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
		}
		p_file->print(str);
	}
	//print data to file
	for (size_t j = 0; j < p_logData->num_samples; ++j) {
		char str[4096] = { 0 };
		size_t counter_char = 0;
		for (int i = -1; i < p_params->num_of_signals; ++i) { //starts from -1 which is the sample
			if (i == (p_params->num_of_signals - 1)) { //last signal, new line used
				size_t char_written = sprintf_s(str+counter_char, 4096-counter_char, "%f\n", p_logData->get(i, j));
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
			else if (i == -1) { //first value is sample (corrisponding to -1 here)
				size_t char_written = sprintf_s(str + counter_char, 4096 - counter_char, "%lu\t", (unsigned long) j);
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
			else { //other signals, comma used
				size_t char_written = sprintf_s(str+counter_char, 4096-counter_char, "%f\t", p_logData->get(i, j));
				counter_char += char_written;
				if (char_written < 0) { return 0; }
			}
		}
#ifdef OS_WIN
		//sub comma with dot and \t with
		for (int i = 0; i < counter_char; ++i) if (str[i] == ',') str[i] = '.';
#endif
		//print str to file
		p_file->print(str);
	}
	//p_logData->saved = true;
	return p_logData->num_samples;
}


bool SBBHostCore::save_mat_private(std::string filename, LogData<float>* p_logData){
	//open the mat file
	TinyMATWriterFile* p_matFile=TinyMATWriter_open(filename.c_str());
	if (!p_matFile) { return false; }
	//write info to struct
	TinyMATWriter_writeString(p_matFile, "author", p_logData->getAuthor());
	TinyMATWriter_writeString(p_matFile, "date", p_logData->getDate());
	TinyMATWriter_writeString(p_matFile, "thanks", (std::string)LIST_AUTHOR);
	TinyMATWriter_writeValue<double>(p_matFile, "num_samples", p_logData->num_samples);
	TinyMATWriter_writeValue<double>(p_matFile, "num_signals", p_logData->num_signals);

	//write data
	for (int i = 0; i < p_params->num_of_signals; ++i) {
		//convert float vector to double
		std::string var_name = p_logData->signals[i];
		std::vector<float> var_data_float = p_logData->get(i);
		std::vector<double> var_data(var_data_float.begin(),var_data_float.end());
		//create variable name (signal name with ' ' replaced by '_')
		for (int j = 0; j < var_name.size(); ++j) {
			if (var_name[j] == ' ') var_name[j] = '_';
		}
		//write to mat file
		TinyMATWriter_writeDoubleVector(p_matFile, var_name.c_str(), var_data, true);
	}
	//write samples
	std::string var_name = "Sample";
	std::vector<size_t> var_data_float = p_logData->samples;
	std::vector<double> var_data(var_data_float.begin(),var_data_float.end());
	TinyMATWriter_writeDoubleVector(p_matFile, var_name.c_str(), var_data, true);
	//close the mat file and return
	TinyMATWriter_close(p_matFile);
	//p_logData->saved = true;
	return true;
}

int SBBHostCore::get_lastUnsaved(int* id) {
	if (logData.size() == 0) { return EXIT_NOLOGDATA; }
	int i = logData.size() - 1;
	while (i>=0) {
		if (!logData[i]->saved) {
			*id = logData[i]->id;
			return EXIT_DEFAULT;
		}
		i--;
	}
	*id = NULL_ID;
	return EXIT_ALLSAVED;
}

bool SBBHostCore::check_allSaved() {
	//return false if one or more logs not saved
	if (logData.size() == 0) { return true; } //no log, return true by default
	int i = 0;
	while (i < logData.size()) {
		if (!logData[i]->saved) { return false; } //unsaved log found, return false
		++i;
	}
	return true; //al logs saved, return true
}

void SBBHostCore::set_path_logs(std::string folder) {
	for (int i = 0; i < logData.size(); ++i) {
		if (!logData[i]->saved) logData[i]->path = folder;
	}
}

bool SBBHostCore::delete_log(int id) {
	int i = find(id);
	if (i < 0 )  { return false; } //log id not found, return false7
	logData.erase(logData.begin() + i);
	return true; //log deleted, return true for success
}