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

#ifndef SLW_CALL_H
#define SLW_CALL_H

#include <functional>

#include "slw/state.hpp"
#include "slw/types.hpp"

namespace slw
{
class Call
{
public:
    typedef std::function<int(slw::State &, void *)> entry_t;

    /* \brief construct a new SLW::Call by
     *   using the supplied function name
     */
    Call(slw::State &state, slw::string_t);

    /* \brief construct a new callback by
     *   using the supplied function, name, and optional user data
     */
    Call(slw::State &, slw::string_t event, entry_t, void * = nullptr);

    ~Call();

    /* \brief reset the parameter count of this call
     */
    void clear();

    /* \brief push a parameter onto the stack
     */
    void param(slw::string_t);
    void param(slw::number_t);
    void param(long);
    void param(int);
    void param(char);
    void param(bool);

    /* \brief increment the number of pushed parameters
     * \note useful if you push values onto the stack
     */
    void param();

    /* \brief set manually the number of parameters on the stack
     * \note useful if you push values onto the stack
     */
    void set_param(int);

    /*
     * \brief Push parameters, and call the function
     *
     */
    template<typename ...args_t>
    bool operator ()(args_t ...args) {
        param(args...);
        return call();
    }

    /* \brief pop all arguments from the stack and call the
     * initially supplied function.
     * \param [optional] nresults number of results to expect
     * \return true if no errors (syntax or others) happened
     * \note Define LUA_AS_CPP to catch (and re-throw) exceptions
     */
    bool call(const unsigned int nresults = 0);

private:
    const Call &operator=( const Call &);

    static int handler( lua_State *state );

    struct entry_data_t {
        entry_data_t(const entry_data_t &);
        entry_data_t(entry_t, slw::string_t, slw::Call &, void *);

        entry_t entry;
        string_t event;
        slw::Call &call;
        void *user;
    };

    typedef std::vector<entry_data_t> entries_t;

    entries_t entries;

    slw::State &state;
    int call_ref;
    bool isValid;
    unsigned int args;
};
}

#endif//SLW_CALL_H
