#pragma once

#include <gmock/gmock.h>
#include "SMSDB/ISMSDatabase.hpp"

namespace ue {

class ISMSDatabaseMock : public ISMSDatabase{
public:
    ISMSDatabaseMock();
    ~ISMSDatabaseMock();

    MOCK_METHOD(void, addSMS,(common::PhoneNumber,common::PhoneNumber,std::string),(final));
    MOCK_METHOD(ITextMessage&, getSMS,(unsigned int),(final));
    MOCK_METHOD((std::vector<std::pair<unsigned int, std::unique_ptr<ITextMessage>>>&), getAllSMS,(),(final));
};

}