#pragma once

#include <memory>
#include <functional>
#include "CommonEnvironment/ITransport.hpp"
#include "Messages.hpp"

namespace bts
{

using common::ITransport;
using common::BinaryMessage;
using ITransportPtr = std::shared_ptr<ITransport>;
using UeConnectedCallback=std::function<void(ITransportPtr)>;

}
