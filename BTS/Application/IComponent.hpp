#pragma once

namespace bts
{

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
};

}
