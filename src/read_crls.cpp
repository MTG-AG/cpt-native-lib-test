#include "read_crls.h"
#include "readdir.h"
#include "util.h"
#include "exceptn.h"
#include <iostream>

#include <string>

using namespace std;

crl_params_t read_crls_from_testcase_dir(std::string const& test_case_dir)
{
  vector<string> crl_dir = get_entries_of_dir(test_case_dir, dir_entries_with_path, "crls");
  if(crl_dir.size() > 1)
  {
    throw test_exceptn_t("multiple crl dirs, this should not happen");
  }
  crl_params_t result;
  result.have_crls = (crl_dir.size() != 0);
  if(result.have_crls)
  {
    vector<string> crl_paths = get_entries_of_dir(crl_dir[0], dir_entries_with_path, ".crl", false, ".pem.crl");
    result.crl_data = file_contents_from_paths(crl_paths);
    
    vector<string> crl_paths_pem = get_entries_of_dir(crl_dir[0], dir_entries_with_path, ".pem.crl");
    result.crl_data_pem = file_contents_from_paths(crl_paths_pem);
  }

  return result;
}
