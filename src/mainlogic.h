#ifndef AMBIENTMIXER_MAINLOGIC_H_
#define AMBIENTMIXER_MAINLOGIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <libgen.h>

#include <memory>
#include <vector>

#include "ambientplayer.h"
#include "mainscreen.h"
#include "preset_parser.h"

namespace ambientmixer {

class MainLogic {
 private:
  bool loop = true;
  int posX = 0;
  int posY = 0;
  int sizeX = 800;
  int sizeY = 480;

  SDL_Window *window;
  SDL_Renderer *renderer;

  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 2048;

  std::shared_ptr<AmbientPlayer> ap;
  std::shared_ptr<MainScreen> ms;

  bool InitEverything();
  bool InitSDL();
  bool InitAudio();
  bool CreateWindow();
  bool CreateRenderer();
  void SetupRenderer();
  void CheckTouch(SDL_Event &event);

  void Render();
  void RunGame();

 public:
  void Start(int argc, char *argv[]);
  void Stop();
};
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_MAINLOGIC_H_
