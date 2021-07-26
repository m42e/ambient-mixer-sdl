#ifndef AMBIENTMIXER_PRESET_PARSER_H_
#define AMBIENTMIXER_PRESET_PARSER_H_
#include <SDL2/SDL_mixer.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "channel.h"
#include "randomunit.h"
#include <future>
#include <memory>
#include <vector>

namespace ambientmixer {

class Preset : public std::vector<Channel> {
 protected:
  void SetupChannel(std::string name, xmlNode* channel_node);
  void SetupTemplate(xmlNode* doc);
  int template_;

 public:
  Preset();
  static std::shared_ptr<Preset> createFromXml(xmlNode* doc);
  std::future<bool> Load();
  int GetTemplateId(){return template_;}
  ~Preset();
};

}  // namespace ambientmixer
#endif  // AMBIENTMIXER_PRESET_PARSER_H_
