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

#ifndef SLW_STDOUT_H
#define SLW_STDOUT_H

#include <cstdio>

#endif//SLW_STDOUT_H

#ifndef log_msg
#define log_msg printf
#endif

#ifndef log_warning
#define log_warning( format, ... ) \
    std::fprintf( stderr, (format), __VA_ARGS__ )
#endif

#ifndef log_error
#define log_error( format, ... )\
    std::fprintf( stderr, (format), __VA_ARGS__ )
#endif
