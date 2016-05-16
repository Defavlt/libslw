#ifndef SLW_TYPES_H
#define SLW_TYPES_H

#include <lua.hpp>

namespace SLW
{
	enum Types
	{
		// Would be nice if this could be kept somewhat vertically aligned.
		TNONE			= LUA_TNONE,		// (-1)
		TNIL			= LUA_TNIL,			// 0
		TBOOLEAN		= LUA_TBOOLEAN,		// 1
		TLIGHTUSERDATA	= LUA_TLIGHTUSERDATA,// 2
		TNUMBER			= LUA_TNUMBER,		// 3
		TSTRING			= LUA_TSTRING,		// 4
		TTABLE			= LUA_TTABLE,		// 5
		TFUNCTION		= LUA_TFUNCTION,	// 6
		TUSERDATA		= LUA_TUSERDATA,	// 7
		TTHREAD			= LUA_TTHREAD,		// 8
		NUMTAGS			= LUA_NUMTAGS		// 9
	};
}

#endif//SLW_TYPES_H