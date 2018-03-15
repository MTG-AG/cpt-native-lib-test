#include <fstream>
#include <iostream>
#include "test_params.h"
#include "util.h"
#include "readdir.h"
#include "exceptn.h"
#include "parse_test_spec.h"

using namespace std;

std::string expected_verify_result_to_string(expected_verify_result_t exp_res)
{
  if(exp_res == expected_to_verify_unspecified)
  {
    return string("unspecified/unknown");
  }
  else if(exp_res == expected_to_verify_ok)
  {
    return string("VALID");
  }
  else if(exp_res == expected_to_verify_fail)
  {
    return string("INVALID");
  }
  else
  {
    return string("internal problem: unmapped value");
  }
}


test_params_t::test_params_t(std::string const& dir)
  : m_expected_verify_result(expected_to_verify_unspecified)
{
  size_t sep_pos = dir.find_last_of('/');

  if(sep_pos == string::npos)
  {
    sep_pos = 0;
  }
  else
  {
    sep_pos++;
  }
  string leaf = dir.substr(sep_pos, dir.size() - sep_pos);
  m_test_name = leaf;

  vector<string> trust_anchors_str = get_entries_of_dir(dir, dir_entries_with_path, std::string(".TA.crt"));
  vector<string> ca_certs_str      = get_entries_of_dir(dir, dir_entries_with_path, std::string(".CA.crt"));
  vector<string> target_cert_str   = get_entries_of_dir(dir, dir_entries_with_path, std::string(".TC.crt"));


  vector<string> trust_anchors_str_pem = get_entries_of_dir(dir, dir_entries_with_path, std::string(".TA.pem.crt"));
  vector<string> ca_certs_str_pem      = get_entries_of_dir(dir, dir_entries_with_path, std::string(".CA.pem.crt"));
  vector<string> target_cert_str_pem   = get_entries_of_dir(dir, dir_entries_with_path, std::string(".TC.pem.crt"));

   m_trust_anchors = file_contents_from_paths(trust_anchors_str);
   m_ca_certs = file_contents_from_paths(ca_certs_str);

   m_trust_anchors_pem = file_contents_from_paths(trust_anchors_str_pem);
   m_ca_certs_pem = file_contents_from_paths(ca_certs_str_pem);
   std::vector<std::vector<uint8_t>> target_certs = file_contents_from_paths(target_cert_str);
   std::vector<std::vector<uint8_t>> target_certs_pem = file_contents_from_paths(target_cert_str_pem);

  if((target_certs.size() != 1) || (target_certs_pem.size() != 1)  )
  {
    throw test_exceptn_t("number of target certs is not equal to one (dir = '" + dir + "'");
  }

  m_target_cert   = target_certs[0];
  m_target_cert_pem   = target_certs_pem[0];

  test_result_spec_t res_spec = parse_test_result_spec(dir);
  m_expected_verify_result = res_spec.expected_verify_result;
  m_severity = res_spec.severity;
  m_description = res_spec.description;

  m_crl_params = read_crls_from_testcase_dir(dir);
}

std::vector<test_params_t> construct_test_cases(std::string const& test_cases_dir)
{
  vector<string> test_dirs = get_entries_of_dir(test_cases_dir, dir_entries_with_path, "", true);
  vector<test_params_t> result;
  for(string test_dir : test_dirs)
  {
    result.push_back(test_params_t(test_dir));
  }
  return result;
}

test_params_t test_params_t::get_copy_with_rndmzd_order() const
{
  test_params_t result(*this);
  result.m_ca_certs = vector_randomized_copy(m_ca_certs);
  result.m_ca_certs_pem = vector_randomized_copy(m_ca_certs_pem);
  result.m_trust_anchors = vector_randomized_copy(m_trust_anchors);
  result.m_trust_anchors_pem = vector_randomized_copy(m_trust_anchors_pem);
  result.m_crl_params = m_crl_params.get_copy_with_rndmzd_order();
  return result;  
}
