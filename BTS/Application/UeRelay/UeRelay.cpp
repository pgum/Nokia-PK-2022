#include "UeRelay.hpp"

namespace bts
{

class UeRelay::UeSlotBase : public UeSlot::IImpl
{
public:
    UeSlotBase(UeRelay& relay);
    bool sendMessage(BinaryMessage message, PhoneNumber to) override;
protected:
    UeRelay& relay;
    template <typename ...Arg>
    void logError(Arg&& ...arg);
    template <typename ...Arg>
    void logDebug(Arg&& ...arg);
};

class UeRelay::UeSlotAdded : public UeSlotBase
{
public:
    UeSlotAdded(UeRelay& relay, UePtr ue);

    UeSlot::IImplPtr attach(PhoneNumber phone) override;
    bool isAttached() const;
    PhoneNumber getPhoneNumber() const;
    void remove() override;

private:
    NotAttachedUe::iterator whereAdded;
};

class UeRelay::UeSlotAttached : public UeSlotBase
{
public:
    UeSlotAttached(UeRelay& relay, AttachedUe::iterator whereAdded);

    UeSlot::IImplPtr attach(PhoneNumber phone) override;
    bool isAttached() const;
    PhoneNumber getPhoneNumber() const;
    void remove() override;

private:
    AttachedUe::iterator whereAdded;
};


UeRelay::UeRelay(common::ILogger &logger)
    : logger(logger, "[RELAY]")
{}

UeSlot UeRelay::add(UePtr ue)
{
    return UeSlot(std::make_shared<UeSlotAdded>(*this, std::move(ue)));
}

bool UeRelay::sendMessage(BinaryMessage message, PhoneNumber to)
{
    auto ueSlot = attachedUe.find(to);
    if (ueSlot == attachedUe.end())
    {
        logger.logError("Connection does not exist for: ", to);
        return false;
    }
    ueSlot->second->sendMessage(message);
    return true;
}

std::size_t UeRelay::count() const
{
    return countAttached() + countNotAttached();
}

std::size_t UeRelay::countAttached() const
{
    return attachedUe.size();
}

std::size_t UeRelay::countNotAttached() const
{
    return notAttachedUe.size();
}

void UeRelay::visitAttachedUe(IUeRelay::UeVisitor ueVisitor)
{
    for (auto& ue: attachedUe)
    {
        ueVisitor(*(ue.second));
    }
}

void UeRelay::visitNotAttachedUe(IUeRelay::UeVisitor ueVisitor)
{
    for (auto& ue: notAttachedUe)
    {
        ueVisitor(*ue);
    }
}

UeRelay::UeSlotBase::UeSlotBase(UeRelay &relay)
    : relay(relay)
{}

template <typename ...Arg>
void UeRelay::UeSlotBase::logError(Arg&& ...arg)
{
    relay.logger.logError(std::forward<Arg>(arg)...);
}

template <typename ...Arg>
void UeRelay::UeSlotBase::logDebug(Arg&& ...arg)
{
    relay.logger.logDebug(std::forward<Arg>(arg)...);
}

bool UeRelay::UeSlotBase::sendMessage(BinaryMessage message, PhoneNumber to)
{
    return relay.sendMessage(std::move(message), to);
}

UeRelay::UeSlotAdded::UeSlotAdded(UeRelay &relay, UePtr ue)
    : UeSlotBase(relay),
      whereAdded(relay.notAttachedUe.insert(relay.notAttachedUe.begin(), std::move(ue)))
{
}

UeSlot::IImplPtr UeRelay::UeSlotAdded::attach(PhoneNumber phone)
{
    auto result = relay.attachedUe.insert(AttachedUe::value_type(phone, UePtr{}));
    if (result.second)
    {
        result.first->second = std::move(*whereAdded);
        logDebug("Attached: ", *result.first->second);
        relay.notAttachedUe.erase(whereAdded);
        return std::make_shared<UeSlotAttached>(relay, result.first);
    }

    logError("While attaching: other connection exists for: ", phone);
    return shared_from_this();
}

bool UeRelay::UeSlotAdded::isAttached() const
{
    return false;
}

PhoneNumber UeRelay::UeSlotAdded::getPhoneNumber() const
{
    return {};
}

void UeRelay::UeSlotAdded::remove()
{
    auto ue = std::move(*whereAdded);
    logDebug("Removed not attached: ", *ue);
    relay.notAttachedUe.erase(whereAdded);
    ue.reset();
}

UeRelay::UeSlotAttached::UeSlotAttached(UeRelay &relay, AttachedUe::iterator whereAdded)
    : UeSlotBase(relay),
      whereAdded(whereAdded)
{}

UeSlot::IImplPtr UeRelay::UeSlotAttached::attach(PhoneNumber phone)
{
    if (phone == whereAdded->first)
    {
        logDebug("Reattached to same phone number ignored: ", *whereAdded->second);
        return shared_from_this();
    }

    UePtr ue = std::move(whereAdded->second);
    struct EraseOnExit
    {
        EraseOnExit(UeSlotAttached& thisObject) : thisObject(thisObject) {}
        ~EraseOnExit()
        {
            // till the end of thisObject - the slot must be there
            // otherwise getPhoneNumber reads not own memory: whereAdded->first
            // that is why it is last instruction before return...
            thisObject.relay.attachedUe.erase(thisObject.whereAdded);
        }
        UeSlotAttached& thisObject;
    } eraseOnExit(*this);


    auto result = relay.attachedUe.insert(AttachedUe::value_type(phone, UePtr{}));
    if (result.second)
    {
        result.first->second = std::move(ue);
        logDebug("Attached: ", *result.first->second);
        return std::make_shared<UeSlotAttached>(relay, result.first);
    }

    logError("While re-attaching: other connection exists for: ", phone);
    return std::make_shared<UeSlotAdded>(relay, std::move(ue));
}

bool UeRelay::UeSlotAttached::isAttached() const
{
    return true;
}

PhoneNumber UeRelay::UeSlotAttached::getPhoneNumber() const
{
    return whereAdded->first;
}

void UeRelay::UeSlotAttached::remove()
{
    UePtr ue = std::move(whereAdded->second);
    logDebug("Removed attached: ", *ue);
    relay.attachedUe.erase(whereAdded);
    ue.reset();
}

}
