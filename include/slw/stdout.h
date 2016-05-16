#ifndef SLW_STDOUT_H
#define SLW_STDOUT_H

#include <cstdio>

#endif//SLW_STDOUT_H

#ifndef log_msg
#define log_msg printf
#endif

#ifndef log_warning
#define log_warning( format, ... ) \
	std::fprintf( stderr, format, __VA_ARGS__ )
#endif

#ifndef log_error
#define log_error( format, ... ) \
	std::fprintf( stderr, format, __VA_ARGS__ )
#endif