#ifndef STACKLANG_LOCATION_HPP
#define STACKLANG_LOCATION_HPP

#include <cctype>

namespace stacklang::lexer
{
    struct Location
    {
        std::size_t m_begin, m_end, m_line;
    };
}

#endif //STACKLANG_LOCATION_HPP
