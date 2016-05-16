#ifndef SLW_GET_FIELD_H
#define SLW_GET_FIELD_H

struct lua_State;

namespace SLW
{
	/* \brief get a field or object from a Lua state
	 * \return true if object or function was found
	 */
	inline bool
	get_field( lua_State* state, const char* fn_or_obj );
}

#endif//SLW_GET_FIELD_H