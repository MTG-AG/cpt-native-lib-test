
#ifndef __util__H_
#define __util__H_

#include <string>
#include <sstream>
#include <vector>

template <typename t>
inline std::string num_to_string(t num);

template <typename t>
inline std::string num_to_string(t num)
{
    std::stringstream ss;
    std::string str_is;
    ss <<  num;
    ss >> str_is;
    return str_is;
}
template <>
inline std::string num_to_string<unsigned char>(unsigned char num)
{
  unsigned uns_num = num;
    std::stringstream ss;
    std::string str_is;
    ss <<  uns_num;
    ss >> str_is;
    return str_is;
}

  std::string read_file_as_string(std::string const& filename);

  std::vector<std::vector<uint8_t> > file_contents_from_paths(std::vector<std::string> const& paths);

  template <typename t>
  inline std::vector<t> vector_randomized_copy(std::vector<t> const& orig);


  template <typename t>
  inline std::vector<t> vector_randomized_copy(std::vector<t> const& orig)
{
  auto direct_copy = orig;
  std::vector<t> result;
  while(direct_copy.size())
  {
    size_t index = rand() % direct_copy.size();
    result.push_back(direct_copy[index]);
    direct_copy[index] = direct_copy.back();
    direct_copy.pop_back();
  }
  return result;
}
#endif /* h-guard */
