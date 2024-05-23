#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include "Node.hpp"
#include "scopeGuard.hpp"

namespace rav = ravinskij;

void rav::printHelp()
{
  std::cout << "Usage: huffman [COMMAND] [FILES]...\n";
  std::cout << "Working with encoding and decoding texts from files.\n";
  std::cout << "Example: huffman encode text1 text2 encoding1\n";
  std::cout << "\nWorking with text:\n";
  std::cout << "\tadd-text\t <text-name> <file>\n"; 
  std::cout << "\tsave-text\t <text-name> <file>\n"; 
  std::cout << "\tdelete-text\t <text-name>\n"; 
  std::cout << "\tprint-text\t <text-name>\n"; 

  std::cout << "\nEncoding/decoding:\n";
  std::cout << "\tcreate-encoding\t <text-name> <encoding-name>\t\n";
  std::cout << "\tdelete-encoding\t <encoding-name>\t\n";
  std::cout << "\tencode\t\t <text-name>  <encoded-name> <encoding-name> \t\n";
  std::cout << "\tdecode\t\t <encoded-text> <decoded-text> <encoding-name>\t\n";
  std::cout << "\tadd-encoding\t <encoding-name> <file>\t\n";
  std::cout << "\tsave-encoding\t <encoding-name> <file>\t\n";

  std::cout << "\nComparing encodings:\n";
  std::cout << "\tcompare-encodings <text-name> <encoding-name-1> <encoding-name-2> <...> <encoding-name-N>\t";

  std::cout << "\n\nDescription:\n";
  std::cout << "\tadd-text\t add text to work with\n";
  std::cout << "\tsave-text\t write text to the file (content of the file will be overwritten)\n";
  std::cout << "\tdelete-text\t delete text to work with\n";
  std::cout << "\tprint-text\t print the text to the console\n";
  std::cout << "\tcreate-encoding\t create encoding table in accordance with the text\n";
  std::cout << "\tdelete-encoding\t delete encoding table to work with\n";
  std::cout << "\tencode\t\t encode the text in accordance with the encoding table\n";
  std::cout << "\tdecode\t\t decode the text in accordance with the encoding table\n";
  std::cout << "\tadd-encoding\t add the encoding table from the file in the format 'symbol - binary code'\n";
  std::cout << "\tsave-encoding\t save the encoding table to the file in the format 'symbol - binary code'\n";
  std::cout << "\tcompare-encodings\t Compares encodings applied to the same text.\n";
  std::cout << "\t\t\t\tDisplays a comparison table of the format on the console:";
  std::cout << "encoding name - size of the compressed text in bytes - \n\t\t\t\tcompression percentage accurate to two decimal places";
  std::cout << "(for the source text, the text name is displayed instead of the encoding).\n";
}

constexpr int bitsInByte()
{
  return 8;
}

void readAlphabet(std::istream &input, rav::Tree<char, int> &alphabet)
{
  rav::ScopeGuard guard(input);
  input >> std::noskipws;
  char c = 0;
  while (!input.eof())
  {
    input >> c;
    alphabet[c]++;
  }

  input.clear();
  input.seekg(0); // перемещаем указатель снова в начало файла
}

void buildHuffmanTree(rav::List<rav::nodePtr> &lst, const rav::Tree<char, int> &alphabet, rav::NodeComparator comp)
{
  for (auto itr = alphabet.cbegin(); itr != alphabet.cend(); ++itr)
  {
    rav::nodePtr p = std::make_shared<rav::Node>();
    p->symbol = itr->first;
    p->frequency = itr->second;
    lst.push_back(p);
  }

  //////  создаем дерево
  while (lst.size() != 1)
  {
    lst.sort(comp);

    rav::nodePtr leftChild = lst.front();
    lst.pop_front();
    rav::nodePtr rightChild = lst.front();
    lst.pop_front();

    rav::nodePtr parent = std::make_shared<rav::Node>(leftChild, rightChild);
    lst.push_back(parent);
  }
}

void buildTable(rav::nodePtr root, std::vector<bool> &code, rav::encodeMap &table)
{

  if (root== nullptr)
  {
    throw std::logic_error("empty pointer");
  }
  if (root->left != nullptr)
  {
    code.push_back(0);
    buildTable(root->left, code, table);
  }

  if (root->right != nullptr)
  {
    code.push_back(1);
    buildTable(root->right, code, table);
  }

  if (root->left == nullptr && root->right == nullptr)
    table[root->symbol] = code;

  code.pop_back();
}

void encodeAndWrite(const rav::encodeMap &table, std::istream &input, std::ostream &output)
{
  int position = 0;
  char buf = 0;
  while (!input.eof())
  {
    char c = input.get();
    if (c == EOF)
    {
      break;
    }
    std::vector<bool> x = table.at(c);
    for (size_t n = 0; n < x.size(); n++)
    {
      buf = buf | x[n] << (bitsInByte() - 1 - position);
      position++;
      if (position == bitsInByte())
      {
        position = 0;
        output << buf;
        buf = 0;
      }
    }
  }
}

