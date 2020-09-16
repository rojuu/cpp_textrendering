#include <fstream>
#include <gtest/gtest.h>
#include <Utils.hpp>

TEST(Utils, readEntireTextFile)
{
    std::ofstream file("test.txt");
    file << "test123";
    file.close();

    const auto fc = readEntireTextFile("test.txt");

    EXPECT_FALSE(fc.empty());
    EXPECT_EQ(fc, "test123");
}

TEST(Utils, readEntireBinaryFile)
{
    std::ofstream file("test.bin", std::ios::binary);
    std::vector<uint8_t> testData = {0xff, 0x12, 0x21};
    file << testData[0];
    file << testData[1];
    file << testData[2];
    file.close();

    const auto fc = readEntireBinaryFile("test.bin");

    ASSERT_EQ(fc.size(), 3);
    EXPECT_EQ(fc[0], testData[0]);
    EXPECT_EQ(fc[1], testData[1]);
    EXPECT_EQ(fc[2], testData[2]);
}
