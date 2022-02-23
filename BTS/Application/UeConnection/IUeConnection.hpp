#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include "Messages.hpp"
#include "Messages/BtsId.hpp"


namespace bts
{

using common::BinaryMessage;
using common::PhoneNumber;
using common::BtsId;

class UeSlot;
class IUeConnection
{
public:
    using UePtr = std::unique_ptr<IUeConnection>;
    virtual ~IUeConnection() = default;

    virtual void start(UeSlot ueSlot) = 0;
    virtual void sendMessage(BinaryMessage message) = 0;
    virtual void sendSib(BtsId btsId) = 0;
    virtual PhoneNumber getPhoneNumber() const = 0;
    virtual bool isAttached() const = 0;
    virtual void print(std::ostream&) const = 0;
};

inline std::ostream& operator << (std::ostream& os, IUeConnection const& ue)
{
    ue.print(os);
    return os;
}

}
