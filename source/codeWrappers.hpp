#ifndef CODE_WRAPPERS_HPP
#define CODE_WRAPPERS_HPP

#include <iosfwd>
#include <vector>
#include "BoolVector.hpp"

namespace ravinskij
{
  struct ReadWrapper
  {
    char &symbol;
    BoolVector &code;
  };

  struct WriteWrapper
  {
    const char &symbol;
    const BoolVector &code;
  };
  
  std::ostream& operator<<(std::ostream& out, WriteWrapper&& wrapper);
  std::istream& operator>>(std::istream& in, ReadWrapper&& wrapper);
}

#endif