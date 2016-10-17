#include "stdafx.h"


void Utility::GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileNameA(GetModuleHandleA(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}

void Utility::GetFilesInDirectory(std::vector<std::string>& out, const std::string & directory, unsigned char includePath)
{
	HANDLE dir;
	WIN32_FIND_DATAA file_data;

	if ((dir = FindFirstFileA((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;


		out.push_back(includePath ? full_file_name : file_name);
	} while (FindNextFileA(dir, &file_data));

	FindClose(dir);
}