void decodeAndWrite(const rav::List<rav::nodePtr>& travers, std::istream &input, std::ostream &output)
{
  rav::nodePtr root = travers.front();
  rav::nodePtr traverser =root;
  int position = 0;
  char byte;
  byte = input.get();
  while (!input.eof())
  {
    bool checkedBitState = byte & (1 << (bitsInByte() - 1 - position));
    if (checkedBitState)
      traverser = traverser->right;
    else
      traverser = traverser->left;
    if (traverser->left == nullptr && traverser->right == nullptr)
    {
      output << traverser->symbol;
      traverser = root;
    }
    position++;
    if (position == bitsInByte())
    {
      position = 0;
      byte = input.get();
    }
  }
  output << '\n';
}

void copyFile(std::ifstream& in, std::ostream& out)
{
  while(!in.eof())
  {
    std::string line;
    std::getline(in, line, '\n');
    out << line << '\n';
  }
}

void rav::addText(std::istream& in, fileTable& files)
{
  std::string textName, fileName;
  in >> textName >> fileName;

  if (files.find(textName) != files.end())
  {
    throw std::logic_error("Such text already exists");
  }

  std::ifstream input(fileName);
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  files.insert({textName, fileName});
  input.close();
}

void rav::saveText(std::istream& in, fileTable& files)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  if (files.find(textName) == files.end())
  {
    throw std::logic_error("Requested text is not found");
  }
  std::ifstream input(files.find(textName)->second);
  std::ofstream output(fileName);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  copyFile(input, output);
  input.close();
  output.close();
}

void rav::deleteText(std::istream& in, fileTable& files)
{
  std::string textName;
  in >> textName;
  if (files.find(textName) == files.end())
  {
    throw std::logic_error("Requested text is not found");
  }
  files.erase(textName);
}

void rav::printText(std::istream& in, std::ostream& out, const fileTable& files)
{
  std::string textName;
  in >> textName;
  if (files.find(textName) == files.cend())
  {
    throw std::logic_error("Requested text is not found");
  }
  std::ifstream input(files.find(textName)->second);
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  copyFile(input, out);
}

void rav::createEncoding(std::istream& in, encodesTable& encodings, traverserTable& traverses, const fileTable& files)
{
  std::string textName, encodingName;
  in >> textName >> encodingName;
  if (encodings.find(encodingName) != encodings.end())
  {
    throw std::logic_error("Such encoding already exists");
  }

  std::ifstream input(files.at(textName));
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  rav::Tree<char, int> alphabet;
  readAlphabet(input, alphabet);
  rav::List<rav::nodePtr> tree;
  buildHuffmanTree(tree, alphabet, rav::NodeComparator());
  traverses.insert({encodingName, tree});
  if (tree.empty())
  {
    throw std::logic_error("Empty tree");
  }
  rav::nodePtr root = tree.front();
  std::vector<bool> code;
  buildTable(root, code, encodings[encodingName]);
}

void rav::deleteEncoding(std::istream& in, encodesTable& encodings, traverserTable& traverses)
{
  std::string encodingName;
  in >> encodingName;
  if (encodings.find(encodingName) == encodings.end())
  {
    throw std::logic_error("No such encoding is provided");
  }
  encodings.erase(encodingName);
  traverses.erase(encodingName);
}

void rav::encode(std::istream& in, const encodesTable& encodings, fileTable& files)
{
  std::string textName, encodedName, encodingName;
  in >> textName >> encodedName >> encodingName;

  if (encodings.find(encodingName) == encodings.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  if (files.find(textName) == files.end())
  {
    throw std::logic_error("No such text is provided");
  }
  if (textName == encodedName)
  {
    throw std::logic_error("Names collision occured");
  }

  std::ifstream input(files[textName]);
  files.insert({encodedName, encodedName});
  std::ofstream output(files[encodedName], std::ios::out | std::ios::binary);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open files");
  }
  encodeAndWrite(encodings.find(encodingName)->second, input, output);
}

void rav::decode(std::istream& in, const traverserTable& traverses, fileTable& files)
{
  std::string textName, decodedName, encodingName;
  in >> textName >> decodedName >> encodingName;
  if (textName == decodedName)
  {
    throw std::logic_error("Names collision has occured");
  }
  if (traverses.find(encodingName) == traverses.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  std::ifstream input(files.at(textName), std::ios::in | std::ios::binary);
  std::ofstream output(decodedName);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open any file");
  }
  rav::List<rav::nodePtr> traverser = traverses.find(encodingName)->second;
  decodeAndWrite(traverser, input, output);
  files.insert({decodedName, decodedName});
}
