
#include "playerthread.h"

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

PlayerThread::PlayerThread(std::shared_ptr<Preset> p)
    : channels_(p), playing_(true) {}

void PlayerThread::SetChannels(std::vector<int>&& sdlChannels) {
  sdlChannelsInUse_ = sdlChannels;
  for (auto c : sdlChannelsInUse_) {
    spdlog::info("got channel: {}", c);
  }
}

std::vector<int> PlayerThread::GetChannels() {
  return sdlChannelsInUse_;
}
int PlayerThread::MapChannelId(int channel_id) {
  if (channel_id <= 0 || channel_id > sdlChannelsInUse_.size()) {
    spdlog::error("Invalid channel id");
  }
  return sdlChannelsInUse_[channel_id - 1];
}

void PlayerThread::SetupChannels() {
  for (auto& c : *channels_) {
    spdlog::info("Channel {} {}", MapChannelId(c.channel_id), c.name);
    if (!c.random) {
      spdlog::debug("Fade in {}", MapChannelId(c.channel_id));
      Mix_FadeInChannel(MapChannelId(c.channel_id), c.chunk, -1, 1500);
    } else {
      spdlog::debug("Random {}", MapChannelId(c.channel_id));
      randommap_[c.channel_id] =
          std::make_shared<RandomTrigger>(c.random_unit, c.random_count, c.id);
      activations_[randommap_[c.channel_id]->GetNextTrigger()] = c.channel_id;
    }
    int angle = int(270 + ((c.balance + 100) / 100.0) * 90) % 360;
    spdlog::debug("Set angle to {} for channel {}", angle, c.channel_id);
    Mix_SetPosition(MapChannelId(c.channel_id), angle, 0);
  }
  spdlog::debug("Setup of Channels done");
}

void PlayerThread::Run() {
  SetupChannels();
  Playing();
  StopChannels();
}

void PlayerThread::Stop() {
  spdlog::info("Stop Thread");
  playing_ = false;
}

void PlayerThread::Playing() {
  while (playing_) {
    if (activations_.size() == 0 ||
        (*(activations_.begin())).first >
            std::chrono::system_clock::now() + std::chrono::seconds(3)) {
      activations_[std::chrono::system_clock::now() + std::chrono::seconds(3)] =
          -1;
    }
    auto x = *(activations_.begin());
    std::this_thread::sleep_until(x.first);
    activations_.erase(activations_.begin());
    if (x.second == -1) {
      spdlog::debug("wakup check, playing: {} template {}", playing_, channels_->GetTemplateId());
      continue;
    }
    int channel_id = x.second;
    auto v = randommap_[channel_id];
    v->RemoveTrigger();
    activations_[v->GetNextTrigger()] = x.second;
    if (Mix_Playing(MapChannelId(channel_id)) == 0) {
      spdlog::info("Trigger {}", channel_id);
      Mix_PlayChannel(MapChannelId(channel_id),
                      (*channels_)[channel_id - 1].chunk, 0);
    }
    spdlog::info("sleep till {}",
                 (x.first.time_since_epoch() -
                  std::chrono::system_clock::now().time_since_epoch())
                     .count());
  }
}
void PlayerThread::StopChannels() {
  spdlog::info("Fade out channels");
  activations_.clear();
  for (auto c : sdlChannelsInUse_) {
    spdlog::info("Fade out channel {}", c);
    Mix_FadeOutChannel(c, 4000);
  }
  spdlog::info("All channels stopped playing");
#if 0
  while(Mix_Playing(-1)){
    for (auto c: sdlChannelsInUse_){
      if(Mix_Playing(c)){
        spdlog::debug("Playing {}", c);
      }
    }
  }
#endif
}

bool PlayerThread::IsPlaying() { return playing_; }
PlayerThread::~PlayerThread() { spdlog::info("Stopped PlayerThread"); }

}  // namespace ambientmixer
