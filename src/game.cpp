#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <signal.h>

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include "mainlogic.h"

using namespace ambientmixer;

std::shared_ptr<MainLogic> ml;

void IntHandler(int sig) {
  switch (sig) {
    case SIGINT:
      ml->Stop();
      break;
  }
}

int main(int argc, char *argv[]) {
  ml = std::make_shared<MainLogic>();
  signal(SIGINT, IntHandler);
  ml->Start(argc, argv);
}
