#ifndef AMBIENTMIXER_UTILS_H_
#define AMBIENTMIXER_UTILS_H_
#include <string>
namespace ambientmixer {
namespace utils {
std::string GetEnv(const std::string& variable_name,
                   const std::string& default_value);

}  // namespace utils
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_UTILS_H_
