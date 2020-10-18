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

#ifndef GET_FIELD_HPP
#define GET_FIELD_HPP

#include <string>
#include <tuple>

struct lua_State;

namespace slw {
namespace internal {

////////////////////////////////////////////////////////////
/// \typedef cons A tuple of strings.
////////////////////////////////////////////////////////////
typedef std::tuple<std::string, std::string> cons;

////////////////////////////////////////////////////////////
/// \brief Split a list of items, separated by a specified separator,
/// but excluding the separator/s.
/// \param The separator with which the items are separated
/// \return The first item, or the head, and the rest, or the tail.
////////////////////////////////////////////////////////////
cons split_path(const std::string &, const char separator = '.');

////////////////////////////////////////////////////////////
/// \brief A collection of tokens, or parts, of a path,
/// with a specific separator, implemented as a forward-only collection.
////////////////////////////////////////////////////////////
class path {
public:
    ////////////////////////////////////////////////////////////
    /// \brief An iterator of the path collection
    ////////////////////////////////////////////////////////////
    class iterator : public std::iterator<
            std::forward_iterator_tag,
            std::string> {
        ////////////////////////////////////////////////////////////
        /// \brief The separator
        /// \see slw::internal::split_path
        ////////////////////////////////////////////////////////////
        const char M_separator;

        ////////////////////////////////////////////////////////////
        /// \brief The current element we're at, and the rest of the path
        ////////////////////////////////////////////////////////////
        cons M_cons;

        ////////////////////////////////////////////////////////////
        /// \brief Whether this iterator is the end
        ////////////////////////////////////////////////////////////
        bool M_end;
public:
        ////////////////////////////////////////////////////////////
        /// \brief Construct an iterator, using a specified path, and separator
        /// \param path The path to iterate
        /// \param separator The separator with which the part is delimited by.
        /// \see slw::internal::split_path
        ////////////////////////////////////////////////////////////
        explicit iterator(const std::string &path = "", const char separator = '.');

        ////////////////////////////////////////////////////////////
        /// \brief Increment this iterator
        /// \return A reference to this iterator
        ////////////////////////////////////////////////////////////
        iterator &operator ++();

        ////////////////////////////////////////////////////////////
        /// \brief operator ++ Increment this iterator
        /// \return A copy of this iterator
        ////////////////////////////////////////////////////////////
        iterator operator ++(int);

        ////////////////////////////////////////////////////////////
        /// \brief Compare this iterator, to another
        /// \param other The other iterator
        /// \return Whether this iterator is the other iterator
        ///
        /// Currently implemented as both a value-wise, *and* a
        /// identity-wise equality comparison. E.g.
        /// \code
        /// this == &other
        ///     || (this head == other head
        ///         && this tail == other tail
        ///         && this end == other end);
        /// \endcode
        ////////////////////////////////////////////////////////////
        bool operator ==(const iterator &other) const;

        ////////////////////////////////////////////////////////////
        /// \brief Compare this iterator, to another
        /// \param other The other iterator
        /// \return Whether this iterator is *not* the other iterator
        ////////////////////////////////////////////////////////////
        bool operator !=(const iterator &other) const;

        ////////////////////////////////////////////////////////////
        /// \brief Dereference this iterator
        /// \return A reference to this iterators head.
        ////////////////////////////////////////////////////////////
        reference operator *();

        ////////////////////////////////////////////////////////////
        /// \brief Get the *tail* of this iterator, e.g. the rest of the path.
        /// \return The tail.
        ////////////////////////////////////////////////////////////
        reference tail();
    };

    ////////////////////////////////////////////////////////////
    /// \brief Construct a path container sequence using
    /// a specified path, and separator.
    /// \param path The path
    /// \param separator The path delimiter
    ////////////////////////////////////////////////////////////
    explicit path (const std::string &path, const char separator = '.');

    ////////////////////////////////////////////////////////////
    /// \brief Get the first part, the head, of this path.
    /// \return An iterator for the head of this path.
    ////////////////////////////////////////////////////////////
    iterator begin();

    ////////////////////////////////////////////////////////////
    /// \brief Get the end iterator for this path.
    /// \return The end iterator of this path.
    ////////////////////////////////////////////////////////////
    iterator end();

private:
    ////////////////////////////////////////////////////////////
    /// \brief The end iterator for this path.
    ////////////////////////////////////////////////////////////
    const iterator M_end;

    ////////////////////////////////////////////////////////////
    /// \brief The head iterator for this path.
    ////////////////////////////////////////////////////////////
    iterator M_begin;
};

////////////////////////////////////////////////////////////
/// \brief Touch a path, making sure each object along it is
/// a valid container for the next.
/// \param path The path to follow
/// \param idx The valid index for the containing object.
/// \param separator The separator which delimits each object
///
/// In case the *last* object exists, but it isn't a LUA_TTABLE,
/// it won't be touched. The resulting object might be none, nil,
/// or anything else supported by Lua.
///
/// if idx is a stack position, or pseudo-index, then idx is removed.
///
/// \see lua_getfield
////////////////////////////////////////////////////////////
void touch_path(lua_State *, const std::string &path, int idx, char separator = '.');

}/*ns internal*/
}/*ns slw*/

////////////////////////////////////////////////////////////
/// \class slw::internal::path
/// \ingroup internals
///
/// A member of the internal namespace, slw::internal::path
/// is the main driver behind this whole library. Use it for
/// resolving object paths.
///
/// Usage example:
/// \code
/// // The path works with underscores, special signs (e.g. ?, !, åÅäÄöÖ),
/// // and even spaces.
/// slw::internal::path to { "person.birth_town.population" };
/// for (std::string part : to)
///     std::cout << part << std::endl;
/// // Output:
/// // person
/// // birth_town
/// // population
/// \endcode
///
/// \see slw::internal::touch_path
///

#endif//GET_FIELD_HPP
