#ifndef AMBIENTMIXER_BUTTON_H_
#define AMBIENTMIXER_BUTTON_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <functional>
#include <string>

namespace ambientmixer {

class Button {
  using ActionCallback = std::function<void(Button &, SDL_Event &)>;

 private:
 public:
  SDL_Rect rect_;
  SDL_Texture* texture_;
  std::string imgpath_;
  bool textureLoaded_;
  ActionCallback action_;
  int button_id_;

 protected:
  SDL_Color color_;

 public:
  Button(SDL_Rect btn, SDL_Color, int button_id);
  virtual void Render(SDL_Renderer *renderer);

  void SetImage(std::string path);
  void SetCallback(ActionCallback cb);
  bool IsTouched(SDL_Point &tp);
  virtual void Action(SDL_Event &event);
};
}  // namespace ambientmixer
#endif  // AMBIENTMIXER_BUTTON_H_
