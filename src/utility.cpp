#include "slw/utility.h"

void slw::debug::print_stack_contents(slw::State &state, std::ostream &cout)
{
    int size = state.top();

    for (; size > 0; --size)
    {
        int top = -1;
        int type = state.type(top);

        std::cout << "[" << state.type_name(type) << "] ";
        std::cout << std::to_string(size) << ":";

        switch(type)
        {
        case slw::TNUMBER:
            {
                slw::number_t _num = 0;
                state.pop(_num);
                std::cout << std::to_string(_num) << std::endl;
                break;
            }
        case slw::TBOOLEAN:
            {
                bool _bool;
                state.pop(_bool);
                std::cout << std::to_string(_bool) << std::endl;
                break;
            }
        case slw::TSTRING:
            {
                slw::string_t _str;
                state.pop(_str);
                std::cout << _str << std::endl;
                break;
            }
        default:
            break;
        }
    }
}
