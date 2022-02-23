#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <limits>
#include <stdexcept>
#include <cstdint>
#include <map>
#include <regex>

namespace common
{

namespace Details
{

template <bool Signed> struct NumberTypeToReadImpl;

template <> struct NumberTypeToReadImpl<true>
{
    using type = long long;
};

template <> struct NumberTypeToReadImpl<false>
{
    using type = unsigned long long;
};

template <bool Signed> using NumberTypeToRead = typename NumberTypeToReadImpl<Signed>::type;

}

class MultiLineConfig
{
public:
    using Key = std::string;

    /**
     * @example Input
     *
     * key1 = 123
     * key2 = Well, that is find string
     * # key3 = this is comment - ignored
     *
     */
    MultiLineConfig(std::istream& is);

    /**
     * @example Inputs
     *
     * key1 = 123 key2= 123 key3 =123
     *
     * Caution - when contructing from main() arguments - skip program name, i.e. use (argc - 1, argv + 1)
     *
     */
    MultiLineConfig(int argc, char* argv[]);

    /**
     * @throw std::invalid_argument
     */
    std::string getString(const std::string& key) const;

    /**
     * @throw nothing
     * @return either value attached to key, or default value
     */
    std::string getString(const std::string& key, const std::string& defaultValue) const;

    /**
     * @throw std::invalid_argument, std::out_of_range
     *
     * Caution - only some types are supported and not in full range
     */
    template <typename T>
    T getNumber(const Key& key) const;

    /**
     * @throw nothing
     * @return either value attached to key, or default value
     */
    template <typename T>
    T getNumber(const Key& key, T defaultValue) const;

    /**
     * Caution - this function does not overwrite existing key,value pairs
     */
    void insertFrom(const MultiLineConfig& other);

private:
    void parseLine(const std::string& line);

    void getNumber(Details::NumberTypeToRead<true>& valueToRetrieve, const std::string& text) const;
    void getNumber(Details::NumberTypeToRead<false>& valueToRetrieve, const std::string& text) const;

    void assertNumberLength(std::size_t readSequenceLength, const std::string& text) const;

    using Values = std::map<Key,std::string>;
    Values values;

    struct KeyValueRegex
    {
        std::smatch::size_type keyIndex;
        std::smatch::size_type valueIndex;
        std::regex lineRegex;
    };

    static KeyValueRegex makeKeyValueRegex();
    static const KeyValueRegex keyValueRegex;
};


template <typename T>
T MultiLineConfig::getNumber(const Key& key) const
{
    Details::NumberTypeToRead<std::is_signed<T>::value> numberFromText{};
    getNumber(numberFromText, this->getString(key));

    T returnValue = static_cast<T>(numberFromText);

    if (returnValue != numberFromText)
    {
        throw std::out_of_range(std::to_string(numberFromText) + " is out of range!");
    }

    return returnValue;
}

template <typename T>
T MultiLineConfig::getNumber(const Key& key, T defaultValue) const
{
    try
    {
        return this->template getNumber<T>(key);
    }
    catch (std::exception&)
    {
        return defaultValue;
    }
}
}
