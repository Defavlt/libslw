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

	static inline bool
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

						// lua copies the string internally
						free( substr );

						if ( !lua_istable( state, -2 ) )
						{
							__log_error( "Attempt to index a nil value ('%s')\n", fn_or_obj );

							lua_pop( state, ( int ) level );
							return false;
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
				lua_pop( state, ( int ) level );

				return false;
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
					lua_pop( state, ( int ) level );

					return false;
				}

				free( substr );
			}
		}

		return true;
	}

	class State;

	typedef __LUA_ENTRY_OUT (*Entry) __LUA_ENTRY_IN;

    class State
    {
public:
        friend class Call;
		friend class Table;

        State( void )
            : state( lua_open() )
        {
            luaL_openlibs( state );
        }

		State( State& _state )
			: state( NULL )
		{
			state = _state.state;
		}

		State( lua_State* _state )
			: state( _state )
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

#define IW_MSC 4800 //performance warning: forcing value to bool
#include "wpush"

            return lua_pcall( state, 0, LUA_MULTRET, 0 );

#include "wpop"
        }

		void
		registerfn( const char* event, Lua::Entry callback, void* user = NULL )
		{
			Lua::State::EntryPoint* point = new Lua::State::EntryPoint;

			point->entry = callback;
			point->event = event;
			point->state = this;
			point->user	 = user;

			lua_pushnumber( state, ( size_t )( point ) );
			lua_pushcclosure( state, &handler, 1 );
			lua_setglobal( state, event );
		}

        const bool
        pop( const char*& v, int offset = 0 )
        {
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isstring( state, index ) )
                return false;

            else
            {
                v = lua_tostring( state, index );
				v = strdcpy( v, 0, strlen( v ) );

                lua_pop( state, 1 );

                return true;
            }
        }

        const bool
        pop( bool& v, int offset = 0 )
        {
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isboolean( state, index ) )
                return false;

            else
            {

#define IW_MSC 4800 //performance warning: forcing value to bool
#include "wpush"

                v = lua_toboolean( state, index );
                lua_pop( state, 1 );

#include "wpop"

                return true;
            }
        }

        template< typename NumT >
        const bool
        pop( NumT& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isnumber( state, index ) )
                return false;

            else
            {
                v = ( NumT )lua_tonumber( state, index );
                lua_pop( state, 1 );

                return true;
            }
        }

		template< typename VecT, unsigned int length >
		const bool
		pop( VecT& v, int offset = 0 )
		{
			int _top	= lua_gettop( state );

			bool success = true;

			int state_i = _top + offset;
			int vec_i	= length;

			for ( ; success 
				&& _top > 0 
				&& ( success = lua_isnumber( state, state_i ) );
				--vec_i, --state_i )
			{
				v[ i ] = ( float )lua_tonumber( state, state_i );
				lua_pop( state, state_i );
			}

			return success;
		}

		const bool
		peek( const char*& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

			if ( _top == 0 || !lua_isstring( state, index ) )
				return false;

			else
			{
				v = lua_tostring( state, index );
				v = strdcpy( v, 0, strlen( v ) );

				return true;
			}
		}

		const bool
		peek( bool& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

			if ( _top == 0 || !lua_isboolean( state, index ) )
				return false;

			else
			{

#define IW_MSC 4800 //performance warning: forcing value to bool
#include "wpush"

				v = lua_toboolean( state, index );

#include "wpop"

				return true;
			}
		}

		template< typename NumT >
		const bool
		peek( NumT& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

			if ( _top == 0 || !lua_isnumber( state, index ) )
				return false;

			else
			{
				v = ( NumT )lua_tonumber( state, index );

				return true;
			}
		}

#define IW_MSC 4706 //assignment within conditional expression
#include "wpush"

		template< typename VecT, unsigned int length >
		bool
		peek( VecT& v, int offset = 0 )
		{
			int _top	= lua_gettop( state );

			bool success = true;

			int state_i = -1 - offset;
			int vec_i	= length - 1;

			for ( ; success 
				&& _top > 0 
				&& state_i > -_top 
				&& ( success = success && lua_isnumber( state, state_i ) );
			
				--vec_i, --state_i )
				{
					v[ vec_i ] = ( float )lua_tonumber( state, state_i );
				}

			return success;
		}

