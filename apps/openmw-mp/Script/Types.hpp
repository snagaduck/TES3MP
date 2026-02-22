#ifndef TMPTYPES_HPP
#define TMPTYPES_HPP

#ifdef _WIN32
#include <winsock2.h>
typedef HMODULE lib_t;
#else
typedef void* lib_t;
#endif

#endif //TMPTYPES_HPP
