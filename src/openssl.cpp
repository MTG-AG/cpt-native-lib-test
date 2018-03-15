#include "test_params.h"
#include "openssl/x509.h"
#include "exceptn.h"
#include "test_result.h"
#include "util.h"
#include <iostream>
#include "prop_set.h"

using namespace std;
const std::string ossl_ref_version("OpenSSL 1.1.0  25 Aug 2016");

#define OSSL_CALL(__f) if(1 != __f) {throw test_exceptn_t("error with ossl call " + #__f);}


typedef struct
{
  const char* s;
  unsigned    c;
} err_string_code_map_entry_t;

static err_string_code_map_entry_t ossl_err_map [] = {
  {"X509_V_OK",                                        0},
  {"X509_V_ERR_UNSPECIFIED",                           1},
  {"X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT",             2},
  {"X509_V_ERR_UNABLE_TO_GET_CRL",                     3},
  {"X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE",      4},
  {"X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE",       5},
  {"X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY",    6},
  {"X509_V_ERR_CERT_SIGNATURE_FAILURE",                7},
  {"X509_V_ERR_CRL_SIGNATURE_FAILURE",                 8},
  {"X509_V_ERR_CERT_NOT_YET_VALID",                    9},
  {"X509_V_ERR_CERT_HAS_EXPIRED",                     10},
  {"X509_V_ERR_CRL_NOT_YET_VALID",                    11},
  {"X509_V_ERR_CRL_HAS_EXPIRED",                      12},
  {"X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD",       13},
  {"X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD",        14},
  {"X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD",       15},
  {"X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD",       16},
  {"X509_V_ERR_OUT_OF_MEM",                           17},
  {"X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT",          18},
  {"X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN",            19},
  {"X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY",    20},
  {"X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE",      21},
  {"X509_V_ERR_CERT_CHAIN_TOO_LONG",                  22},
  {"X509_V_ERR_CERT_REVOKED",                         23},
  {"X509_V_ERR_INVALID_CA",                           24},
  {"X509_V_ERR_PATH_LENGTH_EXCEEDED",                 25},
  {"X509_V_ERR_INVALID_PURPOSE",                      26},
  {"X509_V_ERR_CERT_UNTRUSTED",                       27},
  {"X509_V_ERR_CERT_REJECTED",                        28},
  {"X509_V_ERR_SUBJECT_ISSUER_MISMATCH",              29},
  {"X509_V_ERR_AKID_SKID_MISMATCH",                   30},
  {"X509_V_ERR_AKID_ISSUER_SERIAL_MISMATCH",          31},
  {"X509_V_ERR_KEYUSAGE_NO_CERTSIGN",                 32},
  {"X509_V_ERR_UNABLE_TO_GET_CRL_ISSUER",             33},
  {"X509_V_ERR_UNHANDLED_CRITICAL_EXTENSION",         34},
  {"X509_V_ERR_KEYUSAGE_NO_CRL_SIGN",                 35},
  {"X509_V_ERR_UNHANDLED_CRITICAL_CRL_EXTENSION",     36},
  {"X509_V_ERR_INVALID_NON_CA",                       37},
  {"X509_V_ERR_PROXY_PATH_LENGTH_EXCEEDED",           38},
  {"X509_V_ERR_KEYUSAGE_NO_DIGITAL_SIGNATURE",        39},
  {"X509_V_ERR_PROXY_CERTIFICATES_NOT_ALLOWED",       40},
  {"X509_V_ERR_INVALID_EXTENSION",                    41},
  {"X509_V_ERR_INVALID_POLICY_EXTENSION",             42},
  {"X509_V_ERR_NO_EXPLICIT_POLICY",                   43},
  {"X509_V_ERR_DIFFERENT_CRL_SCOPE",                  44},
  {"X509_V_ERR_UNSUPPORTED_EXTENSION_FEATURE",        45},
  {"X509_V_ERR_UNNESTED_RESOURCE",                    46},
  {"X509_V_ERR_PERMITTED_VIOLATION",                  47},
  {"X509_V_ERR_EXCLUDED_VIOLATION",                   48},
  {"X509_V_ERR_SUBTREE_MINMAX",                       49},
  {"X509_V_ERR_APPLICATION_VERIFICATION",             50},
  {"X509_V_ERR_UNSUPPORTED_CONSTRAINT_TYPE",          51},
  {"X509_V_ERR_UNSUPPORTED_CONSTRAINT_SYNTAX",        52},
  {"X509_V_ERR_UNSUPPORTED_NAME_SYNTAX",              53},
  {"X509_V_ERR_CRL_PATH_VALIDATION_ERROR",            54},
  {"X509_V_ERR_PATH_LOOP",                            55},
  {"X509_V_ERR_SUITE_B_INVALID_VERSION",              56},
  {"X509_V_ERR_SUITE_B_INVALID_ALGORITHM",            57},
  {"X509_V_ERR_SUITE_B_INVALID_CURVE",                58},
  {"X509_V_ERR_SUITE_B_INVALID_SIGNATURE_ALGORITHM",  59},
  {"X509_V_ERR_SUITE_B_LOS_NOT_ALLOWED",              60},
  {"X509_V_ERR_SUITE_B_CANNOT_SIGN_P_384_WITH_P_256", 61},
  {"X509_V_ERR_HOSTNAME_MISMATCH",                    62},
  {"X509_V_ERR_EMAIL_MISMATCH",                       63},
  {"X509_V_ERR_IP_ADDRESS_MISMATCH",                  64},
  {"X509_V_ERR_DANE_NO_MATCH",                        65},
  {"X509_V_ERR_EE_KEY_TOO_SMALL",                     66},
  {"X509_V_ERR_CA_KEY_TOO_SMALL",                     67},
  {"X509_V_ERR_CA_MD_TOO_WEAK",                       68},
  {"X509_V_ERR_INVALID_CALL",                         69},
  {"X509_V_ERR_STORE_LOOKUP",                         70},
  {"X509_V_ERR_NO_VALID_SCTS",                        71},
  {"X509_V_ERR_PROXY_SUBJECT_NAME_VIOLATION",         72}
};

