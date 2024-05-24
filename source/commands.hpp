#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <string>
#include <vector>
#include "Node.hpp"
#include "tree.hpp"
#include "bilist.hpp"

namespace ravinskij
{
    void printHelp();
    using traverserTable = Tree<std::string, List<nodePtr>>;
    using fileTable = Tree<std::string, std::string>;
    void addText(std::istream&, fileTable&); 
    void saveText(std::istream&, fileTable&);
    void deleteText(std::istream&, fileTable&);
    void printText(std::istream&, std::ostream&, const fileTable&);

    using encodeMap = Tree<char, std::vector<bool>>;
    using encodesTable = Tree<std::string, encodeMap>;
    void createEncoding(std::istream&, encodesTable&, traverserTable&, const fileTable&);
    void deleteEncoding(std::istream&, encodesTable&, traverserTable&);
    void encode(std::istream&, const encodesTable&, fileTable&);
    void decode(std::istream&, const traverserTable&, fileTable&);
}

#endif