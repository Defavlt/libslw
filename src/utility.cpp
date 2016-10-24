#include "slw/utility.h"
#include <string>

void slw::debug::print_stack_contents(slw::State &state, std::ostream &cout)
{
    int size = state.top();

    for (; size > 0; --size)
    {
        int top = -1;
        int type = state.type(top);

        cout << "[" << state.type_name(type) << "] ";
        cout << std::to_string(size) << ":";

        switch(type)
        {
        case slw::TNUMBER:
            {
                slw::number_t _num = 0;
                state.pop(_num);
                cout << std::to_string(_num) << std::endl;
                break;
            }
        case slw::TBOOLEAN:
            {
                bool _bool;
                state.pop(_bool);
                cout << std::to_string(_bool) << std::endl;
                break;
            }
        case slw::TSTRING:
            {
                slw::string_t _str;
                state.pop(_str);
                cout << _str << std::endl;
                break;
            }
        default:
            break;
        }
    }
}