static std::string openssl_err_code_to_string(unsigned err_code)
{
  for(unsigned i = 0; i < sizeof(ossl_err_map) / sizeof(ossl_err_map[0]); i++)
  {
    if(err_code == ossl_err_map[i].c)
    {
      return std::string(ossl_err_map[i].s);
    }
  }
  return std::string("unmatched error code " + num_to_string(err_code));
}

test_result_t call_openssl(test_params_t const& test_param, property_set_t const& cmdl_args)
{
  verification_result_t ver_result = verification_result_ok;
  string error_string = "";

  std::string ossl_version(OPENSSL_VERSION_TEXT);
  if(ossl_version != ossl_ref_version)
  {
    throw test_exceptn_t("wrong OpenSSL version: '" + ossl_version + "' instead of '" + ossl_ref_version + "'");
  }
  X509_STORE_CTX* ctx = X509_STORE_CTX_new();
  X509_STORE* store;
  store = X509_STORE_new();
  if(!ctx || !store)
  {
    throw test_exceptn_t("error allocating openssl objects");
  }


  vector<uint8_t> const& target_cert_vec = test_param.get_target_cert();
  const uint8_t* data   = &target_cert_vec[0];
  X509* target_cert_obj = d2i_X509(NULL, &data, target_cert_vec.size());

  if(!target_cert_obj)
  {
    ver_result = verification_result_decoding_error;
  }

  if(ver_result == verification_result_ok)
  {
    vector<vector<uint8_t> > const& trust_anchors = test_param.get_trust_anchors();
    for(vector<uint8_t> const& ta_vec : trust_anchors)
    {
      const uint8_t* data = &ta_vec[0];
      X509* cert = d2i_X509(NULL, &data, ta_vec.size());

      if(!cert)
      {
        ver_result = verification_result_decoding_error;
        break;
      }
      else
      {
        X509_STORE_add_cert(store, cert);
        X509_free(cert);
      }
    }
  }

  STACK_OF(X509) * untrusted = sk_X509_new_null();
  if(!untrusted)
  {
    throw test_exceptn_t("error creating OpenSSL stack object");
  }
  if(ver_result == verification_result_ok)
  {
    vector<vector<uint8_t> > const& ca_certs = test_param.get_ca_certs();
    for(vector<uint8_t> const& ca_cert : ca_certs)
    {
      const uint8_t* data = &ca_cert[0];
      X509* cert = d2i_X509(NULL, &data, ca_cert.size());
      if(!cert)
      {
        ver_result = verification_result_decoding_error;
        break;
      }
      else
      {
        sk_X509_push(untrusted, cert);
      }
    }
  }
  if(ver_result == verification_result_ok)
  {
     for(vector<uint8_t> bin_crl : test_param.get_crl_params().crl_data)
     {
       const uint8_t* data = &bin_crl[0];
        X509_CRL* crl = d2i_X509_CRL(NULL, &data, bin_crl.size());

      if(!crl)
      {
        ver_result = verification_result_decoding_error;
        break;
      }
      else
      {
        X509_STORE_add_crl(store, crl);
        X509_CRL_free(crl);
      }
     }
  }

  if(ver_result == verification_result_ok)
  {
    X509_STORE_CTX_set_chain(ctx, untrusted);
    if(true)
    {
      X509_STORE_set_flags(store, X509_V_FLAG_POLICY_CHECK);
    }
    if(test_param.get_crl_params().have_crls)
    {
        X509_STORE_set_flags(store, X509_V_FLAG_CRL_CHECK);
        X509_STORE_set_flags(store, X509_V_FLAG_CRL_CHECK_ALL);
    }

    if(X509_STORE_CTX_init(ctx, store, target_cert_obj, untrusted) != 1)
    {
      throw test_exceptn_t("error initializing ossl ctx");
    }
    int result = X509_verify_cert(ctx);

    if(result == 1)
    {
      ver_result = verification_result_ok;
    }
    else
    {
      ver_result = verification_result_appl_spec_error;

      error_string = openssl_err_code_to_string(X509_STORE_CTX_get_error(ctx));
    }
  }
  test_result_t result(test_param, ver_result, error_string);

  if(store) X509_STORE_free(store);
  if(ctx) X509_STORE_CTX_free(ctx);
  if(target_cert_obj) X509_free(target_cert_obj);
  if(untrusted)
  {
    while(sk_X509_num(untrusted))
    {
      X509* cert = sk_X509_pop(untrusted);
      X509_free(cert);
    }
    sk_X509_free(untrusted);
  }
  return result;
} // call_openssl
