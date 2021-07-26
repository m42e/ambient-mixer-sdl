#ifndef AMBIENTMIXER_AMBIENTPLAYER_H_
#define AMBIENTMIXER_AMBIENTPLAYER_H_
#include <SDL2/SDL_mixer.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <thread>
#include <vector>

#include "preset_parser.h"
#include "playerthread.h"
#include "randomtrigger.h"
#include "randomunit.h"

namespace ambientmixer {

class AmbientPlayer {
 private:
  std::shared_ptr<PlayerThread> pp_;
  bool playing_;
  bool run_;
  std::thread playingThread_;
  void Main(std::shared_ptr<Preset> p);
  void SetupChannels();
  void Playing();
  void StopChannels();
  std::vector<int> AllocateChannels(int chansNeeded);
  void DeallocateChannels(std::vector<int> channels);
  std::mutex channelAllocation_;
  std::mutex playerControl_;
  std::set<int> sdlChannelsInUse_;
  int sdlChannelsCount_;

 public:
  AmbientPlayer(int channels = 128);
  ~AmbientPlayer();
  void Play(std::shared_ptr<Preset> preset);
  void Stop();
  bool IsPlaying();
};
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_AMBIENTPLAYER_H_
