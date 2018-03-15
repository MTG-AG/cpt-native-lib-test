#ifndef __test_result__H_
#define __test_result__H_

#include <string>
#include "test_params.h"

typedef enum { verification_result_exception, verification_result_decoding_error, verification_result_appl_spec_error, verification_result_ok, verification_result_ambiguous } verification_result_t;

std::string verification_result_to_string(verification_result_t ver_res);


class test_result_t
{
public:
  test_result_t(
    test_params_t         test_case,
    verification_result_t ver_result,
    std::string const     & error_string = ""
  );
  std::string const& get_name() const { return m_test_name; };
  std::string const& get_description() const { return m_descrption; };
  std::string to_string() const;
  std::string to_string_table() const;
  bool is_test_passed() const;
  
  verification_result_t get_verification_result() const { return m_ver_result; };

static test_result_t evaluate_test_results_for_single_test_case(std::vector<test_result_t> const& test_results);
private:
  std::string m_test_name;
  std::string to_string_inner(bool csv) const;
  expected_verify_result_t m_expected_verify_result;
  test_severity_t m_severity;
  verification_result_t m_ver_result;
  std::string m_error_string;
  std::string m_descrption;
};


std::string test_results_to_string(std::vector<test_result_t> const& test_results, bool table);

void sort_test_results(std::vector<test_result_t> & test_results);

#endif /* h-guard */
