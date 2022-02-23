#pragma once

#include <functional>
#include "Messages.hpp"

namespace common
{

class ITransport
{
public:
    using MessageCallback=std::function<void (BinaryMessage)>;
    using DisconnectedCallback=std::function<void()>;

    virtual ~ITransport() = default;

    virtual void registerMessageCallback(MessageCallback) = 0;
    virtual void registerDisconnectedCallback(DisconnectedCallback) = 0;

    virtual bool sendMessage(BinaryMessage) = 0;

    virtual std::string addressToString() const = 0;
};

}
