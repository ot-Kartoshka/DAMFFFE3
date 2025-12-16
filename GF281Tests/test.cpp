#include "pch.h"
#include "gtest/gtest.h"
#include "../LAb3/GF281.h" 

class GF281Test : public ::testing::Test {
protected:
    GF281 a, b, c, zero, one;

    void SetUp() override {

        a = GF281("101100111000111100001010101000000000000000000000000000000000000000000005555555555");
        b = GF281("11001100110011001100110000000000000000000000000000000000000000000000000003F8000FF");
        c = GF281("11101010101010111111111100000000001");

        zero = GF281::Zero();
        one = GF281::One();
    }
};


TEST_F(GF281Test, ZeroIsZero) {
    EXPECT_EQ(zero.toBinString(), "0");
    EXPECT_EQ(zero.deg(), -1);
}

TEST_F(GF281Test, OneIsOne) {
    EXPECT_EQ(one.toBinString(), "1");
    EXPECT_EQ(one.deg(), 0);
}

TEST_F(GF281Test, StringConversionConsistency) {
    std::string input = "101010101010101";
    GF281 val(input);

    GF281 val2(val.toBinString());

    EXPECT_EQ(val, val2);
}
TEST_F(GF281Test, AddCommutativity) {
    EXPECT_EQ(a + b, b + a);
}

TEST_F(GF281Test, AddAssociativity) {
    EXPECT_EQ((a + b) + c, a + (b + c));
}

TEST_F(GF281Test, AddIdentity) {
    EXPECT_EQ(a + zero, a);
    EXPECT_EQ(zero + a, a);
}

TEST_F(GF281Test, AddInverse) {
    EXPECT_EQ(a + a, zero);
}

TEST_F(GF281Test, MulCommutativity) {
    EXPECT_EQ(a * b, b * a);
}

TEST_F(GF281Test, MulAssociativity) {
    EXPECT_EQ((a * b) * c, a * (b * c));
}

TEST_F(GF281Test, MulIdentity) {
    EXPECT_EQ(a * one, a);
    EXPECT_EQ(one * a, a);
}

TEST_F(GF281Test, MulZero) {
    EXPECT_EQ(a * zero, zero);
    EXPECT_EQ(zero * a, zero);
}

TEST_F(GF281Test, Distributivity) {
    GF281 left = (a + b) * c;
    GF281 right = (a * c) + (b * c);
    EXPECT_EQ(left, right) << "Distributivity check failed";
}
TEST_F(GF281Test, SquareLinearity) {
    EXPECT_EQ((a + b).square(), a.square() + b.square());
}

TEST_F(GF281Test, SquareConsistency) {
    EXPECT_EQ(a.square(), a * a);
}

TEST_F(GF281Test, SquareOneAndZero) {
    EXPECT_EQ(one.square(), one);
    EXPECT_EQ(zero.square(), zero);
}

TEST_F(GF281Test, PowerZero) {
    EXPECT_EQ(a.power(0), one);
}

TEST_F(GF281Test, PowerOne) {
    EXPECT_EQ(a.power(1), a);
}

TEST_F(GF281Test, PowerTwoMatchesSquare) {
    EXPECT_EQ(a.power(2), a.square());
}

TEST_F(GF281Test, PowerAdditionRule) {
    GF281 p2 = a.power(2);
    GF281 p3 = a.power(3);
    EXPECT_EQ(a.power(5), p2 * p3);
}

TEST_F(GF281Test, InverseMultiplication) {
    GF281 inv = a.inverse();
    GF281 res = a * inv;

    EXPECT_EQ(res.toHexString(), one.toHexString()) << "Inverse check failed for 'a'";

    GF281 invB = b.inverse();
    EXPECT_EQ((b * invB).toHexString(), one.toHexString()) << "Inverse check failed for 'b'";
}

TEST_F(GF281Test, InverseOfOne) {
    EXPECT_EQ(one.inverse(), one);
}

TEST_F(GF281Test, DoubleInverse) {
    EXPECT_EQ(a.inverse().inverse(), a);
}

TEST_F(GF281Test, TraceLinearity) {
    int trA = a.trace();
    int trB = b.trace();
    int trSum = (a + b).trace();

    EXPECT_EQ(trSum, trA ^ trB);
}

TEST_F(GF281Test, TraceFrobenius) {
    EXPECT_EQ(a.square().trace(), a.trace());
}

TEST_F(GF281Test, TraceValues) {
    int tr = c.trace();
    EXPECT_TRUE(tr == 0 || tr == 1);
}

TEST_F(GF281Test, MaskingEffectiveness) {

    GF281 res = a * b;

    EXPECT_LT(res.deg(), 281);

    for (int i = 281; i < 320; ++i) {
        EXPECT_EQ(res.getBit(i), 0) << "Found garbage bit at index " << i;
    }
}