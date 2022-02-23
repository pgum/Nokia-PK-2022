#pragma once

#include <memory>
#include "UeConnection/IUeConnection.hpp"


namespace bts
{

class UeSlot
{
public:
    class IImpl;
    using IImplPtr = std::shared_ptr<IImpl>;
    class IImpl : public std::enable_shared_from_this<IImpl>
    {
    public:
        virtual ~IImpl() = default;
        virtual bool sendMessage(BinaryMessage message, PhoneNumber to) = 0;
        virtual IImplPtr attach(PhoneNumber phone) = 0;
        virtual bool isAttached() const = 0;
        virtual PhoneNumber getPhoneNumber() const = 0;
        virtual void remove() = 0;
    };

    UeSlot();
    UeSlot(IImplPtr impl);
    bool sendMessage(BinaryMessage message, PhoneNumber to);
    void attach(PhoneNumber phone);
    bool isAttached() const;
    PhoneNumber getPhoneNumber() const;
    void remove();

private:
    IImplPtr impl;
    class NullImpl;
};

}
