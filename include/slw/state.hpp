/**
* Copyright (C) 2016- The Authors
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
*	Andre Andersson <andre.eric.andersson@gmail.com>
**/

#ifndef SLW_STATE_H
#define SLW_STATE_H

#include <memory>
#include "slw/types.hpp"

struct lua_State;

namespace slw {

////////////////////////////////////////////////////////////
/// \brief A shared, or managed, Lua state
////////////////////////////////////////////////////////////
typedef std::shared_ptr<lua_State> shared_state;

////////////////////////////////////////////////////////////
/// \brief Make a new shared Lua state
////////////////////////////////////////////////////////////
shared_state make_state();

////////////////////////////////////////////////////////////
/// \brief Open the Lua standard libraries
////////////////////////////////////////////////////////////
void open_libs(shared_state);

////////////////////////////////////////////////////////////
/// \brief Get the current size of the stack, e.g. the top.
/// \return The size
////////////////////////////////////////////////////////////
slw::size_t get_size(const shared_state &);

////////////////////////////////////////////////////////////
/// \brief Copy a range of values from, and push them onto the stack
/// \return The starting index of the original values (e.g. size of stack - N)
////////////////////////////////////////////////////////////
slw::size_t push_range(slw::shared_state &state);

////////////////////////////////////////////////////////////
/// \brief Copy a range of values from, and push them onto the stack
/// \return The starting index of the original values (e.g. size of stack - N)
////////////////////////////////////////////////////////////
slw::size_t push_range(slw::shared_state &state, slw::size_t start, slw::size_t N);

////////////////////////////////////////////////////////////
/// \brief Debug the stack by printing the contents to stdout
/// \note DEBUG symbol must be defined
////////////////////////////////////////////////////////////
void debug(slw::shared_state &state);

////////////////////////////////////////////////////////////
/// \brief Clear the stack
/// \param N The number of elements to pop. If 0, clear the stack, but if
/// less than 0, pop (top - abs(N)) elements.
////////////////////////////////////////////////////////////
void clear(shared_state &, slw::int_t N = 0);

} //namespace slw

////////////////////////////////////////////////////////////
/// \typedef slw::shared_state
///
/// A managed Lua state. Automatically calls lua_close when
///  destroyed.
///
/// Usage example:
/// \code
/// // Declare, and create a new Lua state
/// slw::shared_state state = slw::make_state();
/// // Open the standard Lua libraries
/// slw::open_libs(state);
/// \endcode
///
/// \see std::shared_ptr
////////////////////////////////////////////////////////////

#endif // SLW_STATE_H
