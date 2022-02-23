#include "UeSlot.hpp"

namespace bts
{

class UeSlot::NullImpl : public IImpl
{
public:
    bool sendMessage(BinaryMessage message, PhoneNumber to) override;
    IImplPtr attach(PhoneNumber phone) override;
    bool isAttached() const override;
    PhoneNumber getPhoneNumber() const override;
    void remove() override;
};

UeSlot::UeSlot() : UeSlot(std::make_shared<NullImpl>())
{}

UeSlot::UeSlot(IImplPtr impl)
    : impl(impl)
{}

bool UeSlot::sendMessage(BinaryMessage message, PhoneNumber to)
{
    return impl->sendMessage(std::move(message), to);
}

void UeSlot::attach(PhoneNumber phone)
{
    impl = impl->attach(phone);
}

bool UeSlot::isAttached() const
{
    return impl->isAttached();
}

PhoneNumber UeSlot::getPhoneNumber() const
{
    return impl->getPhoneNumber();
}

void UeSlot::remove()
{
    impl->remove();
}

bool UeSlot::NullImpl::sendMessage(BinaryMessage message, PhoneNumber to)
{
    return false;
}

UeSlot::IImplPtr UeSlot::NullImpl::attach(PhoneNumber phone)
{
    return shared_from_this();
}

bool UeSlot::NullImpl::isAttached() const
{
    return false;
}

PhoneNumber UeSlot::NullImpl::getPhoneNumber() const
{
    return {};
}

void UeSlot::NullImpl::remove()
{
}

}
