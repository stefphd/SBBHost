#include "IniReader.h"


IniReader::IniReader(const char* name) {
	p_file = new File();
	_name = name;
}

IniReader::~IniReader() {
}

bool IniReader::getInt(const char* key, int* val) {

	//open 
	if (!p_file->open(_name.c_str(), "r")) {
		return false;
	}

	//build format
	std::string format = key;
	format.append(" = '%d';%s\n");

	//read the file until eof
	while (!p_file->eof()) {
		char str[260] = { 0 };
		char garbage[260] = { 0 };
		p_file->gets(str, 260);
		if (str[0] == '#') { continue; }
		if (sscanf_s(str, format.c_str(), val, garbage) > 0) {
			p_file->close();
			return true;
		}
	}
	p_file->close();
	return false;

}

bool IniReader::getDouble(const char* key, double* val) {

	//open 
	if (!p_file->open(_name.c_str(), "r")) {
		return false;
	}

	//build format
	std::string format = key;
	format.append(" = '%lf';%s\n");

	//read the file until eof
	while (!p_file->eof()) {
		char str[260] = { 0 };
		char garbage[260] = { 0 };
		p_file->gets(str, 260);

		//in Windows replace . with ,
		#ifdef OS_WIN
		for (int i = 0; i < 260; ++i) {
			if (str[i] == '.') str[i] = ',';
		}
		#endif
		
		if (str[0] == '#') { continue; }
		if (sscanf_s(str, format.c_str(), val, garbage) > 0) {
			p_file->close();
			return true;
		}
	}
	p_file->close();
	return false;

}


bool IniReader::getHex(const char* key, unsigned int* val) {

	//open 
	if (!p_file->open(_name.c_str(), "r")) {
		return false;
	}

	//build format
	std::string format = key;
	format.append(" = '0x%x';%s\n");

	//read the file until eof
	while (!p_file->eof()) {
		char str[260] = { 0 };
		char garbage[260] = { 0 };
		p_file->gets(str, 260);
		if (str[0] == '#') { continue; }
		if (sscanf_s(str, format.c_str(), val, garbage) > 0) {
			p_file->close();
			return true;
		}
	}
	p_file->close();
	return false;

}

bool IniReader::getString(const char* key, std::string* val) {

	//open 
	if (!p_file->open(_name.c_str(), "r")) {
		return false;
	}

	//build format
	std::string format = key;
	format.append(" = %s\n");

	//read the file until eof
	while (!p_file->eof()) {
		char str[260] = { 0 };
		p_file->gets(str, 260);
		if (str[0] == '#') { continue; } //skip comments
		std::string str_s = str;
		if (str_s.find(key) != std::string::npos) {
			p_file->close();
			size_t first = str_s.find("'", 0) + 1;
			size_t last = str_s.find("'", first) - 1;
			*val = str_s.substr(first, last - first + 1);
			return true;
		}
	}
	p_file->close();
	return false;

}

bool IniReader::getStringArray(const char* key, std::vector<std::string>* str_array) {
	//open 
	if (!p_file->open(_name.c_str(), "r")) {
		return false;
	}

	//build format
	std::string format = key;
	format.append(" = %s\n");

	//read the file until eof
	while (!p_file->eof()) {
		char str[260] = { 0 };
		p_file->gets(str, 260);
		if (str[0] == '#') { continue; } //skip comments
		std::string str_s = str;
		if (str_s.find(key) != std::string::npos) {
			if (str_s[str_s.length() - 1] == '\n') str_s.erase(str_s.length() - 1);
			size_t first = str_s.find("'", 0) + 1; //first '
			size_t last = str_s.find("'", first) - 1; //last '
			//cat the string with next line if any
			while ((last+1) == std::string::npos) { //last ' not found, new line
				char line[260] = { 0 };
				p_file->gets(line, 260);
				if (line[0] == '#') { continue; } //skip comments
				std::string line_s = line;
				str_s.append(line);
				if (str_s[str_s.length() - 1] == '\n') str_s.erase(str_s.length() - 1);
				last = str_s.find("'", first) - 1; //last '
				if (p_file->eof()) {  //end of file reached w/o finding '
					p_file->close(); 
					return false;  
				}
			}
			p_file->close();
			str_s = str_s.substr(first, last - first + 1);
			//separate values by commas
			size_t pos = 0; //init
			while (true) {
				pos = str_s.find(",", 0);
				str_array->push_back(str_s.substr(0, pos)); //read the string
				if (pos == std::string::npos) { // ',' not found, reading finished, exit
					break;
				}
				while (str_s[pos + 1] == ' ') { //remove white spaces after
					str_s.erase(pos + 1, 1);
				}
				str_s = str_s.substr(pos + 1); //remove read string
			}
			return true;
		}
	}
	p_file->close();
	return false;

}

bool IniReader::getDoubleArray(const char* key, std::vector<double>* double_array) {
	//convert to string array
	std::vector<std::string> std_array;
	if (!getStringArray(key, &std_array)) { return false; }
	//create save into double array
	for (int i = 0; i < std_array.size(); ++i) {
		std::string str = std_array[i];
		//in Windows replace . with ,
		#ifdef OS_WIN
		for (int j = 0; j < str.size(); ++j) {
			if (str[j] == '.') str[j] = ',';
		}
		#endif
		double_array->push_back(std::stod(str));
	}
	return true;
}