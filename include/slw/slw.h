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

#ifndef SLW_STATE_H
#define SLW_STATE_H

#include <exception>

#include "slw/types.h"
#include "slw/warnings.h"
#include "slw/cstrings.h"
#include "slw/stdout.h"

#define _LUA_ENTRY_OUT int
#define _LUA_ENTRY_IN ( SLW::State& state, void* user )

#define __LUA_ENTRY( NAME ) _LUA_ENTRY_OUT NAME _LUA_ENTRY_IN

namespace SLW
{
	class State;

	typedef _LUA_ENTRY_OUT ( *Entry ) _LUA_ENTRY_IN;

    class State
    {
public:
        friend class Call;
		friend class Table;

        State( void );

		State( State& _state )
			: state( NULL )
		{
			state = _state.state;
		}

		State( lua_State* _state );

        virtual ~State( void );

        bool
        load( const char* str, const bool str_is_file = true );

		void
		registerfn( const char* event, SLW::Entry callback, void* user = NULL );
		
		/* \brief Pop a value from the stack, optionally at an offset 
		 * \return true if the stack isn't empty and there's an actual
		 *     value
		 *     
		 * \param v the pop'd value
		 * \note remember to free the value if it's a string
		 */
		bool
        pop( const char*& v, int offset = 0, const bool force = false );

        bool
        pop( bool& v, int offset = 0, const bool force = false );

		bool
		pop( int offset = 0 );

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
		peek( const char*& v, int offset = 0 );

		bool
		peek( bool& v, int offset = 0 );

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
		push( const char* v );

		void
		push( const bool v );

		template< typename NumT >
		void
		push( const NumT v )
		{
			lua_pushnumber( state, v );
		}

		void
		setglobal( const char* field );

		void
		dostring( const char* str );

		/* \brief the current size of the stack
		 **/
		int
		size( void );

		int
		type( int index = -1 );

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
}

#endif // SLW_STATE_H