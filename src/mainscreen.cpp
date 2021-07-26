#include "mainscreen.h"

#include "spdlog/spdlog.h"

namespace ambientmixer {

MainScreen::MainScreen(int width, int height) {
  screenSizeX_ = width;
  screenSizeY_ = height;
  SetupButtons();
}

void MainScreen::SetupButtons() {
  int bw = ((screenSizeX_ - border_) / 4) - border_;
  int bh = ((screenSizeY_ - border_) / 3) - border_;
  spdlog::info("Setting w {} h {} for button", bw, bh);
  int button_id = 0;

  for (int posy = border_; posy < screenSizeY_-border_; posy += bh + border_) {
    for (int posx = border_; posx < screenSizeX_-border_; posx += bw + border_) {
      spdlog::info("Adding Button w {} h {} for button at {}, {}", bw, bh, posx, posy);
      button_id++;
      push_back(std::make_shared<Button>(
          Button{{posx, posy, bw, bh}, {100, 100, 100, 0}, button_id}));
    }
  }

}  // namespace ambientmixer
void MainScreen::Render(SDL_Renderer *renderer) {
  for (auto &button : *this) {
    button->Render(renderer);
  }
}

void MainScreen::CheckTouch(SDL_Point tp, SDL_Event &event) {
  for (auto &button : *this) {
    if (button->IsTouched(tp)) {
      button->Action(event);
    }
  }
}  // namespace ambientmixer
}  // namespace ambientmixer
