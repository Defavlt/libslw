#include "slw/Call.h"
#include "slw/slw.h"

#include <lua.hpp>

SLW::Call::Call( SLW::State& state, const char* fn ) 
	: call_ref( SLW::get_field( state.state, fn )
		? luaL_ref( state.state, LUA_REGISTRYINDEX )
		: 0 )
	, args( 0 )
	, state( state )
{
}

SLW::Call::Call( SLW::State& state, const int index )
	: call_ref( 0 )
	, args( 0 )
	, state( state )
	, function_found( false )
{
	lua_pushvalue( state.state, index );

	call_ref = luaL_ref( state.state, LUA_REGISTRYINDEX );
}

SLW::Call::~Call( void )
{
	luaL_unref( state.state, LUA_REGISTRYINDEX, call_ref );
}

SLW::Call&
SLW::Call::clear( void )
{
	args = 0;

	return *this;
}

SLW::Call&
SLW::Call::param( const char* v )
{
	lua_pushstring( state.state, v );
	++args;

	return *this;
}

SLW::Call&
SLW::Call::param( void )
{
	++args;

	return *this;
}

bool
SLW::Call::call( const unsigned int nresults /*= 0*/ )
{
	if ( call_ref == 0 )
		return false;

#if defined( LUA_AS_CPP )
	try
	{
#endif
		const int fn_index = -( ( int ) ( args ) ) - 1;

		if ( state.type( fn_index ) != LUA_TFUNCTION )
		{
			lua_rawgeti( state.state, LUA_REGISTRYINDEX, call_ref );
			lua_insert( state.state, fn_index );
		}

		if ( lua_pcall( state.state, args, nresults, 0 ) )
		{
			const char* error = lua_tostring( state.state, -1 );

			DISABLE_WARNING( "-Wformat-security", "-Wformat-security", 4000 );
				log_error( error );
			ENABLE_WARNING( "-Wformat-security", "-Wformat-security", 4000 );

				lua_pop( state.state, 1 );

			return false;
		}

#if defined( LUA_AS_CPP )
	}
	catch ( std::exception* e )
	{
		throw e;
	}
#endif

	return true;
}