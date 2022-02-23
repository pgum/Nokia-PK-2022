#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Config/MultiLineConfig.hpp"
#include <memory>
#include <sstream>
#include <initializer_list>

namespace common
{

using namespace ::testing;

class MultiLineConfigTestSuite : public Test
{
protected:
    std::unique_ptr<MultiLineConfig> objectUnderTest;
    void makeObjectUnderTest(const std::string& text)
    {
        std::istringstream is(text);
        objectUnderTest = std::make_unique<MultiLineConfig>(is);
    }
    void makeObjectUnderTest(int argc, char* argv[])
    {
        objectUnderTest = std::make_unique<MultiLineConfig>(argc, argv);
    }
};


TEST_F(MultiLineConfigTestSuite, shallHaveNoValueForEmptyInput)
{
    makeObjectUnderTest("");
    ASSERT_THROW(objectUnderTest->getString("key"), std::invalid_argument);

}

TEST_F(MultiLineConfigTestSuite, shallHaveNoValueForCommentedOutKey)
{
    makeObjectUnderTest("#key = value");
    ASSERT_THROW(objectUnderTest->getString("key"), std::invalid_argument);

}

TEST_F(MultiLineConfigTestSuite, shallHaveNoValueForAbsentKey)
{
    makeObjectUnderTest("key2=value2");
    ASSERT_THROW(objectUnderTest->getString("key1"), std::invalid_argument);

}

TEST_F(MultiLineConfigTestSuite, shallUseDefaultStringForAbsentKey)
{
    makeObjectUnderTest("key2=value2");
    const std::string defaultValue1 = "value1";
    std::string value1;
    ASSERT_NO_THROW(value1 = objectUnderTest->getString("key1", defaultValue1));
    ASSERT_EQ(defaultValue1, value1);
}

TEST_F(MultiLineConfigTestSuite, shallNotUseDefaultStringForPresentKey)
{
    makeObjectUnderTest("key1=value1");
    const std::string defaultValue1 = "not_value1";
    std::string value1;
    ASSERT_NO_THROW(value1 = objectUnderTest->getString("key1", defaultValue1));
    ASSERT_EQ("value1", value1);
}

TEST_F(MultiLineConfigTestSuite, shallUseDefaultNumberForAbsentKey)
{
    makeObjectUnderTest("key2=345");
    const int defaultValue1 = 123;
    int value1;
    ASSERT_NO_THROW(value1 = objectUnderTest->getNumber("key1", defaultValue1));
    ASSERT_EQ(defaultValue1, value1);
}

TEST_F(MultiLineConfigTestSuite, shallNotUseDefaultNumberForPresentKey)
{
    makeObjectUnderTest("key1=345");
    const int defaultValue1 = 123;
    int value1;
    ASSERT_NO_THROW(value1 = objectUnderTest->getNumber("key1", defaultValue1));
    ASSERT_EQ(345, value1);
}

TEST_F(MultiLineConfigTestSuite, shallFailToReadWronglyFormattedNumber)
{
    makeObjectUnderTest("key=notnumber");
    ASSERT_THROW(objectUnderTest->getNumber<int>("key1"), std::invalid_argument);

}

TEST_F(MultiLineConfigTestSuite, shallReadPresentKey)
{
    makeObjectUnderTest("key=value");
    std::string value;
    ASSERT_NO_THROW(value = objectUnderTest->getString("key"));
    ASSERT_EQ("value", value);
}

TEST_F(MultiLineConfigTestSuite, shallReadPresentKeyWithoutTrailingWhitespacesAndComment)
{
    makeObjectUnderTest("  \t key = \t  value  #this is not importat \t");
    std::string value;
    ASSERT_NO_THROW(value = objectUnderTest->getString("key"));
    ASSERT_EQ("value", value);

}

TEST_F(MultiLineConfigTestSuite, shallReadIntValue)
{
    makeObjectUnderTest(" key = 123 ");
    int value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<int>("key"));
    ASSERT_EQ(123, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadUnsignedIntValue)
{
    makeObjectUnderTest(" key = 123 ");
    unsigned int value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<unsigned int>("key"));
    ASSERT_EQ(123u, value);
}

TEST_F(MultiLineConfigTestSuite, shallFailToReadSignedAsUnsignedValue)
{
    makeObjectUnderTest(" key = -123 ");
    unsigned int value;
    ASSERT_THROW(objectUnderTest->getNumber<unsigned int>("key"), std::out_of_range);
}

