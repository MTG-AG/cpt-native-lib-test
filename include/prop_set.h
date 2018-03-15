
#ifndef __prop_set__H_
#define __prop_set__H_

#include <string>
#include <map>

class properties_spec_t : public std::map<std::string, std::string>
{ };


class property_set_t : std::map<std::string, std::string>
{
public:

  typedef enum { value_in_property_str_is_required_e, value_in_property_str_is_not_required_e } property_string_form_t;

  property_set_t(
    std::string const      & filename,
    properties_spec_t const& spec = properties_spec_t()
  );
  property_set_t(
    int                    argc,
    const char**           argv,
    properties_spec_t const& spec = properties_spec_t()
  );
  std::string get_property_as_string(std::string const& index) const;
  std::string get_property_as_string_default_empty(std::string const& index) const;
  uint32_t get_property_as_u32(std::string const& index) const;
  bool get_as_bool_required(std::string const& index) const
  {
    bool* default_val = nullptr;

    return get_property_as_bool(index, default_val);
  }

  bool get_as_bool_default_true(std::string const& index) const
  {
    bool default_val = true;

    return get_property_as_bool(index, &default_val);
  }

  bool get_as_bool_default_false(std::string const& index) const
  {
    bool default_val = false;

    return get_property_as_bool(index, &default_val);
  }

  bool have_index(std::string const& index) const;
  uint32_t get_property_as_u32_default(
    std::string const& index,
    uint32_t       default_val
  ) const;
  std::string const& get_filename() const
  {return m_filename;};

private:

  void throw_exception(
    std::string const& text,
    std::string const& property = ""
  ) const;
  void add_index_name_string_with_equation_mark(
    std::string const      & s,
    property_string_form_t form
  );
  bool get_property_as_bool(
    std::string const& index,
    bool*            default_ptr
  ) const;
  void ensure_index(std::string const& index) const;
  std::string m_filename;
  properties_spec_t m_spec;
};


#endif /* h-guard */
