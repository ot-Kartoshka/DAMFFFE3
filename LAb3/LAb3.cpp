#include <iostream>
#include <chrono>
#include <vector>
#include "GF281.h"

template<typename Func>
long long measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int main() {
    std::cout << "Lab3. V8. GF(2^281).\n";
    std::cout << "Polynomial: x^281 + x^9 + x^4 + x + 1\n\n";


    GF281 a("101010101010101010101010101010101010101");
    GF281 b("111111100000000000000011111111");
    GF281 c("1000000000000000001");

    std::cout << "a = " << a.toHexString() << "\n";
    std::cout << "b = " << b.toHexString() << "\n\n";

    // Перевірка операцій
    std::cout << "Correct Tests\n";

    // Дистрибутивність
    GF281 res1 = (a + b) * c;
    GF281 res2 = (a * c) + (b * c);

    if (res1 == res2) std::cout << "[PASS] Distributivity: (a+b)*c = ac + bc\n";
    else std::cout << "[FAIL] Distributivity\n";

    // Обернений елемент
    GF281 d("1011");
    GF281 inv = d.inverse();
    GF281 unit = d * inv;

    if (unit == GF281::One()) std::cout << "[PASS] Inverse: a * a^-1 = 1\n";
    else {
        std::cout << "[FAIL] Inverse. Result: " << unit.toHexString() << "\n";
    }

    // Квадрат
    if ((a + b).square() == (a.square() + b.square())) {
        std::cout << "[PASS] Square linearity: (a+b)^2 = a^2 + b^2\n";
    }

    std::cout << "\nPerformance Tests (Average of 1000 runs)\n";
    // Заміри часу
    long long totalAdd = 0, totalMul = 0, totalSq = 0;
    int runs = 1000;

    for (int i = 0; i < runs; ++i) {
        totalAdd += measureTime([&]() { volatile GF281 r = a + b; });
        totalMul += measureTime([&]() { volatile GF281 r = a * b; });
        totalSq += measureTime([&]() { volatile GF281 r = a.square(); });
    }

    std::cout << "Add: " << totalAdd / runs << " ns\n";
    std::cout << "Mul: " << totalMul / runs << " ns\n";
    std::cout << "Sqr: " << totalSq / runs << " ns\n";

    return 0;
}