
#include "util.h"
#include <fstream>
#include "exceptn.h"

using namespace std;

static std::vector<unsigned char> read_bin_file(std::string const& filename)
{
  std::ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
  if(!file.is_open())
  {
    throw test_exceptn_t("could not open file '" + filename + "'");
  }
  std::ifstream::pos_type size;
  size = file.tellg();
  std::vector<unsigned char> result(size);
  file.seekg(0, ios::beg);
  file.read((char*) (&result[0]), size);
  file.close();
  return result;
}

vector<vector<uint8_t> > file_contents_from_paths(vector<string> const& paths)
{
  vector<vector<uint8_t> > result;
  for(string file: paths)
  {
    vector<uint8_t> bin = read_bin_file(file);
    result.push_back(bin);

  }
  return result;
}
  std::string read_file_as_string(std::string const& filename)
  {
    ifstream inFile;
    inFile.open(filename);

    stringstream strStream;
    strStream << inFile.rdbuf();
    string str = strStream.str();
    return str;
  }
