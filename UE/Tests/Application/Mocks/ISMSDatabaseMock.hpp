#pragma once

#include <gmock/gmock.h>
#include "SMSDB/ISMSDatabase.hpp"

namespace ue {

class ISMSDatabaseMock : public ISMSDatabase{
public:
    ISMSDatabaseMock();
    ~ISMSDatabaseMock() override;

    MOCK_METHOD(void, addSMS,(common::PhoneNumber,common::PhoneNumber,std::string,bool,SmsTransmissionState),(final));
    MOCK_METHOD(ITextMessage&, getSMS,(unsigned int),(final));
    MOCK_METHOD((std::vector<std::pair<unsigned int, std::shared_ptr<ITextMessage>>>&), getAllSMS,(),(final));
    MOCK_METHOD(void,addSMS,(ITextMessage&),(final));
    MOCK_METHOD((std::optional<std::shared_ptr<ITextMessage>>),getLastSMSSend,(),(final));
};

}