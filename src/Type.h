#ifndef _Type_H_
#define _Type_H_

#include <string>
#include <math.h>
#include <ostream>

#ifndef _WIN32
#include <stdint.h> // uint32 and such on Windows
#else
// used stdint.h replacements
#define uint32_t unsigned long
#define uint64_t unsigned long long
#define int32_t int
#define int64_t long long
#endif

#include <sstream>
#include <glm/glm.hpp>

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t	i64;

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::vec4 rgba;

#endif

