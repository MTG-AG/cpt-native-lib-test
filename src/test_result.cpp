#include "test_result.h"
#include "exceptn.h"
#include <string>
#include <algorithm>

using namespace std;

namespace
{
  std::string newlines_to_spaces(std::string const& s)
  {
    std::string result;
    for( char c : s)
    {
      if(c == '\n')
      {
        c = ' ';
      }
      result += c;
    }
    return result;
  }
}

std::string verification_result_to_string(verification_result_t ver_res)
{
  if(ver_res == verification_result_exception)
  {
    return string("Ausnahme");
  }
  else if(ver_res == verification_result_ok)
  {
    return string("VALID");
  }
  else if(ver_res == verification_result_decoding_error)
  {
    return string("Dekodierungsfehler");
  }
  else if(ver_res == verification_result_appl_spec_error)
  {
    return string("INVALID");
  }
  else if(ver_res == verification_result_ambiguous)
  {
    return string("nicht-eindeutiges Ergebnis");
  }
  else
  {
    return string("internal problem: unmapped value");
  }
}

test_result_t::test_result_t(
  test_params_t         test_case,
  verification_result_t ver_result,
  std::string const     & error_string
)
  : m_test_name(test_case.get_name()),
  m_expected_verify_result(test_case.get_expected_verify_result()),
  m_severity(test_case.get_severity()),
  m_ver_result(ver_result),
  m_error_string(error_string),
  m_descrption(test_case.get_description())
{ }

bool test_result_t::is_test_passed() const
{
  if(m_severity == severity_out_of_scope)
  {
    return true;
  }
  if((m_expected_verify_result == expected_to_verify_unspecified) || (m_ver_result == verification_result_ambiguous))
  {
    return false;
  }
  else if(m_expected_verify_result == expected_to_verify_ok)
  {
    if(m_ver_result == verification_result_ok)
    {
      return true;
    }
    return false;
  }
  else if(m_expected_verify_result == expected_to_verify_fail)
  {
    if((m_ver_result == verification_result_decoding_error) || (m_ver_result == verification_result_appl_spec_error))
    {
      return true;
    }
    /* an exception (verification_result_exception) during path validation leads to failing test */
    return false;
  }
  return false;
}

std::string test_result_t::to_string() const
{
  return to_string_inner(false);
}

std::string test_result_t::to_string_table() const
{
  return to_string_inner(true);
}


std::string test_result_t::to_string_inner(bool csv) const
{
  std::string result(m_test_name);
  std::string sep("|");
  if(!csv)
  {
    result += ": ";
  }
  else
  {
    result += sep;
  }
  if((m_ver_result == verification_result_exception)) 
  {
    result += "Ausnahme";
  }
  else if(m_ver_result == verification_result_ambiguous)
  {
    if(!csv)
    {
      result += "FAIL/AMBIGUOUS";
    }
    else
    {
      result += "FAIL";
    }
  }
  else
  {
    result += (is_test_passed() ? "PASS" : (m_severity == severity_error ? "ERROR" : "WARNING"));
  }
  std::string error_string = m_error_string;
  if(error_string == "")
  {
    error_string = "–";
  }
  if(!csv)
  {
    result += ", ";
    result += "expected result ='" + expected_verify_result_to_string(m_expected_verify_result) + "', ";
    result += "actual result = '" + verification_result_to_string(m_ver_result) + "', ";
    result += "error string = '" + newlines_to_spaces(error_string) + "', ";
    result += "description = '" + m_descrption + "'";
  }
  else
  {
    result += sep;
    result += expected_verify_result_to_string(m_expected_verify_result);
    result += sep;
    result += verification_result_to_string(m_ver_result);
      result += sep;
      result += newlines_to_spaces(error_string);
    result += sep;
    result += m_descrption;
  }
  return result;
} // test_result_t::to_string_inner

std::string test_results_to_string(
  std::vector<test_result_t> const& test_results,
  bool                            table
)
{
  string result = "";

  for(test_result_t test_res : test_results)
  {
    if(result != "")
    {
      result += "\n";
    }
    if(!table)
    {
      result += test_res.to_string();
    }
    else
    {
      result += test_res.to_string_table();
    }
  }
  return result;
}

static bool sort_test_res_hlp(
  test_result_t const& a,
  test_result_t const& b
)
{
  return a.get_name() < b.get_name();
}

void sort_test_results(std::vector<test_result_t> & test_results)
{
  std::sort(test_results.begin(), test_results.end(), sort_test_res_hlp);
}

test_result_t test_result_t::evaluate_test_results_for_single_test_case(std::vector<test_result_t> const& test_results)
{
  if(test_results.size() == 0)
  {
    throw test_exceptn_t("internal error: no test results");
  }
  test_result_t result(test_results[0]);
  for(unsigned i = 1; i < test_results.size(); i++)
  {
    test_result_t const& tr = test_results[i];
    if(tr.get_verification_result() != result.get_verification_result())
    {
      result.m_ver_result    = verification_result_ambiguous;
      result.m_error_string += "'" + result.m_error_string;
      result.m_error_string += "' <<== UND ==>> '" + tr.m_error_string;
      break;
    }
  }
  return result;
}
