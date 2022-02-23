#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>


#include "Messages/OutgoingMessage.hpp"

using namespace ::testing;

namespace common
{

class OutgoingMessageTestSuite : public TestWithParam<MessageHeader>
{
protected:
    OutgoingMessageTestSuite()
        : objectUnderTest(GetParam().messageId, GetParam().from, GetParam().to)
    {}

    OutgoingMessage objectUnderTest;
    BinaryMessage messageToSend;

    void getMessage()
    {
        messageToSend = objectUnderTest.getMessage();
    }

    template <typename T>
    PhoneNumber phone(T value)
    {
        return PhoneNumber{ static_cast<decltype(PhoneNumber::value)>(value) };
    }

    void assertHeader()
    {
        ASSERT_EQ(get(GetParam().messageId), messageToSend.value[0]);
        ASSERT_EQ(GetParam().from, phone(messageToSend.value[1]));
        ASSERT_EQ(GetParam().to, phone(messageToSend.value[2]));
    }
};

TEST_P(OutgoingMessageTestSuite, shallEncodeMessageHeader)
{
    getMessage();
    assertHeader();
}

TEST_P(OutgoingMessageTestSuite, shallEncodeMessageHeaderAndString)
{
    std::string text = "Something stupid";
    objectUnderTest.writeText(text);

    getMessage();
    assertHeader();

    ASSERT_THAT(text, ElementsAreArray(messageToSend.value.data() + sizeof(MessageHeader),
                                       messageToSend.value.size() - sizeof(MessageHeader)));
}

TEST_P(OutgoingMessageTestSuite, shallEncodeMessageHeaderAndOneByte)
{
    std::uint8_t number = 0x12;
    objectUnderTest.writeNumber(number);

    getMessage();
    assertHeader();

    ASSERT_EQ(sizeof(MessageHeader) + 1u, messageToSend.value.size());
    ASSERT_EQ(number, messageToSend.value[sizeof(MessageHeader)]);
}

TEST_P(OutgoingMessageTestSuite, shallEncodeMessageHeaderAndTwoByteNumber)
{
    std::uint16_t number = 0x1234;
    std::uint8_t highByte = 0x12;
    std::uint8_t lowByte = 0x34;
    objectUnderTest.writeNumber(number);

    getMessage();
    assertHeader();

    ASSERT_EQ(sizeof(MessageHeader) + 2u, messageToSend.value.size());
    ASSERT_EQ(highByte, messageToSend.value[sizeof(MessageHeader)]);
    ASSERT_EQ(lowByte, messageToSend.value[sizeof(MessageHeader) + 1]);
}

TEST_P(OutgoingMessageTestSuite, shallEncodeMessageHeaderAndTwoNumbersAndString)
{
    std::uint16_t number1 = 0x1234;
    std::uint8_t highByte = 0x12;
    std::uint8_t lowByte = 0x34;
    std::uint8_t number2 = 0x56;
    std::string text = "Something stupid";

    objectUnderTest.writeNumber(number1);
    objectUnderTest.writeNumber(number2);
    objectUnderTest.writeText(text);

    getMessage();
    assertHeader();

    ASSERT_EQ(sizeof(MessageHeader) + 3u + text.length(), messageToSend.value.size());
    ASSERT_EQ(highByte, messageToSend.value[sizeof(MessageHeader)]);
    ASSERT_EQ(lowByte, messageToSend.value[sizeof(MessageHeader) + 1]);
    ASSERT_EQ(number2, messageToSend.value[sizeof(MessageHeader) + 2]);

    ASSERT_THAT(text, ElementsAreArray(messageToSend.value.data() + sizeof(MessageHeader) + 3u,
                                       messageToSend.value.size() - sizeof(MessageHeader) - 3u));
}

INSTANTIATE_TEST_SUITE_P(
        DifferentHeaders,
        OutgoingMessageTestSuite,
        Values(
            MessageHeader{MessageId::AttachRequest, 0x02, 0x03},
            MessageHeader{MessageId::AttachResponse, 0x13, 0xFF},
            MessageHeader{MessageId::AttachRequest, 0xFF, 0xDD}
            ));

}
