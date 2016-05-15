/**
* Copyright © The Authors
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
*
*	Authors:
*	Marcus Hansson <marcus.hansson@email.com>
*	André Andersson <andre.eric.andersson@gmail.com>
**/

#ifndef __LUA_H_INCLUDED__
#define __LUA_H_INCLUDED__

#include <cstdlib>
#include <cstring>

#include <exception>

#include <lua.hpp>

#include "slw/warnings.h"
#include "slw/cstrings.h"

#ifndef log_msg
    #define log_msg printf
#endif

#ifndef log_error
    #define log_error printf
#endif

#ifndef log_warning
    #define log_warning printf
#endif

#define __LUA_ENTRY_OUT int
#define __LUA_ENTRY_IN ( SLW::State& state, void* user )

#define __LUA_ENTRY( NAME ) __LUA_ENTRY_OUT NAME __LUA_ENTRY_IN

namespace SLW
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

					// lua copies the string internally
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

	class State;

	typedef __LUA_ENTRY_OUT (*Entry) __LUA_ENTRY_IN;

    class State
    {
public:
        friend class Call;
		friend class Table;

        State( void )
            : state( luaL_newstate() )
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

        bool
        load( const char* str, const bool str_is_file = true )
        {
            if ( str_is_file )
                luaL_loadfile( state, str );

            else
                luaL_loadstring( state, str );

DISABLE_WARNING( "", "", 4800 ) //performance warning: forcing value to bool

            return lua_pcall( state, 0, LUA_MULTRET, 0 );

ENABLE_WARNING( "", "", 4800 )
        }

		void
		registerfn( const char* event, SLW::Entry callback, void* user = NULL )
		{
			SLW::State::EntryPoint* point = new SLW::State::EntryPoint;

			point->entry = callback;
			point->event = event;
			point->state = this;
			point->user	 = user;

			lua_pushnumber( state, ( size_t )( point ) );
			lua_pushcclosure( state, &handler, 1 );
			lua_setglobal( state, event );
		}
		
		/* \brief Pop a value from the stack, optionally at an offset 
		 * \return true if the stack isn't empty and there's an actual
		 *     value
		 *     
		 * \param v the pop'd value
		 * \note remember to free the value if it's a string
		 */
		bool
        pop( const char*& v, int offset = 0, const bool force = false )
        {
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isstring( state, index ) )
			{
				if ( force )
					lua_pop( state, 1 );

				return false;
			}

            else
            {
                v = lua_tostring( state, index );
				v = SLW::strdcpy( v, 0, strlen( v ) );

                lua_pop( state, 1 );

                return true;
            }
        }

        bool
        pop( bool& v, int offset = 0, const bool force = false )
        {
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isboolean( state, index ) )
            {
				if ( force )
					lua_pop( state, 1 );

				return false;
			}

            else
            {

DISABLE_WARNING( "", "", 4800 ) //performance warning: forcing value to bool

                v = lua_toboolean( state, index );
                lua_pop( state, 1 );
ENABLE_WARNING( "", "", 4800 )

                return true;
            }
        }

        template< typename NumT >
        bool
        pop( NumT& v, int offset = 0, const bool force = false )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

            if ( _top == 0 || !lua_isnumber( state, index ) )
			{
				if ( force )
					lua_pop( state, 1 );

				return false;
			}

            else
            {
                v = ( NumT )lua_tonumber( state, index );
                lua_pop( state, 1 );

                return true;
            }
        }

		bool
		pop( int offset = 0 )
		{
			const int _top	= lua_gettop( state ) + offset;

			if ( _top == 0 )
				return false;

			else
			{
				lua_pop( state, 1 );
				return true;
			}
		}

		template< typename VecT, unsigned int length >
		bool
		pop( VecT& v, int offset = 0 )
		{
			int _top	= lua_gettop( state );

			bool success = true;

			int state_i = _top + offset;
			int vec_i	= length;

			for ( ; success 
				&& _top > 0 
				&& ( success = lua_isnumber( state, state_i ) );
				--vec_i, --state_i, _top = lua_gettop( state ) )
			{
				v[ vec_i ] = ( float )lua_tonumber( state, state_i );
				lua_pop( state, state_i );
			}

			return success;
		}

		/* \brief Peek at the stack - optionally from an offset
		 * \return true if the stack isn't empty and there's an actual
		 *     value
		 *
		 * \param v the value in the stack
		 **/
		bool
		peek( const char*& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

			if ( _top == 0 || !lua_isstring( state, index ) )
				return false;

			else
			{
				v = lua_tostring( state, index );
				v = SLW::strdcpy( v, 0, strlen( v ) );

				return true;
			}
		}

		bool
		peek( bool& v, int offset = 0 )
		{
			const int index = top() + offset;
			const int _top	= lua_gettop( state );

			if ( _top == 0 || !lua_isboolean( state, index ) )
				return false;

			else
			{

DISABLE_WARNING( "", "", 4800 ) //performance warning: forcing value to bool

				v = lua_toboolean( state, index );

ENABLE_WARNING( "", "", 4800 )

				return true;
			}
		}

		template< typename NumT >
		bool
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

DISABLE_WARNING( "", "", 4706 ) //assignment within conditional expression

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

