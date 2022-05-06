#pragma once
#include <Messages/PhoneNumber.hpp>
#include <chrono>

class Sms {
private:
    std::string text;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool read = false;
    std::chrono::time_point <std::chrono::system_clock> receive;

public:
    Sms();

    Sms(const std::string &text, const common::PhoneNumber &from, const common::PhoneNumber &to, bool read,
        const std::chrono::time_point<std::chrono::system_clock> &receive);

    void markAsRead();
    bool isRead() const;
    std::string getText();

    const common::PhoneNumber &getFrom() const;

    const common::PhoneNumber &getTo() const;

    const std::chrono::time_point<std::chrono::system_clock> &getReceive() const;
};

