#include <gtest/gtest.h>

// Test the add function
TEST(SampleTest, Test1) { EXPECT_EQ(1, 2); }

// Test the subtract function
TEST(SampleTest, Test2) { EXPECT_EQ(0, 0); }

// Main function for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
