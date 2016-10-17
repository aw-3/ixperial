#pragma once

namespace Utility {
	void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize);
	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath);
}