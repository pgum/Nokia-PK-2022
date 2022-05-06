#include <chrono>
#include "Sms.hpp"

Sms::Sms() {
    text = "text"; // TODO: tymczasowy do testow
}

Sms::Sms(const std::string &text, const common::PhoneNumber &from, const common::PhoneNumber &to, bool read,
         const std::chrono::time_point<std::chrono::system_clock> &receive) : text(text), from(from), to(to),
                                                                              read(read), receive(receive) {}
void Sms::markAsRead() {
    read = true;
}

bool Sms::isRead() const {
    return read;
}

std::string Sms::getText() {
    return text;
}

const common::PhoneNumber &Sms::getFrom() const {
    return from;
}

const common::PhoneNumber &Sms::getTo() const {
    return to;
}

const std::chrono::time_point<std::chrono::system_clock> &Sms::getReceive() const {
    return receive;
}
