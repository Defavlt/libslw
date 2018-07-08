#include "slw/utility.hpp"
#include <string>

void slw::debug::print_stack_contents(slw::State &state, std::ostream &cout)
{
    int size = -state.top();

    for (; size; ++size)
    {
        int index = size;
        int type = state.type(index);

        cout << "[" << state.type_name(type) << "] ";
        cout << std::to_string(size) << ":";

        switch(type)
        {
        case slw::TNUMBER:
            {
                slw::number_t _num = 0;
                state.peek(_num, index);
                cout << std::to_string(_num) << std::endl;
                break;
            }
        case slw::TBOOLEAN:
            {
                bool _bool;
                state.peek(_bool, index);
                cout << std::to_string(_bool) << std::endl;
                break;
            }

        default:
        case slw::TSTRING:
            {
                slw::string_t _str;
                state.peek(_str, index);
                cout << _str << std::endl;
                break;
            }
        }
    }
}
