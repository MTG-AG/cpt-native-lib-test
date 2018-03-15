#include "botan/x509path.h"
#include "test_result.h"
#include "prop_set.h"
#include "targets.h"
#include <vector>

using namespace std;
using namespace Botan;

test_result_t call_botan(
  test_params_t const & test_param,
  property_set_t const& 
)
{
  Botan::Certificate_Store_In_Memory store;
  verification_result_t ver_res = verification_result_ok;
  std::vector<uint8_t> const& target_cert_vec = test_param.get_target_cert(pem_encoding);
  vector<X509_Certificate> cert_chain;
  string error_string;
  try
  {
    Botan::X509_Certificate target_cert(target_cert_vec);
    cert_chain.push_back(target_cert);
  }
  catch(std::exception const& e)
  {
    ver_res      = verification_result_decoding_error;
    error_string = e.what();
  }

  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_chain_cert : test_param.get_ca_certs(pem_encoding))
    {
      try
      {
        Botan::X509_Certificate cert(enc_chain_cert);
        cert_chain.push_back(cert);
      } catch(std::exception const& e)
      {
        ver_res      = verification_result_decoding_error;
        error_string = e.what();
        break;
      }
    }
  }

  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_ta : test_param.get_trust_anchors(pem_encoding))
    {
      try
      {
        X509_Certificate cert(enc_ta);
        store.add_certificate(cert);
      }
      catch(std::exception const& e)
      {
        ver_res      = verification_result_decoding_error;
        error_string = e.what();
        break;
      }
    }
  }
  if(ver_res == verification_result_ok)
  {
    for(vector<uint8_t> enc_crl : test_param.get_crl_params().crl_data)
    {
      try
      {
        X509_CRL crl(enc_crl);
        store.add_crl(crl);
      }
      catch(std::exception const& e)
      {
        ver_res      = verification_result_decoding_error;
        error_string = e.what();
        break;
      }
    }
  }

  if(ver_res == verification_result_ok)
  {
    Path_Validation_Restrictions pvr(test_param.get_crl_params().have_crls,
      79,     // minimum strength, default would be 110
      test_param.get_crl_params().have_crls); 
    try
    {
      Path_Validation_Result path_ver_res = x509_path_validate(cert_chain, pvr, store);
      ver_res =
        path_ver_res.successful_validation() ? verification_result_ok : verification_result_appl_spec_error;
      error_string = path_ver_res.result_string();
    }
    catch(std::exception const& e)
    {
      ver_res      = verification_result_exception;
      error_string = e.what();
    }
  }
  else if(ver_res == verification_result_decoding_error)
  { }

  test_result_t result(test_param, ver_res, error_string);

  return result;

} // call_botan
