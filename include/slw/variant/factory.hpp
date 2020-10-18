#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "slw/variant/variant.hpp"
#include "slw/variant/int.hpp"
#include "slw/variant/number.hpp"
#include "slw/variant/string.hpp"
#include "slw/variant/table.hpp"
#include "slw/variant/uint.hpp"
#include "slw/variant/bool.hpp"

namespace slw {

template<typename Tp>
slw::variant<Tp> make_variant(slw::shared_state &, const Tp &)
{ throw slw::no_type_specialization();
}

template <> slw::string make_variant<slw::string_t> (slw::shared_state &, const slw::string_t &);
slw::integer    make_variant(slw::shared_state &, slw::int_t);
slw::number     make_variant(slw::shared_state &, slw::number_t);
slw::uinteger   make_variant(slw::shared_state &, slw::uint_t);
slw::table      make_variant(slw::shared_state &);
slw::boolean    make_variant(slw::shared_state &, slw::bool_t);
}

#endif // FACTORY_HPP
