#ifndef _Type_H_
#define _Type_H_

#ifndef _WIN32
#include <stdint.h> // uint32 and such on Windows
#else
// used stdint.h replacements
#define uint32_t unsigned long
#define uint64_t unsigned long long
#define int32_t int
#define int64_t long long
#endif

#endif
