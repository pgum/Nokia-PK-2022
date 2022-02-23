#pragma once

#include "UeRelay/IUeRelay.hpp"
#include "ITransport.hpp"

namespace bts
{

class IUeConnectionFactory
{
public:
    virtual ~IUeConnectionFactory() = default;
    virtual IUeRelay::UePtr createConnection(ITransportPtr transport) = 0;
};

}
