#ifndef ENUM_TOINT_INCLUDE
#define ENUM_TOINT_INCLUDE
#include <iostream>

template <typename Enumeration>
auto toInt(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
#endif
