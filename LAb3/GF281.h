#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <iomanip>

const int M = 281;
const int WORDS_COUNT = (M + 63) / 64;

class GF281 {
private:
    uint64_t data[WORDS_COUNT];
    void mask();

public:
    GF281();
    GF281(const std::string& hexString);

    void setBit(int index);
    int getBit(int index) const;
    int deg() const;
    static GF281 One();
    static GF281 Zero();

    GF281 operator+(const GF281& other) const;

    GF281 operator*(const GF281& other) const;

    GF281 square() const;

    GF281 power(const GF281& exp) const;
    GF281 power(uint64_t exp) const;

    GF281 inverse() const;

    int trace() const;

    std::string toBinString() const;
    std::string toHexString() const;

    bool operator==(const GF281& other) const;
    bool operator!=(const GF281& other) const;
};