#pragma once

#include <memory>
#include <cstdint>
#include <functional>
#include "UeConnection/IUeConnection.hpp"
#include "UeConnection/UeSlot.hpp"
#include "Messages.hpp"

namespace bts
{

using common::BinaryMessage;
using common::PhoneNumber;

class IUeConnection;
class IUeRelay
{
public:
    using UePtr = IUeConnection::UePtr;
    using UeVisitor = std::function<void(IUeConnection&)>;

    virtual ~IUeRelay() = default;

    virtual UeSlot add(UePtr) = 0;

    virtual std::size_t count() const = 0;
    virtual std::size_t countAttached() const = 0;
    virtual std::size_t countNotAttached() const = 0;

    virtual void visitAttachedUe(UeVisitor) = 0;
    virtual void visitNotAttachedUe(UeVisitor) = 0;

    virtual bool sendMessage(BinaryMessage message, PhoneNumber to) = 0;
};


}
