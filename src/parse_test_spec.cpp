#include "test_params.h"
#include "util.h"
#include "parse_test_spec.h"
#include "exceptn.h"
#include <iostream>

using namespace std;

std::string parse_content_for_value(
  std::string const& content,
  std::string const& index,
  std::string const& test_case_dir
)
{
  size_t pos = content.find(index);
  if(pos == string::npos)
  {
    throw test_exceptn_t("error parsing test spec for '" + index + "' in dir '" + test_case_dir + "'");
  }
  pos += index.size();

  


  size_t pos_end = content.find_first_of(0x0a, pos);
  if(pos_end == string::npos)
  {
    throw test_exceptn_t("error parsing test spec for '" + index + "' in dir '" + test_case_dir + "'");
  }
  return content.substr(pos, pos_end - pos);
}

test_result_spec_t parse_test_result_spec(std::string const& test_case_dir)
{
  string file_path = test_case_dir + "/testReport.txt";
  string content   = read_file_as_string(file_path);

  const string exp_name = string("Expected Result: ");
  string exp_res_str    = parse_content_for_value(content, exp_name, test_case_dir);
  expected_verify_result_t exp_res;

  if(exp_res_str == "VALID")
  {
    exp_res = expected_to_verify_ok;
  }
  else if(exp_res_str == "INVALID")
  {
    exp_res = expected_to_verify_fail;
  }
  else
  {
    throw test_exceptn_t("could not parse expected result '" + exp_res_str + "' in dir '" + test_case_dir + "'");
  }

  const string sev_name = string("Severity: ");
  test_severity_t severity;
  string sev_str = parse_content_for_value(content, sev_name, test_case_dir);
  if(sev_str == "ERROR")
  {
    severity = severity_error;
  }
  else if(sev_str == "WARNING")
  {
    severity = severity_warning;
  }
  else if(sev_str == "OUT_OF_SCOPE")
  {
    severity = severity_out_of_scope;
  }
  else
  {
    throw test_exceptn_t("could not parse severity '" + sev_str + "' in dir '" + test_case_dir + "'");
  }

  
  test_result_spec_t result;
  result.severity = severity;
  result.expected_verify_result = exp_res;
  result.description = parse_content_for_value(content, "Purpose: ", test_case_dir);
  return result;
} // parse_test_result_spec
