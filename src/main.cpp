#include "prop_set.h"
#include "test_params.h"
#include "test_result.h"
#include "targets.h"
#include "exceptn.h"

#include <time.h>

#include <iostream>

using namespace std;
int main(
  int          argc,
  const char** argv
)
{
  srand(time(NULL));
  property_set_t cmdl_args(argc, argv);

  bool use_table_output = cmdl_args.get_as_bool_default_true("table");
  if(cmdl_args.have_index("test_cases_dir"))
  {
    vector<test_params_t> test_cases = construct_test_cases(cmdl_args.get_property_as_string("test_cases_dir"));
    vector<test_result_t> test_results;
    for(test_params_t base_test : test_cases)
    {
      const unsigned nb_rnd_execs = 16;
      std::vector<test_result_t> test_results_for_single_test_case;
      for(unsigned i = 0; i < nb_rnd_execs; i++)
      {
        test_params_t test = base_test.get_copy_with_rndmzd_order();
        if(cmdl_args.get_property_as_string("target") == "openssl")
        {
          test_results_for_single_test_case.push_back(call_openssl(test, cmdl_args));
        }
        else if(cmdl_args.get_property_as_string("target") == "botan")
        {
          test_results_for_single_test_case.push_back(call_botan(test, cmdl_args));
        }
        else if(cmdl_args.get_property_as_string("target") == "mbedtls")
        {
          test_results_for_single_test_case.push_back(call_mbedtls(test, cmdl_args));
        }
        else
        {
          throw test_exceptn_t("invalid value for argument 'target'");
        }
      }
      test_results.push_back(test_result_t::evaluate_test_results_for_single_test_case(test_results_for_single_test_case));
    }
    sort_test_results(test_results);
    std::cout << test_results_to_string(test_results, use_table_output) << std::endl;
  }
  else
  {
    std::cerr << "either single_test_dir or test_cases_dir must be specified" << std::endl;
  }
  return 0;
} // main
