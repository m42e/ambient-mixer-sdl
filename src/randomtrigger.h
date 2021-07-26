#ifndef AMBIENTMIXER_RANDOMTRIGGER_H_
#define AMBIENTMIXER_RANDOMTRIGGER_H_
#include <chrono>
#include <deque>
#include <random>

#include "randomunit.h"

namespace ambientmixer {

class RandomTrigger {
 private:
  int count_;
  std::chrono::seconds factor_;
  std::mt19937 gen_;
  void AddNewTriggers();
  std::uniform_int_distribution<> dis_;

 public:
  RandomTrigger(RandomUnit u, int count, int seed);
  std::chrono::system_clock::time_point GetNextTrigger();
  void RemoveTrigger();
  std::deque<std::chrono::system_clock::time_point> currentTriggers_;
};
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_RANDOMTRIGGER_H_