ENABLE_WARNING( "", "", 4706 )

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

		void
		setglobal( const char* field )
		{
			lua_setglobal( state, field );
		}

		void
		dostring( const char* str )
		{
			luaL_dostring( state, str );
		}

		/* \brief the current size of the stack
		 **/
		int
		size( void )
		{
			return lua_gettop( state );
		}

		int
		type( int index = -1 )
		{
			return lua_type( state, index );
		}

		void
		debug( void )
		{
			/*
				LUA_TNONE		(-1)
				LUA_TNIL		0
				LUA_TBOOLEAN		1
				LUA_TLIGHTUSERDATA	2
				LUA_TNUMBER		3
				LUA_TSTRING		4
				LUA_TTABLE		5
				LUA_TFUNCTION		6
				LUA_TUSERDATA		7
				LUA_TTHREAD		8

				LUA_NUMTAGS		9
			*/

			int _type = 0;
			int i = -size();

			for ( ; i < 0; ++i )
				_type = type( i );
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
			SLW::Entry	entry;
			const char* event;
			SLW::State* state;
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

		Table( SLW::State& state, const int index = -1 )
			: index( (index > 0)? index * -1: index )
			, state( state )
		{
			/*bool table = */lua_istable( state.state, index );	
			/*int top = state.size();*/
		}

		Table& Debug( void )
		{
			int size = state.size();
			int type = 0;

			for ( int i = -1; i >= -size; --i )
				type = lua_type( state.state, i );

			return *this;
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
			SLW::get_field( state.state, field );

			result = lua_istable( state.state, index );

			/* stack:
			 *   1 table
			 */

			return *this;
		}

		/* \brief Set table element this[field] = v
		 *
		 **/
		Table&
		Set( const char* field, const char* v )
		{
			state.push( v );
			lua_setfield( state.state, index, field );

			return *this;
		}

		Table&
		Set( const char* field, bool v )
		{
			state.push( v );
			lua_setfield( state.state, index, field );

			return *this;
		}

		template< typename NumT >
		Table&
		Set( const char* field, NumT v )
		{
			state.push( v );
			lua_setfield( state.state, index, field );

			return *this;
		}

		template< typename VecT, unsigned int length >
		Table&
		Set( const VecT v )
		{
			for ( unsigned int i = 0; i < length; ++i )
			{
				state.push( v[ i ] );
				state.debug();

				// remember: lua array fields start 1
				lua_rawseti( state.state, index - 1, i + 1 );
			}

			return *this;
		}

		/* \brief Get table element this[field]
		 * \note Reuse the `result´ in subsequent calls
		 *     for a neat abort on error effect
		 * \note remember to free v if it's a string
		 **/
		Table&
		Get( const char* field, const char*& v )
		{
			lua_getfield( state.state, index, field );

			state.pop( v, 0, true );

			return *this;
		}

		Table&
		Get( const char* field, bool& v )
		{
			lua_getfield( state.state, index, field );

			state.pop( v, 0, true );

			return *this;
		}

		template< typename NumT >
		Table&
		Get( const char* field, NumT& v )
		{
			lua_getfield( state.state, index, field );

			Debug();

			state.pop( v, 0, true );

			return *this;
		}

		template< typename VecT, unsigned int length >
		Table&
		Get( const char* field, VecT& v )
		{
			lua_getfield( state.state, index, field );

			int type = lua_type( state.state, index );

			if ( type == LUA_TTABLE )
			{
				lua_pushnil( state.state );

				int i = 0;

				/*
				* stack:
				*   1 nil
				*   2 table
				*/

				for ( ; /*i < length && */lua_next( state.state, index - 1 ); ++i )
				{
					/*
					* stack:
					*   1 value
					*   2 key
					*   3 table
					*/

					state.pop( v[ i ] );

					/*
					* stack:
					*   1 key
					*   2 table
					*/
				}
			}

			lua_pop( state.state, 1 );

			return *this;
		}

		Table&
		Get( int field, bool& v )
		{
			lua_rawgeti( state.state, index, field );

			state.pop( v, 0, true );

			return *this;
		}

		Table&
		Get( int field, const char*& v )
		{
			lua_rawgeti( state.state, index, field );

			state.pop( v, 0, true );

			return *this;
		}

		template< typename NumT >
		Table&
		Get( int field, NumT& v )
		{
			lua_rawgeti( state.state, index, field );

			Debug();

			state.pop( v, 0, true );

			return *this;
		}

		template< typename VecT, unsigned int length >
		Table&
		Get( int field, VecT& v )
		{
			lua_rawgeti( state.state, index, field );

			int type = lua_type( state.state, index );

			if ( type == LUA_TTABLE )
			{
				lua_pushnil( state.state );

				int i = 0;

				/*
				* stack:
				*   1 nil
				*   2 table
				*/

				for ( ; /*i < length && */lua_next( state.state, index - 1 ); ++i )
				{
					/*
					* stack:
					*   1 value
					*   2 key
					*   3 table
					*/

					state.pop( v[ i ] );

					/*
					* stack:
					*   1 key
					*   2 table
					*/
				}
			}

			lua_pop( state.state, 1 );

			return *this;
		}

	private:
		const Table& operator=( const Table& );

		const int index;

		SLW::State& state;
	};
}

#endif // __LUA_H_INCLUDED__