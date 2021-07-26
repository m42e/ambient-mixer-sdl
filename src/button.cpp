#include "button.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "spdlog/spdlog.h"

#include <iostream>

namespace ambientmixer {
void Button::Action(SDL_Event &event) {
  if (action_) action_(*this, event);
}

Button::Button(SDL_Rect btn, SDL_Color color = {255, 255, 255, 0}, int button_id = 0)
    : rect_(btn), color_(color), texture_(nullptr), textureLoaded_(false), button_id_{button_id} {}

void Button::Render(SDL_Renderer *renderer) {
  if (!textureLoaded_ && (texture_ == nullptr && !imgpath_.empty())){
    spdlog::info("load texture {}", imgpath_);
    SDL_Surface* surf = IMG_Load(imgpath_.c_str());
    texture_ = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    textureLoaded_ = true;
  }
  if(texture_ != nullptr){
    SDL_RenderCopy(renderer, texture_, NULL, &rect_);
  }else{
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect_);
  }
}
void Button::SetCallback(ActionCallback cb) { action_ = cb; }
void Button::SetImage(std::string path) { imgpath_ = path; }

bool Button::IsTouched(SDL_Point &tp) {
  if ((tp.x > rect_.x && tp.x < rect_.x + rect_.w) &&
      (tp.y > rect_.y && tp.y < rect_.y + rect_.h))
    return true;
  return false;
}
}  // namespace ambientmixer
