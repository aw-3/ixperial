#pragma once

int findPattern(const char *data, const char *mask, int dwLen);
int FindSignature(int dwStartAddress, const char *aob, int dwLen);

// compile time strlen
template<size_t N>
constexpr size_t length(char const (&)[N])
{
	return N - 1;
}

#define sigScan(addr, aob) FindSignature(addr, aob, length(aob))

