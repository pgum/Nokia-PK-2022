#pragma once
#include <Messages/PhoneNumber.hpp>
#include <chrono>

class Sms {
private:
    std::string text;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool read = false;
    bool received = true; // TODO: handling failed sms sending
    std::chrono::time_point <std::chrono::system_clock> receiveTime;

public:
    Sms();

    Sms(std::string text, const common::PhoneNumber &from, const common::PhoneNumber &to, bool read = false, bool received = true,
        const std::chrono::time_point<std::chrono::system_clock> &receiveTime = std::chrono::system_clock::now());

    void markAsRead();
    void markAsNotReceived();
    bool isRead() const;
    bool isReceived() const;
    std::string getText();

    const common::PhoneNumber &getFrom() const;

    const common::PhoneNumber &getTo() const;

    const std::chrono::time_point<std::chrono::system_clock> &getReceiveTime() const;
};

