#pragma once

namespace ape
{

enum class KeyModifier
{

    none = 0x0000,

    shift = 0x0001,
    
    control = 0x0002,
    
    alt = 0x0004,
    
    super = 0x0008,
    
    capsLock = 0x0010,
    
    numLock = 0x0020

};

inline auto operator & (KeyModifier const lhs, KeyModifier const rhs)
    -> KeyModifier
{
    return static_cast<KeyModifier>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline auto operator | (KeyModifier const lhs, KeyModifier const rhs)
    -> KeyModifier
{
    return static_cast<KeyModifier>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline auto hasModifier(KeyModifier const combination, KeyModifier const modifier)
{
    return ((combination & modifier) == modifier);
}

} // namespace ape
