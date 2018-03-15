#ifndef __readdir__H_
#define __readdir__H_


#include <vector>
#include <string>

typedef enum { dir_entries_with_path, dir_entries_only_leafs } dir_entry_extract_mode_t;

std::vector<std::string> get_entries_of_dir(
  std::string const        & dir_name,
  dir_entry_extract_mode_t extr_mode,
  std::string const        & postfix = "",
  bool                     get_only_entries_that_are_dirs = false,
  std::string const        & exclude_postfix = "",
  std::string const        & prefix = ""
);


#endif /* h-guard */
