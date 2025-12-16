#include "GF281.h"
#include <sstream>
#include <iomanip> 

GF281::GF281() {
    for (int i = 0; i < WORDS_COUNT; i++) data[i] = 0;
}

void GF281::setBit(int index) {
    if (index < 0) return;
    int wordIndex = index / 64;
    int bitIndex = index % 64;
    if (wordIndex < WORDS_COUNT) {
        data[wordIndex] |= (1ULL << bitIndex);
    }
}

int GF281::getBit(int index) const {
    if (index < 0) return 0;
    int wordIndex = index / 64;
    int bitIndex = index % 64;
    if (wordIndex < WORDS_COUNT) {
        return (data[wordIndex] >> bitIndex) & 1;
    }
    return 0;
}

int GF281::deg() const {
    for (int i = M - 1; i >= 0; i--) {
        if (getBit(i)) return i;
    }
    return -1;
}

GF281 GF281::Zero() { return GF281(); }

GF281 GF281::One() {
    GF281 one;
    one.setBit(0);
    return one;
}

// Для відсікання зайвих бітів
void GF281::mask() {
    if (WORDS_COUNT > 0) {

        data[WORDS_COUNT - 1] &= ((1ULL << 25) - 1);
    }
}

// Додавання 
GF281 GF281::operator+(const GF281& other) const {
    GF281 res;
    for (int i = 0; i < WORDS_COUNT; i++) {
        res.data[i] = this->data[i] ^ other.data[i];
    }
    res.mask();
    return res;
}

// Множення
GF281 GF281::operator*(const GF281& other) const {
    GF281 a = *this;
    GF281 b = other;
    GF281 p;

    for (int i = M - 1; i >= 0; i--) {
        int highestBit = p.getBit(M - 1);

        for (int w = WORDS_COUNT - 1; w >= 0; w--) {
            p.data[w] <<= 1;
            if (w > 0 && (p.data[w - 1] & (1ULL << 63))) {
                p.data[w] |= 1;
            }
        }

        if (highestBit) {
            p.data[0] ^= ((1ULL << 9) | (1ULL << 4) | (1ULL << 1) | 1ULL);
        }

        if (b.getBit(i)) {
            p = p + a;
        }
    }

    p.mask();
    return p;
}

// Квадрат
GF281 GF281::square() const {

    uint64_t temp[10] = { 0 };

    auto setTempBit = [&](int idx) {
        temp[idx / 64] |= (1ULL << (idx % 64));
        };

    auto xorTempBit = [&](int idx) {
        temp[idx / 64] ^= (1ULL << (idx % 64));
        };

    auto getTempBit = [&](int idx) -> int {
        return (temp[idx / 64] >> (idx % 64)) & 1;
        };

    for (int i = 0; i < M; i++) {
        if (getBit(i)) {
            setTempBit(2 * i);
        }
    }

    for (int i = 2 * (M - 1); i >= M; i--) {
        if (getTempBit(i)) {
            int shift = i - M;

            xorTempBit(shift + 9);
            xorTempBit(shift + 4);
            xorTempBit(shift + 1);
            xorTempBit(shift + 0);
        }
    }

    GF281 res;
    for (int i = 0; i < WORDS_COUNT; i++) {
        res.data[i] = temp[i];
    }

    for (int i = 0; i < WORDS_COUNT; ++i) {
        res.data[i] = temp[i];
    }

    res.mask(); 
    return res;
}

// Піднесення до степеня
GF281 GF281::power(uint64_t exp) const {
    GF281 res = GF281::One();
    GF281 base = *this;
    while (exp > 0) {
        if (exp & 1) res = res * base;
        base = base.square();
        exp >>= 1;
    }
    return res;
}

// Trace
int GF281::trace() const {

    GF281 sum = *this;
    GF281 sq = *this;

    for (int i = 1; i < M; i++) {
        sq = sq.square();
        sum = sum + sq;
    }

    return sum.getBit(0);
}

// Обернений елемент
GF281 GF281::inverse() const {

    GF281 base = *this;
    GF281 accumulated = GF281::One();

    base = base.square();

    for (int i = 1; i < M; i++) {
        accumulated = accumulated * base;
        base = base.square();
    }

    return accumulated;
}

// Вивід в рядок
std::string GF281::toBinString() const {
    std::string s = "";
    bool leadingZeros = true;

    for (int i = M - 1; i >= 0; i--) {
        int bit = getBit(i);

        if (bit != 0) leadingZeros = false;

        if (!leadingZeros) {
            s += (bit ? "1" : "0");
        }
    }

    if (s.empty()) return "0";

    return s;
}

// Вивід в Hex
std::string GF281::toHexString() const {
    std::stringstream ss;
    ss << std::hex << std::uppercase;
    for (int i = WORDS_COUNT - 1; i >= 0; i--) {
        ss << std::setw(16) << std::setfill('0') << data[i];
    }
    return ss.str();
}

bool GF281::operator==(const GF281& other) const {
    for (int i = 0; i < WORDS_COUNT; ++i) if (data[i] != other.data[i]) return false;
    return true;
}
bool GF281::operator!=(const GF281& other) const { return !(*this == other); }

GF281::GF281(const std::string& binString) {
    for (int i = 0; i < WORDS_COUNT; i++) data[i] = 0;
    int len = binString.length();
    for (int i = 0; i < len; i++) {
        if (binString[len - 1 - i] == '1') {
            setBit(i);
        }
    }
    mask();
}