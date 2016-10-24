#include <lua.hpp>

#include "slw/State.h"

SLW::State::State( void )
	: state( luaL_newstate() )
{
	luaL_openlibs( state );
}

SLW::State::State( lua_State* _state )
	: state( _state )
{

}

SLW::State::~State( void )
{
	lua_close( state );
	state = NULL;
}

bool
SLW::State::load( const char* str, const bool str_is_file /* = true */ )
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
SLW::State::registerfn( const char* event, SLW::Entry callback, void* user /* = NULL */ )
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

bool
SLW::State::pop( const char*& v, int offset /* = 0 */, const bool force /* = false */ )
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
SLW::State::pop( bool& v, int offset /* = 0 */, const bool force /* = false */ )
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

bool
SLW::State::pop( int offset /* = 0 */ )
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

bool
SLW::State::peek( const char*& v, int offset /* = 0 */ )
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
SLW::State::peek( bool& v, int offset /* = 0 */ )
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

void
SLW::State::push( const char* v )
{
	lua_pushstring( state, v );
}

void
SLW::State::push( const bool v )
{
	lua_pushboolean( state, v );
}

void
SLW::State::setglobal( const char* field )
{
	lua_setglobal( state, field );
}

void
SLW::State::dostring( const char* str )
{
	luaL_dostring( state, str );
}

int
SLW::State::size( void )
{
	return lua_gettop( state );
}

int
SLW::State::type( int index /* = -1 */ )
{
	return lua_type( state, index );
}