// ----------------------------------------------------------------------------

#include <stdint.h>

#include "inc/types.h"

// ----------------------------------------------------------------------------

class HashKey
{
public:
	static Hash GetHashKey(const char* str);
	static Hash GetHashKey(const char* str, Hash initialHash);
	static Hash GetHashKey(const char* str, const char* concat);
	static Hash GetHashKey(const char* str, const char* concat, Hash initialHash);

private:
	static Hash GetHashKeySubString(const char* str, Hash initialHash);
	static Hash GetHashKeyFinalize(const char* str, Hash initialHash);
	
private:
	static const uint8_t m_LookupTable[];

private:
	HashKey() {}
};

// ----------------------------------------------------------------------------