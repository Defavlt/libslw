#include <lua.hpp>

#include <cstdlib>
#include <cstring>

#include "slw/get_field.h"
#include "slw/cstrings.h"
#include "slw/stdout.h"

inline bool
SLW::get_field( lua_State* state, const char* fn_or_obj )
{
	const   unsigned int length     = strlen( fn_or_obj );
	unsigned int current_i  = 0;
	unsigned int boundary   = 0;
	unsigned int level      = 0;

	for ( ; current_i < length; )
	{
		const char& current_c = fn_or_obj[ current_i ];

		switch( current_c )
		{
		case '.':
			{
				const unsigned int block_l = current_i - boundary;
				char* substr = SLW::strdcpy( fn_or_obj, boundary, block_l );

				if ( !boundary )
					lua_getglobal( state, substr );

				else
				{
					lua_pushstring( state, substr );
					++level;

					if ( !lua_istable( state, -2 ) )
					{
						log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );

						lua_pop( state, ( int ) level );
						return false;
					}

					else
					{
						lua_gettable( state, -2 );
						lua_remove( state, -2 );
					}
				}

				// Lua copies the string internally
				free( substr );

				++current_i;
				boundary = current_i;

				break;
			}

		default:
			++current_i;
			break;
		}
	}

	if ( !boundary )
		lua_getglobal( state, fn_or_obj );

	else
	{
		if ( lua_isnil( state, -1 ) )
		{
			log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );
			lua_pop( state, ( int ) level );

			return false;
		}

		else
		{
			const unsigned int block_l = current_i - boundary;
			char* substr = SLW::strdcpy( fn_or_obj, boundary, block_l );

			lua_pushstring( state, substr );
			++level;

			if ( lua_istable( state, -2 ) )
			{
				lua_gettable( state, -2 );
				lua_remove( state, -2 );
			}

			else
			{
				log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );
				lua_pop( state, ( int ) level );

				return false;
			}

			free( substr );
		}
	}

	return true;
}