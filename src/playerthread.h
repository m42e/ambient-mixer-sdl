#ifndef AMBIENTMIXER_PLAYERTHREAD_H_
#define AMBIENTMIXER_PLAYERTHREAD_H_
#include <SDL2/SDL_mixer.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <atomic>
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
#include "randomtrigger.h"
#include "randomunit.h"

namespace ambientmixer {
  class PlayerThread {
    std::shared_ptr<Preset> channels_;
    std::vector<int> sdlChannelsInUse_;
    std::map<std::chrono::system_clock::time_point, int> activations_;
    std::map<int, std::shared_ptr<RandomTrigger>> randommap_;
    void SetupChannels();
    void Playing();
    void StopChannels();
    int MapChannelId(int channel_id);
    std::atomic_bool playing_;

   public:
    PlayerThread(std::shared_ptr<Preset> p);
    ~PlayerThread();
    void SetChannels(std::vector<int>&& sdlChannels);
    void Run();
    void Stop();
    bool IsPlaying();
    std::vector<int> GetChannels();
  };
}
#endif  // AMBIENTMIXER_PLAYERTHREAD_H_
