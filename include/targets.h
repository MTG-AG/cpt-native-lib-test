
#ifndef __targets__H_
#define __targets__H_


test_result_t call_openssl(test_params_t const& test_param, property_set_t const& cmdl_args);
test_result_t call_botan(test_params_t const& test_param, property_set_t const& cmdl_args);
test_result_t call_mbedtls(test_params_t const& test_param, property_set_t const& cmdl_args);


#endif /* h-guard */
