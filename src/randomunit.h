#ifndef AMBIENTMIXER_RANDOMUNIT_H_
#define AMBIENTMIXER_RANDOMUNIT_H_
#include <map>
namespace ambientmixer {

enum class RandomUnit { _1h, _10m, _1m };
extern std::map<RandomUnit, int> randomUnitToSeconds;
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_RANDOMUNIT_H_
