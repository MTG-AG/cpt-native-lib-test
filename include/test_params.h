
#ifndef __test_params__H_
#define __test_params__H_

#include <vector>
#include <string>
#include <stdint.h>
#include "read_crls.h"

typedef enum { severity_error, severity_warning, severity_out_of_scope } test_severity_t;

typedef enum { expected_to_verify_unspecified, expected_to_verify_ok, expected_to_verify_fail } expected_verify_result_t;

typedef enum { pem_encoding, bin_encoding } der_encoding_type;

std::string expected_verify_result_to_string(expected_verify_result_t exp_res);

class test_params_t
{

  public:
    test_params_t(std::string const& dir);

    std::vector<uint8_t> const& get_target_cert(der_encoding_type enc = bin_encoding) const { if(enc == bin_encoding) {return m_target_cert;} else {return m_target_cert_pem; }  };

    std::vector<std::vector<uint8_t>> const& get_ca_certs(der_encoding_type enc = bin_encoding) const { if(enc == bin_encoding) {return m_ca_certs;} else {return m_ca_certs_pem;} };

    std::vector<std::vector<uint8_t>> const& get_trust_anchors(der_encoding_type enc = bin_encoding) const  { if(enc == bin_encoding) {return m_trust_anchors;} else {return m_trust_anchors_pem;} };

    std::string const& get_name() const { return m_test_name; };
    std::string const& get_description() const { return m_description; };
    test_severity_t const& get_severity() const { return m_severity; };

    expected_verify_result_t get_expected_verify_result() const { return m_expected_verify_result; };

    crl_params_t const& get_crl_params() const { return m_crl_params; };

  test_params_t get_copy_with_rndmzd_order() const;


  private:
    std::string m_test_name;
    expected_verify_result_t m_expected_verify_result;
    test_severity_t m_severity;
    std::vector<std::vector<uint8_t>> m_ca_certs;
    std::vector<std::vector<uint8_t>> m_ca_certs_pem;
    std::vector<std::vector<uint8_t>> m_trust_anchors;
    std::vector<std::vector<uint8_t>> m_trust_anchors_pem;
    std::vector<uint8_t> m_target_cert;
    std::vector<uint8_t> m_target_cert_pem;
    std::string m_description;
    crl_params_t m_crl_params;
};

std::vector<test_params_t> construct_test_cases(std::string const& test_cases_dir);

#endif /* h-guard */
