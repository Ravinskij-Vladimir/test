#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <map>
#include <list>
#include <string>
#include <vector>
#include "Node.hpp"
#include "tree.hpp"
#include "bilist.hpp"
namespace ravinskij
{
    void printHelp();
    using traverserTable = Tree<std::string, ravinskij::List<ravinskij::Node*>>;
    //using traverserTable = ravinskij::Tree<std::string, ravinksij::List<ravinskij::Node>>;
    using fileTable = ravinskij::Tree<std::string, std::string>;
    void addText(std::istream&, fileTable&);
    void saveText(std::istream&, fileTable&);
    void deleteText(std::istream&, fileTable&);
    void printText(std::istream&, std::ostream&, const fileTable&);

    using encodeMap = std::map<char, std::vector<bool>>;
    using encodesTable = std::map<std::string, encodeMap>;
    void createEncoding(std::istream&, encodesTable&, traverserTable&, const fileTable&);
    void deleteEncoding(std::istream&, encodesTable&, traverserTable&);
    void encode(std::istream&, const encodesTable&, fileTable&);
    void decode(std::istream&, const traverserTable&, fileTable&);
    void addEncoding(std::istream&, encodesTable&);
    void saveEncoding(std::istream&, encodesTable&);

    void compareEncodings(std::istream&, const fileTable&, const encodesTable&);
}

#endif