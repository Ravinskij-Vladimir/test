#include <iostream>
#include <set>
#include <scopeGuard.hpp>
#include "inputDictionary.hpp"



void zakozhurnikova::inputArgs(std::istream& in, std::list< std::string >& args)
{
  ScopeGuard guard(in);
  in >> std::noskipws;
  std::string tmp;
  char delim = 0;
  in >> delim;
  while (in && delim != '\n')
  {
    in >> tmp;
    if (in)
    {
      args.push_back(tmp);
    }
    in >> delim;
  }
}

bool isEnglish(const std::string& temp)
{
  if ((temp[0] >= 'a' && temp[0] <= 'z') || (temp[0] >= 'A' && temp[0] <= 'Z'))
  {
    return true;
  }
  return false;
}


std::istream& zakozhurnikova::operator>>(std::istream& in, subDict& dict)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
    std::string temp;
    std::string word;
    std::set< std::string > translate;
    in >> std::noskipws;
    char space;
    in >> space;
    while (in && space != '\n')
    {
      in >> temp;
      if (isEnglish(temp) && word.empty())
      {
        word = temp;
      }
      else if (!isEnglish(temp) && !word.empty())
      {
        translate.insert(temp); // не push_back, потому что set
      }
      else if (!word.empty() && !translate.empty())
      {
        dict[word] =  translate;
        word.clear();
        translate.clear();
        word = temp;
      }
      else
      {
        std::cout << "incorret(empty) input translate word: " << temp << " - the word is not written in the dictionary\n";
      }
      in >> space;
    }
    if (!word.empty() && !translate.empty())
    {
      dict[word] =  translate;
    }
    if (!in.eof())
    {
      in.clear();
    }
  return in;
}

std::ostream& zakozhurnikova::operator<<(std::ostream& out, const subDict& dict)
{
  std::ostream::sentry guard(out);
  if (!guard)
  {
    return out;
  }
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << it->first;
    for (auto itSet = it->second.cbegin(); itSet != it->second.cend(); ++it)
    {
      out << ' ' << *itSet;
    }
  }
  return out;
}

void zakozhurnikova::inputDictionary(
  std::istream& in, std::map< std::string, subDict >& maps
)
{
  ScopeGuard guard(in);
  std::string nameDictionary;
  while (in)
  {
    in >> nameDictionary;
    subDict translation;
    in >> translation;
    if (!translation.empty() && !nameDictionary.empty())
    {
      maps[nameDictionary] = translation;
    }
    else if (!nameDictionary.empty())
    {
      std::cout << "The dictionary is empty:" << nameDictionary << '\n';
    }
  }
}
