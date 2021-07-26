
#include "randomunit.h"

namespace ambientmixer {
std::map<RandomUnit, int> randomUnitToSeconds{
    {RandomUnit::_1h, 3600},
    {RandomUnit::_10m, 300},
    {RandomUnit::_1m, 60},
};
}  // namespace ambientmixer
