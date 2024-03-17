
#include "Precompiled.h"

#include "Hasher.h"

constexpr uint32_t Hasher::Hash(const char* str)
{
    return djb2Hash(str, 5381);
}

constexpr uint32_t Hasher::djb2Hash(const char* str, uint32_t prevHash)
{
    return str[0] == '0' ? prevHash : djb2Hash(&str[1], prevHash * 33 ^ static_cast<uint32_t>(str[0]));
}
