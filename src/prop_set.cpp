#include "prop_set.h"
#include "exceptn.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

static uint32_t string_to_u32bit_unchecked(std::string const& str)
{
  std::istringstream is(str);
  uint32_t result;
  is >> result;
  return result;
}

static bool is_string_only_numeric(std::string const& str)
{
  for(unsigned i = 0; i < str.size(); i++)
  {
    if(str[i] != '0' &&
      str[i] != '1' &&
      str[i] != '2' &&
      str[i] != '3' &&
      str[i] != '4' &&
      str[i] != '5' &&
      str[i] != '6' &&
      str[i] != '7' &&
      str[i] != '8' &&
      str[i] != '9')
    {
      return false;
    }
  }
  return true;
}
static uint32_t string_to_u32bit(std::string const& str)
{
  if(!is_string_only_numeric(str))
  {
    throw test_exceptn_t("error parsing string '" + str + "' as numeric value");
  }

  return string_to_u32bit_unchecked(str);
}

static bool is_string_only_whitespaces(std::string const& str)
{
  std::string::const_iterator it;
  for(it = str.begin(); it != str.end(); it++)
  {
    if(*it != ' ')
    {
      return false;
    }
  }
  return true;
}

static std::vector<std::string> read_file_line_wise(std::string const& filename)
{
  std::ifstream input(filename);
  if(!input)
  {
    throw test_exceptn_t("could not open file " + filename);
  }
  std::vector<std::string> result;
  for(std::string line; getline(input, line);)
  {
    result.push_back(line);
  }
  return result;
}

static std::string remove_ws(std::string const& s)
{
  std::string result;
  for(char c : s)
  {
    if(c != ' ')
    {
      result.push_back(c);
    }
  }
  return result;
}

void property_set_t::add_index_name_string_with_equation_mark(
  std::string const      & s,
  property_string_form_t form
)
{
  size_t equ_pos = s.find("=");

  std::string value, name;
  if(equ_pos == std::string::npos)
  {
    if(form == value_in_property_str_is_required_e)
    {
      throw test_exceptn_t("could not parse s '" + s + "' in file " + m_filename);
    }
    name = remove_ws(s);
  }
  else
  {
    name = s.substr(0, equ_pos);
    if(equ_pos == s.size())
    {
      return;
    }
    value = s.substr(equ_pos + 1, s.size());
    name  = remove_ws(name);
    value = remove_ws(value);
  }

  if(m_spec.size() != 0)
  {
    if(m_spec.find(name) == m_spec.end())
    {
      throw_exception(std::string("error with unspecified property"), name);
    }
  }
  (*this)[name] = value;
} // property_set_t::add_index_name_string_with_equation_mark

property_set_t::property_set_t(
  int                    argc,
  const char**           argv,
  properties_spec_t const& spec
)
  : m_spec(spec)
{
  for(unsigned i = 1; i < static_cast<unsigned>(argc); i++)
  {
    std::string arg_string(argv[i]);
    if(arg_string.find("--") != 0)
    {
      throw test_exceptn_t("invalid command line arg " + arg_string);
    }
    arg_string = arg_string.substr(2, arg_string.size() - 2);
    add_index_name_string_with_equation_mark(arg_string, value_in_property_str_is_not_required_e);
  }
}

property_set_t::property_set_t(
  std::string const      & filename,
  properties_spec_t const& spec
)
  : m_filename(filename),
  m_spec(spec)
{
  vector<string> lines = read_file_line_wise(filename);
  for(string line: lines)
  {
    if((line.find("#") == 0) || is_string_only_whitespaces(line))
    {
      continue;
    }
    add_index_name_string_with_equation_mark(line, value_in_property_str_is_required_e);

  }
}

void property_set_t::throw_exception(
  std::string const& text,
  std::string const& property
) const
{
  if(property == "")
  {
    throw test_exceptn_t("error in file " + m_filename + ": " + text);
  }
  else
  {
    std::string value_inf;
    if(have_index(property))
    {
      value_inf = " with value '" + get_property_as_string(property) + "'";
    }
    throw test_exceptn_t(
            "error in file " + m_filename + " with property '" + property + "'" + value_inf + ": " + text
    );
  }
}

bool property_set_t::get_property_as_bool(
  std::string const& index,
  bool*            default_val
) const
{
  if(default_val == nullptr)
  {
    ensure_index(index);
  }
  else if(!have_index(index))
  {
    return *default_val;
  }
  if(find(index)->second == "true")
  {
    return true;
  }
  else if(find(index)->second == "false")
  {
    return false;
  }

  else
  {
    throw_exception(std::string("could not parse propery"), index);
    return false; // to make compiler happy, never reached
  }
}

std::string property_set_t::get_property_as_string(std::string const& index) const
{
  ensure_index(index);
  return find(index)->second;
}

std::string property_set_t::get_property_as_string_default_empty(std::string const& index) const
{
  if(!have_index(index))
  {
    return std::string("");
  }
  return find(index)->second;
}

bool property_set_t::have_index(std::string const& index) const
{
  return (find(index) != this->end());
}

void property_set_t::ensure_index(std::string const& index) const
{
  if(!have_index(index))
  {
    throw test_exceptn_t("did not find index '" + index + "' in file " + m_filename);
  }
}

uint32_t property_set_t::get_property_as_u32_default(
  std::string const& index,
  uint32_t       default_val
) const
{
  if(have_index(index))
  {
    return get_property_as_u32(index);
  }
  return default_val;
}

uint32_t property_set_t::get_property_as_u32(std::string const& index) const
{
  ensure_index(index);
  string value = find(index)->second;
  if(!value.size())
  {
    throw test_exceptn_t(
            std::string("value of property '") + index + std::string(
              "' in file "
            ) + m_filename + " is not numeric as expected"
    );
  }
  return string_to_u32bit(value);
}

