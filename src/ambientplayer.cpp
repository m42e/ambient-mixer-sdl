#include "ambientplayer.h"

#include <SDL2/SDL_mixer.h>

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

#include "randomtrigger.h"
#include "randomunit.h"
#include "spdlog/spdlog.h"

namespace ambientmixer {

AmbientPlayer::AmbientPlayer(int channels)
    : run_(true), playing_(false), sdlChannelsCount_(channels) {}

void AmbientPlayer::Main(std::shared_ptr<Preset> preset) {
  std::stringstream tid;
  tid << std::this_thread::get_id();

  spdlog::info("Playing (Thread {})", tid.str());
  std::shared_ptr<PlayerThread> my_player = std::make_shared<PlayerThread>(preset);
  {
    std::lock_guard<std::mutex> lck{playerControl_};
    pp_ = my_player;
  }
  my_player->SetChannels(AllocateChannels(preset->size()));
  my_player->Run();
  DeallocateChannels(my_player->GetChannels());
  spdlog::debug("Stopped playing & Exiting (Thread {})", tid.str());
}

std::vector<int> AmbientPlayer::AllocateChannels(int chansNeeded) {
  std::lock_guard<std::mutex> lck(channelAllocation_);
  std::vector<int> available_channels;
  for (int i = 0;
       i < sdlChannelsCount_ && available_channels.size() < chansNeeded; i++) {
    if (sdlChannelsInUse_.count(i) == 0) {
      sdlChannelsInUse_.insert(i);
      available_channels.push_back(i);
    }
  }
  return available_channels;
}
void AmbientPlayer::DeallocateChannels(std::vector<int> channels) {
  std::lock_guard<std::mutex> lck(channelAllocation_);
  for (int i : channels) {
    sdlChannelsInUse_.erase(i);
  }
}

void AmbientPlayer::Play(std::shared_ptr<Preset> p) {
  {
    std::lock_guard<std::mutex> lck{playerControl_};
    if (pp_ && pp_->IsPlaying()) {
      pp_->Stop();
    }
  }
  if (playingThread_.joinable()) {
    playingThread_.detach();
  }
  playing_ = true;
  playingThread_ = std::thread(&AmbientPlayer::Main, this, p);
}

void AmbientPlayer::Stop() {
  spdlog::info("Stop");
  {
    std::lock_guard<std::mutex> lck{playerControl_};
    if (pp_) pp_->Stop();
  }
  playing_ = false;
}

bool AmbientPlayer::IsPlaying() { return playing_; }

AmbientPlayer::~AmbientPlayer() {
  Stop();
  run_ = false;
  if (playingThread_.joinable()) playingThread_.join();
}

}  // namespace ambientmixer
