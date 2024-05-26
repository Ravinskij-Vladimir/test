#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <string>
#include <utility>
#include <map>
#include <list>

using dict = std::map< std::string, std::map< std::string, std::list< std::string > > >;
using dictionaryOne = std::map< std::string, std::list< std::string > >;

namespace zakozhurnikova
{
  void print(const std::list< std::string >& args, const dict& dictionary);
  void complement(std::list< std::string >& args, dict& dictionary);
  void intersect(std::list< std::string >& args, dict& dictionary);
  void doUnion(std::list< std::string >& args, dict& dictionary);
  void specificLetter(std::list< std::string >& args, dict& dictionary);
  void elimination(std::list< std::string >& args, dict& dictionary);
  void destruction(std::list< std::string >& args, dict& dictionary);
  void addition(std::list< std::string >& args, dict& dictionary);
  void palindrome(std::list< std::string >& args, dict& dictionary);
  void rider(std::list< std::string >& args, dict& dictionary);
  void interpreter(std::list< std::string >& args, dict& dictionary);
  void save(std::list< std::string >& args, dict& dictionary);
}

#endif
