#ifndef STRINGTOOLS_STRINGTOOLS_H
#define STRINGTOOLS_STRINGTOOLS_H

#include <string>
#include <vector>

/* Handy utensils to manipulate strings */
class StringTools
{
public:
  //! Will replace every occurence of `find` in `str` by `subst`.
  static std::string Replace(const std::string& str, const char find, const std::string& subst);

  //! Will replace every occurence of `find` in `str` by `subst`.
  static std::string Replace(const std::string& str, const std::string& find, const std::string& subst);

  //! Will replace every occurence of `find` in `str` by `subst`.
  static std::string Replace(const std::string& str, const char find, const char subst);

  //! Will replace every occurence of `find` in `str` by `subst`.
  static std::string Replace(const std::string& str, const std::string& find, const char subst);

  //! Will make a string all-lowercase.
  static std::string Lower(const std::string& str);

  //! Will make a string all-uppercase.
  static std::string Upper(const std::string& str);

  //! Will split a string by a string seperator
  static std::vector<std::string> Split(const std::string& str, const std::string& seperator);

  //! Will pad a string to the left to length l 
  static std::string PadLeft(const std::string& str, const char pad, const std::size_t len);

  //! Will pad a string to the right to length l 
  static std::string PadRight(const std::string& str, const char pad, const std::size_t len);

private:
  // No instanciation! >:(
  StringTools();
};

#endif //STRINGTOOLS_STRINGTOOLS_H
