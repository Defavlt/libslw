#include "slw/Table.h"
#include "slw/State.h"

#include "slw/get_field.h"

#include <lua.hpp>

SLW::Table::Table( SLW::State& state, const int index /* = -1 */ )
	: index( (index > 0)? index * -1: index )
	, state( state )
{
}

SLW::Table&
SLW::Table::Allocate( const int narr, const int nrec )
{
	lua_createtable( state.state, narr, nrec );

	return *this;
}

bool
SLW::Table::Find( const char* field, bool& result )
{
	return SLW::get_field( state.state, field );

	/* stack:
		*   1 table
		*/
}