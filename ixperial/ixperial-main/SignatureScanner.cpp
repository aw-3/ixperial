#include "stdafx.h"

int findPattern(const char *data, const char *mask, int dwLen) {
	for (int i = 0; i < dwLen; i++) {
		if (mask[i] != 0 && data[i] != mask[i])
			return 0;
	}

	return 1;
}

int FindSignature(int dwStartAddress, const char *aob, int dwLen) {
	const int maxSearch = 0xF00000;

	for (int i = 0; i < maxSearch; i++) {
		if (findPattern((char*)(dwStartAddress + i), aob, dwLen))
			return dwStartAddress + i;
	}

	return NULL;
}

