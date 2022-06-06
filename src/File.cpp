#include "File.h"

File::File() {
	_isOpen = false;
}

File::File(const char* str, const char* mode) {
	_isOpen = open(str, mode);
}

File::~File() {
	if (_isOpen) close();
}

bool File::open(const char* str, const char* mode) {
	fopen_s(&p_file, str, mode); // &p_file is a pointer of a pointer.......
	if (p_file == NULL) _isOpen = false;
	else _isOpen = true;
	return _isOpen;
}

size_t File::write(void* buf, size_t len) {
	if (!_isOpen) { return 0; }
	return fwrite(buf, 1, len, p_file);
}

size_t File::print(const char* str) {
	if (!_isOpen) { return 0; }
	return fprintf(p_file, str);
}

size_t File::read(void* buf, size_t len) {
	if (!_isOpen) { return 0; }
	if (feof(p_file)) { return 0; }
	return fread(buf, len, 1, p_file);
}

size_t File::read(unsigned char** p_buf) {
	if (!_isOpen) { return 0; }
	fseek(p_file, 0, SEEK_END); //file pointer to the end
	size_t fsize = ftell(p_file); //get file pointer position
	fseek(p_file, 0, SEEK_SET); //file pointer to the beggining
	*p_buf = (unsigned char*) malloc(fsize); //set the pointer
	if (*p_buf == nullptr) { return 0; }
	fread(*p_buf, fsize, 1, p_file); //read fsize bytes
	return fsize;
}

bool File::gets(char* str, size_t len) {
	if (!_isOpen) { return 0; }
	if (feof(p_file)) { return false; }
	fgets(str, len, p_file);
	return true;
}

bool File::eof() {
	if (!_isOpen) { return true; }
	return (bool) feof(p_file);
}

void File::close() {
	if (_isOpen) fclose(p_file);
}