TEST_F(MultiLineConfigTestSuite, shallReadHexValue)
{
    makeObjectUnderTest(" key = 0x123 ");
    unsigned int value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<unsigned int>("key"));
    ASSERT_EQ(0x123, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadLongValue)
{
    makeObjectUnderTest(" key = -123456789 ");
    long value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<long>("key"));
    ASSERT_EQ(-123456789l, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadUnsignedLongValue)
{
    makeObjectUnderTest(" key = 123456789 ");
    unsigned long value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<unsigned long>("key"));
    ASSERT_EQ(123456789ul, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadLongLongValue)
{
    makeObjectUnderTest(" key = -123456789123456789 ");
    long long value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<long long>("key"));
    ASSERT_EQ(-123456789123456789ll, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadUnsignedLongLongValue)
{
    makeObjectUnderTest(" key = 123456789123456789 ");
    unsigned long long value;
    ASSERT_NO_THROW(value = objectUnderTest->getNumber<unsigned long long>("key"));
    ASSERT_EQ(123456789123456789ull, value);
}

TEST_F(MultiLineConfigTestSuite, shallReadLongMultiLineConfig)
{
    makeObjectUnderTest(
        " ## This is example file\n"
        " key1 = 123456789123456789 \n"
        "\n\n"
        "                 # Some section\n"
        " key2 = Something stupid \n"
        " key3 = Something stupid 2"
    );
    unsigned long long value1;
    std::string value2;
    std::string value3;
    ASSERT_NO_THROW(value2 = objectUnderTest->getString("key2"));
    ASSERT_NO_THROW(value3 = objectUnderTest->getString("key3"));
    ASSERT_NO_THROW(value1 = objectUnderTest->getNumber<unsigned long long>("key1"));

    ASSERT_EQ(123456789123456789ull, value1);
    ASSERT_EQ("Something stupid", value2);
    ASSERT_EQ("Something stupid 2", value3);
}

TEST_F(MultiLineConfigTestSuite, shallReadLongMultiLineConfigFromMainArgs)
{
    char* argv[] = {
        (char*)"key1",
        (char*)"=",
        (char*)"123456789123456789",
        (char*)"key2=",
        (char*)"Something stupid",
        (char*)"key3",
        (char*)"=Something stupid 2"
    };
    makeObjectUnderTest(sizeof(argv)/sizeof(argv[0]), argv);
    unsigned long long value1;
    std::string value2;
    std::string value3;
    ASSERT_NO_THROW(value2 = objectUnderTest->getString("key2"));
    ASSERT_NO_THROW(value3 = objectUnderTest->getString("key3"));
    ASSERT_NO_THROW(value1 = objectUnderTest->getNumber<unsigned long long>("key1"));

    ASSERT_EQ(123456789123456789ull, value1);
    ASSERT_EQ("Something stupid", value2);
    ASSERT_EQ("Something stupid 2", value3);
}


TEST_F(MultiLineConfigTestSuite, shallInsertNotPresentKeysFromOtherConfig)
{
    makeObjectUnderTest(
        " ## This is example file\n"
        " key1 = 0xDEAD \n"
        "\n\n"
        "                 # Some section\n"
        " key2 = Value not overwritten \n"
        " key3 = Overwritten value"
    );
    auto firstConfig = *objectUnderTest;
    makeObjectUnderTest(
        " ## This is example file\n"
        " key1 = 0xBEAF \n"
        "\n\n"
        "                 # Some section\n"
        " key2 = Original value \n"
    );
    objectUnderTest->insertFrom(firstConfig);

    unsigned value1;
    std::string value2;
    std::string value3;
    ASSERT_NO_THROW(value2 = objectUnderTest->getString("key2"));
    ASSERT_NO_THROW(value3 = objectUnderTest->getString("key3"));
    ASSERT_NO_THROW(value1 = objectUnderTest->getNumber<unsigned>("key1"));

    ASSERT_EQ(0xBEAF, value1);
    ASSERT_EQ("Original value", value2);
    ASSERT_EQ("Overwritten value", value3);
}

TEST_F(MultiLineConfigTestSuite, shallNotThrowExceptionWhileParsingEOF)
{
    std::string input = "key1 = value1\n";
    input += std::char_traits<char>::eof();

    makeObjectUnderTest(input);

    std::string value1;
    ASSERT_NO_THROW(value1 = objectUnderTest->getString("key1"));

    ASSERT_EQ("value1", value1);
}



} // namespace common
