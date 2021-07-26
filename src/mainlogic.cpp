#include "mainlogic.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include "spdlog/spdlog.h"

namespace ambientmixer {
void MainLogic::Render() {
  // Clear the window and make it all green
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

  ms->Render(renderer);

  // Change color to green
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

  // Render the changes above
  SDL_RenderPresent(renderer);
}

bool MainLogic::InitEverything() {
  if (!InitSDL()) return false;
  if (!CreateWindow()) return false;
  if (!CreateRenderer()) return false;
  if (!InitAudio()) return false;
  SetupRenderer();
  return true;
}

bool MainLogic::InitSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    spdlog::error("Failed to initialize SDL: {}", SDL_GetError());
    return false;
  }
  return true;
}

bool MainLogic::CreateWindow() {
  window = SDL_CreateWindow("Server", posX, posY, sizeX, sizeY, 0);
  if (window == nullptr) {
    spdlog::error("Failed to create Window : {}", SDL_GetError());
    return false;
  }
  return true;
}

bool MainLogic::CreateRenderer() {
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == nullptr) {
    spdlog::error("Failed to create renderer : {}", SDL_GetError());
    return false;
  }
  return true;
}

bool MainLogic::InitAudio() {
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) !=
      0) {
    spdlog::error("Unable to initialize audio: {}", SDL_GetError());
    return false;
  }
  Mix_AllocateChannels(128);

  Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD);
  return true;
}

void MainLogic::SetupRenderer() {
  // Set size of renderer to the same as window
  SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
  // Set color of renderer to green
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
}

void MainLogic::CheckTouch(SDL_Event &event) {
  SDL_Point tp;
  spdlog::info("EventType {}, {}", event.type, SDL_MOUSEBUTTONDOWN);
  if (event.type == SDL_FINGERDOWN ||
      event.type == event.type == SDL_FINGERMOTION) {
    tp.x = event.tfinger.y * sizeX;
    tp.y = sizeY - event.tfinger.x * sizeY;
  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    spdlog::info("Position mouse: {}, {}", event.button.x, event.button.y);
    tp.x = event.button.x;
    tp.y = event.button.y;
  }
  spdlog::info("Position: {}, {}", tp.x, tp.y);
  ms->CheckTouch(tp, event);
}

void MainLogic::RunGame() {
  while (loop) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        loop = false;
      else if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION) {
        CheckTouch(event);
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        CheckTouch(event);
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          default:
            break;
        }
      }
    }

    Render();

    // Add a 16msec delay to make our game run at ~60 fps
    SDL_Delay(16);
  }
}

void MainLogic::Start(int argc, char *argv[]){
  //spdlog::set_level(spdlog::level::debug);
  spdlog::info("Starting");

  if (!InitEverything()) return;

  ms = std::make_shared<MainScreen>(sizeX, sizeY);
  spdlog::info("Init Done");

  for (int i = 1; i < std::min(int(ms->size()), argc); i++) {
    spdlog::error("loading {} to {}", argv[i], i);
    std::thread([i, this](char* file){
      xmlDoc *doc = NULL;
      xmlNode *root_element = NULL;
      spdlog::error("loading {} to {}", file, i);
      doc = xmlReadFile(file, NULL, 0);

      if (doc == NULL) {
        spdlog::error("Could not read document {}", file);
      }else{
        /*Get the root element node */
        root_element = xmlDocGetRootElement(doc);
        auto p = Preset::createFromXml(root_element->children);
        xmlFreeDoc(doc);
        (*(this->ms))[i]->SetCallback([p, this](Button &b, SDL_Event &e) { 
            	      spdlog::info("play {}, {}, {}", b.button_id_, p->GetTemplateId(), b.imgpath_);
            	      spdlog::info("rect {}, {} -> {}x {}", b.rect_.x, b.rect_.y, b.rect_.w, b.rect_.h);
            	      this->ap->Play(p); 
          	      });
        char *fname = basename(file);
        fname[strlen(fname) - 4] = 0;
        (*(this->ms))[i]->SetImage("images/" + std::string(fname) + ".jpg");
        p->Load();
        spdlog::info("this was i:{} {}", i, file);
      }
    }, argv[i]).detach();
  }
  (*ms)[0]->SetCallback([&, this](Button &b, SDL_Event &e) { 
		  this->ap->Stop(); 
		  for (auto b : *(this->ms)){
			spdlog::info("this a {} {}", b->imgpath_, static_cast<bool>(b->action_));
		  }
		  });
  (*(this->ms))[0]->SetImage("images/stop.jpg");
  ap = std::make_shared<AmbientPlayer>();

  spdlog::info("Entering mainloop");
  RunGame();
}

void MainLogic::Stop(){
  ap->Stop();
  Mix_HaltChannel(-1);
  loop = false;
}
}
