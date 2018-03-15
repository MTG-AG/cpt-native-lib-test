#include <fstream>
#include <vector>
#include "readdir.h"
#include "exceptn.h"
#include <iostream>

/* linux only ==> */
#include <sys/types.h>
#include <dirent.h>
/* <== linux only */



static bool string_ends_with(
  std::string const &fullString,
  std::string const &ending
)
{
  if(fullString.length() >= ending.length())
  {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  else
  {
    return false;
  }
}

std::vector<std::string> get_entries_of_dir(
  std::string const        & dir_name,
  dir_entry_extract_mode_t extr_mode,
  std::string const        & postfix,
  bool get_only_entries_that_are_dirs,
  std::string const       & exclude_postfix,
  std::string const        & prefix
)
{
  std::vector<std::string> result;
  DIR* dir;
  struct dirent* ent;
  if((dir = opendir(dir_name.c_str())) != NULL)
  {
    while((ent = readdir(dir)) != NULL)
    {
      if(get_only_entries_that_are_dirs && (ent->d_type != DT_DIR))
      {
        continue;
      }
      std::string s(ent->d_name);
      if(s.find(".") == 0)
      {
        continue;
      }
      if((prefix != "") && s.find(prefix) != 0)
      {
        continue;
      }
      if((postfix != "") && !string_ends_with(s, postfix))
      {
        continue;
      }
      if((exclude_postfix != "") && string_ends_with(s, exclude_postfix))
      {
        continue;
      }
      std::string prefix = "";
      if(extr_mode == dir_entries_with_path)
      {
        prefix = dir_name + "/";
      }
      result.push_back(prefix + s);
    }
    closedir(dir);
  }
  else
  {
    /* could not open directory */
  }
  return result;
} // get_entries_of_dir
