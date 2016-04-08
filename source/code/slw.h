/**
* Copyright © Marcus Hansson <marcus.hansson@email.com>
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef __LUA_H_INCLUDED__
#define __LUA_H_INCLUDED__

#include <cstdlib>
#include <cstring>

#include <exception>

#include <lua.hpp>

#ifndef __log_msg
    #define __log_msg printf
#endif

#ifndef __log_error
    #define __log_error printf
#endif

#ifndef __log_warning
    #define __log_warning printf
#endif

#define __LUA_ENTRY_OUT int
#define __LUA_ENTRY_IN ( Lua::State& state, void* user )

/** \brief Dynamic string copy
 * Copies `length` characters from `src`, starting at `start`.
 * \param src the source string
 * \param start the starting index
 * \param lenght the length of the sub string
 * \return a pointer to the sub string
 * \note Shit in, shit out. `Start + length` must be < strlen(src)
 * \note Remember to `free` your substring when done
 */
static inline char*
strdcpy (
    const char* const src,
    const unsigned int start,
    const unsigned int length )
{
    char* substr = ( char* ) malloc( 1 + length * sizeof( char ) );
    memcpy( substr, src + start, length + 1 );
    substr[ length ] = ( char )0;

    return substr;
}

namespace Lua
{
	class State;

	typedef __LUA_ENTRY_OUT (*Entry) __LUA_ENTRY_IN;

    class State
    {
public:
        friend class Call;

        State( void )
            : state( lua_open() )
        {
            luaL_openlibs( state );
        }

        State( lua_State& state )
            : state( &state )
        {

        }

        ~State( void )
        {
            lua_close( state );
            state = NULL;
        }

        const bool
        load( const char* str, const bool str_is_file = true )
        {
            if ( str_is_file )
                luaL_loadfile( state, str );

            else
                luaL_loadstring( state, str );

            return lua_pcall( state, 0, LUA_MULTRET, 0 );
        }

		void
		registerfn( const char* event, Lua::Entry callback, void* user = NULL )
		{
			Lua::State::EntryPoint* point = new Lua::State::EntryPoint;

			point->entry = callback;
			point->event = event;
			point->state = this;
			point->user	 = user;

			lua_pushnumber( state, ( unsigned int )( point ) );
			lua_pushcclosure( state, &handler, 1 );
			lua_setglobal( state, event );
		}

        const bool
        pop( const char*& v )
        {
            if ( !lua_isstring( state, 0 ) )
                return false;

            else
            {
                v = lua_tostring( state, 0 );
                lua_pop( state, 1 );

                return true;
            }
        }

        const bool
        pop( bool& v )
        {
            if ( !lua_isboolean( state, 0 ) )
                return false;

            else
            {
                v = lua_toboolean( state, 0 );
                lua_pop( state, 1 );

                return true;
            }
        }

        template< typename NumT >
        const bool
        pop( NumT& v )
        {
            if ( !lua_isnumber( state, 0 ) )
                return false;

            else
            {
                v = ( NumT )lua_tonumber( state, 0 );
                lua_pop( state, 1 );

                return true;
            }
        }

    private:

		static int
		handler( lua_State* state )
		{
			Lua::State::EntryPoint* point = NULL;

			point = 
				( Lua::State::EntryPoint* ) 
				( unsigned int )
				( lua_tonumber( state, lua_upvalueindex( 1 ) ) );

			return ( *point->entry )( *point->state, point->user );
		}

		struct EntryPoint
		{
			Lua::Entry	entry;
			const char* event;
			Lua::State* state;
			void*		user;
		};

        lua_State* state;

		static int handlers;
    };

    class Call
    {
    public:

        static inline void
        get_field( lua_State* state, const char* fn_or_obj )
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
                        char* substr = strdcpy( fn_or_obj, boundary, block_l );

                        if ( !boundary )
                            lua_getglobal( state, substr );

                        else
                        {
                            lua_pushstring( state, substr );
                            ++level;

                            free( substr );

                            if ( !lua_istable( state, -2 ) )
                            {
                                __log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );

                                lua_pop( state, level );
                                return;
                            }

                            else
                            {
                                lua_gettable( state, -2 );
								lua_remove( state, -2 );
                            }
                        }

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
                    __log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );
                    lua_pop( state, level );
                }

                else
                {
                    const unsigned int block_l = current_i - boundary;
                    char* substr = strdcpy( fn_or_obj, boundary, block_l );

                    lua_pushstring( state, substr );
                    ++level;

                    if ( lua_istable( state, -2 ) )
                    {
						lua_gettable( state, -2 );
						lua_remove( state, -2 );
					}

                    else
                    {
                        __log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );
                        lua_pop( state, level );
                    }

                    free( substr );
                }
            }
        }

    public:

        Call( State& _state, const char* fn )
            : args( 0 )
            , state( *_state.state )
        {
			get_field( &state, fn );
        }

        Call&
        param( const unsigned int v )
        {
            lua_pushnumber( &state, v );
            ++args;

            return *this;
        }

        Call&
        param( const double v )
        {
            lua_pushnumber( &state, v );
            ++args;

            return *this;
        }

        Call&
        param( const char* v )
        {
            lua_pushstring( &state, v );
            ++args;

            return *this;
        }

        const char*
        call( const unsigned int nresults )
        {
            try
			{
				if ( lua_pcall( state.state, args, nresults, 0 ) )
				{
		            const char* error = lua_tostring( state.state, 0 );
			        lua_pop( state.state, 1 );

	                return error? error: "";
		        }
			}
			catch ( std::exception* e )
			{
				( void )e;
			}

            return "";
        }

    private:
        unsigned int args;
        lua_State& state;
    };
}

#endif // __LUA_H_INCLUDED__

#define call_prepare( state ) ( call_prepare_r* )( call_prepare_r( state ) )