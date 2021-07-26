#ifndef AMBIENTMIXER_MAINSCREEN_H_
#define AMBIENTMIXER_MAINSCREEN_H_
#include <SDL2/SDL.h>

#include <functional>
#include <vector>

#include "button.h"
#include "preset_parser.h"

namespace ambientmixer {
class MainScreen : public std::vector<std::shared_ptr<Button>> {
  int screenSizeX_;
  int screenSizeY_;
  int border_ = 8;
  void SetupButtons();

 public:
  MainScreen(int width, int height);
  void Render(SDL_Renderer *renderer);
  void CheckTouch(SDL_Point tp, SDL_Event& event);
};
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_MAINSCREEN_H_
