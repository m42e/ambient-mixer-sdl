#include "randomtrigger.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "randomunit.h"

namespace ambientmixer {
RandomTrigger::RandomTrigger(RandomUnit u, int count, int seed = 0)
    : count_(count), gen_(seed) {
  int factor = randomUnitToSeconds[u];
  dis_ = std::uniform_int_distribution<>(0, factor);
  AddNewTriggers();
}
void RandomTrigger::AddNewTriggers() {
  for (int i = 0; i < count_; i++) {
    int secondOffset = dis_(gen_);
    currentTriggers_.emplace_back(std::chrono::system_clock::now() +
                                  std::chrono::seconds(secondOffset));
  }
  std::sort(currentTriggers_.begin(), currentTriggers_.end());
}
std::chrono::system_clock::time_point RandomTrigger::GetNextTrigger() {
  if (currentTriggers_.size() == 0) AddNewTriggers();
  return currentTriggers_.front();
}
void RandomTrigger::RemoveTrigger() { currentTriggers_.pop_front(); }
}  // namespace ambientmixer
