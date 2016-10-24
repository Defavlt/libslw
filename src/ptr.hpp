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

#ifndef RES_PTR_H
#define RES_PTR_H

#include <stddef.h>
#include <memory>
#include <map>

namespace res
{
template<typename T, void (*destroy)(T *), typename _ptr_t = std::shared_ptr<T>, typename _shared_t = std::enable_shared_from_this<T>>
struct ptr_t : public _ptr_t, public _shared_t {
public:

    typedef _ptr_t ptr_type;
    typedef _shared_t shared_t;

    ptr_t()
        : _ptr_t { NULL, destroy } {

    }

    ptr_t(T *t)
        : _ptr_t { t, destroy } {

    }
};
}

#endif//RES_PTR_H
