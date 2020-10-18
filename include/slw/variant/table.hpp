#ifndef SLW_VARIANT_TABLE_HPP
#define SLW_VARIANT_TABLE_HPP

#include "slw/variant/variant.hpp"
#include "slw/variant/number.hpp"
#include "slw/variant/int.hpp"
#include "slw/variant/uint.hpp"
#include "slw/variant/string.hpp"
#include "slw/variant/bool.hpp"

namespace slw {

template<>
struct variant<slw::table_t> {
    typedef slw::table_t expected_value_type;
    static constexpr type_e internal_value_type = TTABLE;

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::variant
    ////////////////////////////////////////////////////////////
    explicit variant(slw::shared_state state);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::variant
    ////////////////////////////////////////////////////////////
    variant(slw::shared_state state, const slw::string_t &name);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::variant
    ////////////////////////////////////////////////////////////
    variant(slw::shared_state state, const slw::reference &ref);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::variant
    ////////////////////////////////////////////////////////////
    variant(shared_state state, slw::int_t idx);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::variant
    ////////////////////////////////////////////////////////////
    variant(const slw::reference &);

    template<typename Tp>
    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator =
    ////////////////////////////////////////////////////////////
    variant<Tp> operator =(const Tp &value)
    {
        slw::reference ref = M_ref;
        slw::variant<Tp> var { M_state, ref };
        return var;
    }

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator =
    ////////////////////////////////////////////////////////////
    variant<slw::table_t> &operator =(variant<slw::table_t> &);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator ==
    ////////////////////////////////////////////////////////////
    bool operator ==(variant<slw::table_t> &);
    bool operator ==(variant<slw::table_t> &&);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator !=
    ////////////////////////////////////////////////////////////
    bool operator !=(variant<slw::table_t> &);
    bool operator !=(variant<slw::table_t> &&);

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::get
    ////////////////////////////////////////////////////////////
    slw::reference get();

    ////////////////////////////////////////////////////////////
    /// \brief Assign this[k] the value at index idx.
    /// \param k the index in this table
    /// \param idx the index at which we find value we're assigning
    ////////////////////////////////////////////////////////////
    slw::reference set(const slw::string_t &k, slw::int_t idx);

    ////////////////////////////////////////////////////////////
    /// \brief Assign this[k] the provided value.
    /// \param k the key in this table
    /// \param v the value we're looking to assign to the key
    /// \tparam Tp The value of the variant type
    ////////////////////////////////////////////////////////////
    reference set(const slw::string_t &k, reference &v)
    { reference r = M_ref[k];
        v.push();
        r.assign();
        return r;
    }

    reference set(const slw::string_t &k, reference v)
    { reference r = M_ref[k];
        v.push();
        r.assign();
        return r;
    }

    template<typename Tp>
    variant<Tp> get(const slw::string_t &k)
    { return variant<Tp> { M_state, M_ref[k] };
    }

    template<typename Tp>
    variant<Tp> get(slw::int_t i)
    { return variant<Tp> { M_state, M_ref[i] };
    }

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator []
    ////////////////////////////////////////////////////////////
    slw::reference operator [](const slw::string_t &k)
    { return M_ref[k];
    }

    ////////////////////////////////////////////////////////////
    /// \see slw::variant::operator []
    ////////////////////////////////////////////////////////////
    slw::reference operator [](slw::int_t k)
    { return M_ref[k];
    }

private:
    slw::shared_state M_state;
    slw::reference M_ref;
};

typedef variant<slw::table_t> table;

} //namespace slw

#endif //SLW_VARIANT_TABLE_HPP
