#include "slw/slw.h"

int
Lua::State::handlers = 0;

int
Lua::State::handler( lua_State* state )
{
	Lua::State::EntryPoint* point = NULL;

	point = 
		( Lua::State::EntryPoint* ) 
		( size_t )
		( lua_tonumber( state, lua_upvalueindex( 1 ) ) );

	return ( *point->entry )( *point->state, point->user );
}