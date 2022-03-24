#include "StringTools.h"
#include <sstream>

std::string StringTools::Replace(const std::string& str, const char find, const std::string& subst) {
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != find)
            ss << str[i];
        else
            ss << subst;
    }

    return ss.str();
}

std::string StringTools::Replace(const std::string& str, const std::string& find, const std::string& subst) {
    if (find.length() == 0)
        return str;

    std::stringstream ss;

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(find, posFound);

        if (posFound != std::string::npos)
        {
            ss << str.substr(lastFound, posFound - lastFound) << subst;
            posFound += find.length();
        }
        else
        {
            ss << str.substr(lastFound, (str.length()) - lastFound);
        }
    }

    return ss.str();
}

std::string StringTools::Replace(const std::string& str, const char find, const char subst) {
    std::stringstream ss;
    ss << subst;

    return Replace(str, find, ss.str());
}

std::string StringTools::Replace(const std::string& str, const std::string& find, const char subst) {
    std::stringstream ss;
    ss << subst;

    return Replace(str, find, ss.str());
}

std::string StringTools::Lower(const std::string& str) {
    std::stringstream ss;

    for (std::size_t i = 0; i < str.size(); i++)
    {
        const char c = str[i];

        // Quick-accept: regular letters
        if ((c >= 'A') && (c <= 'Z'))
            ss << (char)(c | 32);

            // Else: keep the character as is
        else ss << c;
    }

    return ss.str();
}

std::string StringTools::Upper(const std::string& str) {
    std::stringstream ss;

    for (std::size_t i = 0; i < str.size(); i++)
    {
        const char c = str[i];

        // Quick-accept: regular letters
        if ((c >= 'a') && (c <= 'z'))
            ss << (char)(c & ~32);

            // Else: keep the character as is
        else ss << c;
    }

    return ss.str();
}

std::vector<std::string> StringTools::Split(const std::string& str, const std::string& seperator) {
    std::vector<std::string> toRet;
    // Quick-accept: str length is 0
    if (str.length() == 0)
      toRet.push_back("");

    // Quick-accept: seperator length is 0
    else if (seperator.length() == 0) {
      for (const char c : str)
        toRet.push_back(std::string(&c, (&c) + 1));
    }

    else {
      std::size_t idx = 0;
      while (idx != std::string::npos) {
        std::size_t lastIdx = idx;
        idx = str.find(seperator, idx);

        // Grab our substring until the next finding of sep
        if (idx != std::string::npos) {
          toRet.push_back(str.substr(
            lastIdx,
            idx - lastIdx
          ));

          idx += seperator.length();
        }
        // No more seperator found. Grab the rest until the end of the string
        else {
          toRet.push_back(str.substr(
             lastIdx
          ));
        }
      }
    }

    return toRet;
}

std::string StringTools::PadLeft(const std::string& str, const char pad, const std::size_t len) {
  std::stringstream ss;

  for (std::size_t i = str.length(); i < len; i++)
    ss << pad;

  ss << str;

  return ss.str();
}

std::string StringTools::PadRight(const std::string& str, const char pad, const std::size_t len) {
  std::stringstream ss;

  ss << str;

  for (std::size_t i = str.length(); i < len; i++)
    ss << pad;

  return ss.str();
}
