#include "slw/State.h"

int
SLW::State::handlers = 0;

int
SLW::State::handler( lua_State* state )
{
	SLW::State::EntryPoint* point = NULL;

	point = 
		( SLW::State::EntryPoint* ) 
		( size_t )
		( lua_tonumber( state, lua_upvalueindex( 1 ) ) );

	return ( *point->entry )( *point->state, point->user );
}