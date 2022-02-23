#include "MultiLineConfig.hpp"
#include <regex>

namespace common
{

MultiLineConfig::MultiLineConfig(std::istream &is)
{
    std::string line;
    while (is.peek() != EOF && std::getline(is, line))
    {
        if (line.empty())
        {
            continue;
        }
        parseLine(line);
    }
}

MultiLineConfig::MultiLineConfig(int argc, char *argv[])
{
    std::string line;
    for (int i = 0; i < argc; ++i)
    {
        line += argv[i];
        auto equalSign = line.find('=');
        if (equalSign == std::string::npos) // still not present
        {
            continue;
        }
        if ((equalSign == line.length() - 1) && ((i + 1) < argc))
        {
            continue;
        }
        parseLine(line);
        line = "";
    }
}

std::string MultiLineConfig::getString(const std::string& key) const
{
    auto it = values.find(key);
    if (it == values.end())
        throw std::invalid_argument("Key not present: \"" + key + "\"");
    return it->second;
}

std::string MultiLineConfig::getString(const std::string &key, const std::string &defaultValue) const
{
    try
    {
        return getString(key);
    }
    catch (std::invalid_argument&)
    {
        return defaultValue;
    }
}

void MultiLineConfig::insertFrom(const MultiLineConfig &other)
{
    values.insert(other.values.begin(), other.values.end());
}

const MultiLineConfig::KeyValueRegex MultiLineConfig::keyValueRegex = makeKeyValueRegex();
MultiLineConfig::KeyValueRegex MultiLineConfig::makeKeyValueRegex()
{
    KeyValueRegex returnValue{};
    // ignoring leading and following whitespaces,
    // takes everything that is not any of "=#"
    const std::string keyRegex = "\\s*([^\\s=#]([^=#]*[^\\s=#])?)\\s*";
    // str(2) is key                  ^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // str(3) is key w/o 1st char              ^^^^^^^^^^^^^^^^
    returnValue.keyIndex = 2;

    // ignoring leading and following whitespaces,
    // takes everything that is not "#"
    const std::string valueRegex = "\\s*([^\\s#]([^#]*[^\\s#])?)?\\s*";
    // str(4) is value                  ^^^^^^^^^^^^^^^^^^^^^^^^
    // str(5) is value w/o 1st char             ^^^^^^^^^^^^^
    returnValue.valueIndex = 4;

    returnValue.lineRegex = std::regex("(" + keyRegex + "=" + valueRegex + ")?(#.*)?");
    // str(1) is key=value              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // str(6) is for comment                                                  ^^^^^

    return returnValue;
}

void MultiLineConfig::parseLine(const std::string &line)
{
    std::smatch matchKeyValue;
    if (std::regex_search(line, matchKeyValue, keyValueRegex.lineRegex)
        && matchKeyValue.size() >= std::max(keyValueRegex.keyIndex, keyValueRegex.valueIndex))
    {
        std::string key = matchKeyValue.str(keyValueRegex.keyIndex);
        std::string value = matchKeyValue.str(keyValueRegex.valueIndex);
        if (not key.empty())
        {
            values[key] = value;
        }
    }
}

void MultiLineConfig::assertNumberLength(std::size_t readSequenceLength, const std::string &text) const
{
    if (readSequenceLength != text.length())
    {
        throw std::invalid_argument("There are characters in \"" + text + "\" after pos: " + std::to_string(readSequenceLength));
    }
}

void MultiLineConfig::getNumber(Details::NumberTypeToRead<true> &valueToRetrieve, const std::string &text) const
{
    std::size_t pos = 0;
    valueToRetrieve = stoll(text, &pos, 0);
    assertNumberLength(pos, text);
}

void MultiLineConfig::getNumber(Details::NumberTypeToRead<false> &valueToRetrieve, const std::string &text) const
{
    std::size_t pos = 0;
    valueToRetrieve = stoull(text, &pos, 0);
    assertNumberLength(pos, text);
}

}
