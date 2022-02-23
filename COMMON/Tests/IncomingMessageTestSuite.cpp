#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <algorithm>
#include <memory>
#include <iterator>

#include "Messages/IncomingMessage.hpp"

using namespace ::testing;

namespace common
{

class IncomingMessageTestSuite : public Test
{
protected:
    const MessageHeader messageHeader{MessageId::CallDropped, PhoneNumber{0x12}, PhoneNumber{0x34}};
    const std::string text = "Something stupid";
    const std::uint8_t oneByte = 0x78;
    const std::uint32_t number = 0x11223344;
    const std::uint8_t numberByte1 = 0x11;
    const std::uint8_t numberByte2 = 0x22;
    const std::uint8_t numberByte3 = 0x33;
    const std::uint8_t numberByte4 = 0x44;

    using Input = BinaryMessage;

    std::unique_ptr<IncomingMessage> objectUnderTest;
    void makeObjectUnderTest(const Input& input)
    {
        objectUnderTest = std::make_unique<IncomingMessage>(input);
    }
    void assertHeader()
    {
        const MessageHeader actualHeader = objectUnderTest->readMessageHeader();
        ASSERT_EQ(messageHeader.messageId, actualHeader.messageId);
        ASSERT_EQ(messageHeader.from, actualHeader.from);
        ASSERT_EQ(messageHeader.to, actualHeader.to);
    }
    Input createInputForHeader(const MessageHeader& messageHeader);
};

TEST_F(IncomingMessageTestSuite, shallAcceptEmptyInputButHeaderWontBePresent)
{
    Input emptyInput;
    makeObjectUnderTest(emptyInput);
    ASSERT_THROW(assertHeader(), IncomingMessage::ReadEx);
}

TEST_F(IncomingMessageTestSuite, shallAcceptLessThanHeaderButHeaderWontBePresent)
{
    Input lessThanHeader{Input::Value(sizeof(MessageHeader) - 1)};
    makeObjectUnderTest(lessThanHeader);
    ASSERT_THROW(assertHeader(), IncomingMessage::ReadEx);
}

IncomingMessageTestSuite::Input IncomingMessageTestSuite::createInputForHeader(const MessageHeader& messageHeader)
{
    static_assert(sizeof(PhoneNumber::value) == 1,
                  "You need to redefine this test");
    static_assert(sizeof(MessageId) == 1,
                  "You need to redefine this test");
    return { { get(messageHeader.messageId), messageHeader.from.value, messageHeader.to.value } };
}



TEST_F(IncomingMessageTestSuite, shallReadHeaderOnly)
{
    Input headerOnly = createInputForHeader(messageHeader);
    ASSERT_NO_THROW(makeObjectUnderTest(headerOnly));

    assertHeader();
    ASSERT_NO_THROW(objectUnderTest->checkEndOfMessage());
}

TEST_F(IncomingMessageTestSuite, shallReadHeaderAndString)
{
    Input input = createInputForHeader(messageHeader);
    std::copy(text.begin(), text.end(), std::back_inserter(input.value));
    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const std::string actualText = objectUnderTest->readText(text.length());
    ASSERT_NO_THROW(objectUnderTest->checkEndOfMessage());

    ASSERT_EQ(text, actualText);
}

TEST_F(IncomingMessageTestSuite, shallReadHeaderAndByte)
{
    Input input = createInputForHeader(messageHeader);
    input.value.push_back(oneByte);
    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const auto actualByte = objectUnderTest->readNumber<std::uint8_t>();
    ASSERT_NO_THROW(objectUnderTest->checkEndOfMessage());

    ASSERT_EQ(oneByte, actualByte);
}

TEST_F(IncomingMessageTestSuite, shallReadHeaderAndNumber)
{
    Input input = createInputForHeader(messageHeader);
    Input::Value::value_type numbers[] = { numberByte1, numberByte2, numberByte3, numberByte4 };
    std::copy(std::begin(numbers), std::end(numbers), std::back_inserter(input.value));

    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const auto actualNumber = objectUnderTest->readNumber<std::uint32_t>();
    ASSERT_NO_THROW(objectUnderTest->checkEndOfMessage());

    ASSERT_EQ(number, actualNumber);
}

TEST_F(IncomingMessageTestSuite, shallReadHeaderAndByteNumberString)
{
    Input input = createInputForHeader(messageHeader);
    Input::Value::value_type numbers[] = { oneByte, numberByte1, numberByte2, numberByte3, numberByte4 };
    std::copy(std::begin(numbers), std::end(numbers), std::back_inserter(input.value));
    std::copy(text.begin(), text.end(), std::back_inserter(input.value));

    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const auto actualByte = objectUnderTest->readNumber<std::uint8_t>();
    const auto actualNumber = objectUnderTest->readNumber<std::uint32_t>();
    const std::string actualText = objectUnderTest->readText(text.length());
    ASSERT_NO_THROW(objectUnderTest->checkEndOfMessage());

    ASSERT_EQ(oneByte, actualByte);
    ASSERT_EQ(number, actualNumber);
    ASSERT_EQ(text, actualText);
}

TEST_F(IncomingMessageTestSuite, shallFailWhenNotReadFullText)
{
    Input input = createInputForHeader(messageHeader);
    Input::Value::value_type numbers[] = { oneByte, numberByte1, numberByte2, numberByte3, numberByte4 };
    std::copy(std::begin(numbers), std::end(numbers), std::back_inserter(input.value));
    std::copy(text.begin(), text.end(), std::back_inserter(input.value));

    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const auto actualByte = objectUnderTest->readNumber<std::uint8_t>();
    const auto actualNumber = objectUnderTest->readNumber<std::uint32_t>();
    const std::string actualText = objectUnderTest->readText(text.length() - 1);
    ASSERT_THROW(objectUnderTest->checkEndOfMessage(), IncomingMessage::ReadEx);

}

TEST_F(IncomingMessageTestSuite, shallFailWhenNotReadAllFields)
{
    Input input = createInputForHeader(messageHeader);
    Input::Value::value_type numbers[] = { oneByte, numberByte1, numberByte2, numberByte3, numberByte4 };
    std::copy(std::begin(numbers), std::end(numbers), std::back_inserter(input.value));
    std::copy(text.begin(), text.end(), std::back_inserter(input.value));

    ASSERT_NO_THROW(makeObjectUnderTest(input));

    assertHeader();
    const auto actualByte = objectUnderTest->readNumber<std::uint8_t>();
    ASSERT_THROW(objectUnderTest->checkEndOfMessage(), IncomingMessage::ReadEx);

}

}
