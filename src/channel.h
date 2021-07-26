#ifndef AMBIENTMIXER_CHANNEL_H_
#define AMBIENTMIXER_CHANNEL_H_
#include <SDL2/SDL_mixer.h>
#include <string>
#include "randomunit.h"
namespace ambientmixer {
struct Channel {
  int id;
  int channel_id;
  std::string name;
  std::string filename;
  bool mute;
  int volume;
  int balance;
  bool random;
  int random_count;
  RandomUnit random_unit;
  bool crossfade;
  Mix_Chunk *chunk;
};
} // namespace ambientmixer
#endif // AMBIENTMIXER_CHANNEL_H_
