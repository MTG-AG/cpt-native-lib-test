
#ifndef __parse_test_spec__H_
#define __parse_test_spec__H_

#include "test_params.h"

typedef struct
{
  expected_verify_result_t expected_verify_result;
  test_severity_t severity; 
  std::string description;
} test_result_spec_t;

test_result_spec_t parse_test_result_spec(std::string const& test_case_dir);


#endif /* h-guard */
