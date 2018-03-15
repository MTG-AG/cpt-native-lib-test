
#ifndef __read_crls__H_
#define __read_crls__H_


#include <stdint.h>
#include <vector>
#include <string>
#include "util.h"

class crl_params_t
{
  public:
  std::vector<std::vector<uint8_t>> crl_data;
  std::vector<std::vector<uint8_t>> crl_data_pem;
  bool have_crls;

  crl_params_t get_copy_with_rndmzd_order() const
  {
    crl_params_t result(*this);
    result.crl_data = vector_randomized_copy(result.crl_data);
    result.crl_data_pem = vector_randomized_copy(result.crl_data_pem);
    return result;
  };
};

crl_params_t read_crls_from_testcase_dir(std::string const& test_case_dir);

#endif /* h-guard */
