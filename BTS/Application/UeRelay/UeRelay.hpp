#pragma once

#include <map>
#include <list>
#include <memory>
#include <tuple>
#include "IUeRelay.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace bts
{

class UeRelay : public IUeRelay
{
public:
    UeRelay(common::ILogger& logger);

    UeSlot add(UePtr) override;

    virtual std::size_t count() const override;
    virtual std::size_t countAttached() const override;
    virtual std::size_t countNotAttached() const override;

    virtual void visitAttachedUe(UeVisitor) override;
    virtual void visitNotAttachedUe(UeVisitor) override;

    bool sendMessage(BinaryMessage message, PhoneNumber to) override;

private:
    class UeSlotBase;
    class UeSlotAdded;
    class UeSlotAttached;

    // the fact that std::map amd std::list iterators are not invalidated on insert or erase is heavily used in the implementation of this class
    // if you decide to use other containers (like std::unsorted_set) do the appropriate changes in the add/attach/removeUe functions and maybe change the UeSlot definition
    using AttachedUe = std::map<PhoneNumber, UePtr>;
    using NotAttachedUe = std::list<UePtr>;


    AttachedUe attachedUe;
    NotAttachedUe notAttachedUe;
    common::PrefixedLogger logger;

};


}
