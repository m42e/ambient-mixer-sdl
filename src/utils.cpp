
#include <cstdlib>
#include <string>
#include "utils.h"

// namespaces ambientmixer::utils
namespace ambientmixer {
namespace utils {
std::string GetEnv(const std::string& variable_name,
                   const std::string& default_value) {
  const char* value = getenv(variable_name.c_str());
  return value ? value : default_value;
}

}  // namespace utils
}  // namespace ambientmixer