#include "wpop"

		void
		push( const char* v )
		{
			lua_pushstring( state, v );
		}

		void
		push( const bool v )
		{
			lua_pushboolean( state, v );
		}

		template< typename NumT >
		void
		push( const NumT v )
		{
			lua_pushnumber( state, v );
		}

		template< typename VecT, unsigned int length >
		void
		push( const VecT v )
		{
			for ( unsigned int i = 0; i < length; ++i )
				lua_pushnumber( state, v[ i ] );
		}

		int
		size( void )
		{
			return lua_gettop( state );
		}

	protected:

		virtual int
		top( void )
		{
			return -1;
		}

    private:

		static int
		handler( lua_State* state );

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

	/* \brief Create or modify a table
	 *
	 **/
	class Table
	{
	public:

		Table( Lua::State& state, const int index = -1 )
			: index( index )
			, state( state )
		{
			bool table = lua_istable( state.state, index );
		}

		/* \brief allocate space for and create a new table
		 * \param narr number of array elements
		 * \param nrec number of non-array elements
		 *
		 **/
		Table&
		Allocate( const int narr, const int nrec )
		{
			lua_createtable( state.state, narr, nrec );

			return *this;
		}

		Table&
		Find( const char* field, bool& result )
		{
			Lua::get_field( state.state, field );

			result = lua_istable( state.state, -1 );

			/* stack:
			 *   1 table
			 */
		}

		/* \brief Set table element this[field] = v
		 *
		 **/
		Table&
		Set( const char* field, const char* v )
		{
			state.push( v );
			lua_setfield( state.state, -1, field );

			return *this;
		}

		Table&
		Set( const char* field, bool v )
		{
			state.push( v );
			lua_setfield( state.state, -1, field );

			return *this;
		}

		template< typename NumT >
		Table&
		Set( const char* field, NumT v )
		{
			state.push( v );
			lua_setfield( state.state, -1, field );

			return *this;
		}

		/* \brief Get table element this[field]
		 * \note Reuse the `result´ in subsequent calls
		 *     for a neat abort on error effect
		 * \note remember to free v if it's a string
		 **/
		Table&
		Get( const char* field, const char*& v, bool& result )
		{
			if ( result )
			{
				lua_getfield( state.state, -1, field );

				result = state.pop( v );
			}

			return *this;
		}

		Table&
		Get( const char* field, bool& v, bool& result )
		{
			if ( result )
			{
				lua_getfield( state.state, -1, field );

				result = state.pop( v );
			}

			return *this;
		}

		template< typename NumT >
		Table&
		Get( const char* field, NumT& v, bool& result )
		{
			if ( result )
			{
				lua_getfield( state.state, -1, field );

				result = state.pop( v );
			}

			return *this;
		}

		template< typename VecT, unsigned int length >
		Table&
		Get( const char* field, VecT& v, bool& result )
		{
			if ( result )
			{
				lua_getfield( state.state, -1, field );

				result = state.peek< VecT, length >( v );
			}

			return *this;
		}

	private:
		const Table& operator=( const Table& );

		const int index;

		Lua::State& state;
	};

    class Call
    {
    public:

        Call( State& _state, const char* fn )
            : args( 0 )
            , state( _state )
        {
			Lua::get_field( state.state, fn );
        }

        Call&
        param( const unsigned int v )
        {
            lua_pushnumber( state.state, v );
            ++args;

            return *this;
        }

        Call&
        param( const double v )
        {
            lua_pushnumber( state.state, v );
            ++args;

            return *this;
        }

        Call&
        param( const char* v )
        {
            lua_pushstring( state.state, v );
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
		const Call& operator=( const Call& );

        unsigned int args;
        Lua::State& state;
    };
}

#endif // __LUA_H_INCLUDED__