#include "test_result.h"
#include "prop_set.h"
#include "targets.h"
#include <vector>
#include "mbedtls/x509_crt.h"
#include <cstring>
#include <iostream>

using namespace std;


int dummy_verify(
  void*,
  mbedtls_x509_crt*,
  int,
  uint32_t*
)
{
  return 0;
}

test_result_t call_mbedtls(
  test_params_t const & test_param,
  property_set_t const& // cmdl_args
)
{
  string error_string;
  verification_result_t ver_res = verification_result_ok;
  mbedtls_x509_crt cert_chain;

  memset(&cert_chain, 0, sizeof(cert_chain));
  mbedtls_x509_crt trusted;
  memset(&trusted, 0, sizeof(trusted));
  mbedtls_x509_crl crls;
  memset(&crls, 0, sizeof(crls));

  std::vector<uint8_t> const& target_cert_vec = test_param.get_target_cert(bin_encoding);
  int err = mbedtls_x509_crt_parse_der(&cert_chain, &target_cert_vec[0], target_cert_vec.size());
  if(err)
  {
    ver_res      = verification_result_decoding_error;
    error_string = "unspecified decoding error for target cert";
  }
  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_chain_cert : test_param.get_ca_certs(bin_encoding))
    {
      err = mbedtls_x509_crt_parse_der(&cert_chain, &enc_chain_cert[0], enc_chain_cert.size());
      if(err)
      {
        ver_res      = verification_result_decoding_error;
        error_string = "unspecified decoding error for CA cert";
        break;
      }
    }
  }
  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_crl : test_param.get_crl_params().crl_data)
    {
      err = mbedtls_x509_crl_parse(&crls, &enc_crl[0], enc_crl.size());
      if(err)
      {
        ver_res      = verification_result_decoding_error;
        error_string = "unspecified decoding error for CRL: " + num_to_string(err);
        break;
      }
    }
  }
  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_ta : test_param.get_trust_anchors(bin_encoding))
    {
      err = mbedtls_x509_crt_parse_der(&trusted, &enc_ta[0], enc_ta.size());
      if(err)
      {
        ver_res      = verification_result_decoding_error;
        error_string = "unspecified decoding error for trust anchor cert";
        break;
      }
    }
  }

  if(ver_res == verification_result_ok)
  {
    uint32_t result_flags = 0;

    err = mbedtls_x509_crt_verify(
      &cert_chain,
      &trusted,
      &crls,
      NULL,
      &result_flags,
      &dummy_verify,
      NULL
      );
    if(err)
    {
      ver_res = verification_result_appl_spec_error;
      char buf[4000];
      mbedtls_x509_crt_verify_info(buf, sizeof(buf), "", result_flags);
      error_string = string(buf);
    }
  }

  mbedtls_x509_crt_free(&trusted);
  mbedtls_x509_crt_free(&cert_chain);

  mbedtls_x509_crl_free(&crls);
  return test_result_t(test_param, ver_res, error_string);
} // call_mbedtls
