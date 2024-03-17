#pragma once

class Hasher final
{
public:
    Hasher() = delete;
    ~Hasher() = delete;
    Hasher(const Hasher& rhs) = delete;
    Hasher& operator=(const Hasher& rhs) = delete;

    static constexpr uint32_t Hash(const char* str);

private:
    static constexpr uint32_t djb2Hash(const char* str, uint32_t prevHash);
};