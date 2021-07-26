#include "preset_parser.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <future>
#include <sstream>

#include "spdlog/spdlog.h"
#include "utils.h"
namespace ambientmixer {

std::shared_ptr<Preset> Preset::createFromXml(xmlNode *doc) {
  xmlNode *cur_node = NULL;

  std::shared_ptr<Preset> p = std::make_shared<Preset>();
  for (cur_node = doc; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      std::string name = reinterpret_cast<const char *>(cur_node->name);
      spdlog::debug("nodename {}", name);
      if (name.find("channel") == 0) p->SetupChannel(name, cur_node);
      if (name.find("id_template") == 0) p->SetupTemplate(cur_node);
    }
  }
  spdlog::info("Parsed xml file");
  return p;
}

template <typename T>
T get(xmlNode *cur_node) {
  return T{};
}

template <>
int get<int>(xmlNode *cur_node) {
  auto t = reinterpret_cast<const char *>(xmlNodeGetContent(cur_node));
  return std::atoi(t);
}

template <>
char get<char>(xmlNode *cur_node) {
  return reinterpret_cast<const char *>(xmlNodeGetContent(cur_node))[0];
}

template <>
std::string get<std::string>(xmlNode *cur_node) {
  return std::string{
      reinterpret_cast<const char *>(xmlNodeGetContent(cur_node))};
}

template <>
bool get<bool>(xmlNode *cur_node) {
  return get<char>(cur_node) == 't';
}

template <typename T>
void SetFromXml(T &elem, xmlNode *node, std::string xmlname) {
  if (!xmlStrcmp(node->name, (const xmlChar *)xmlname.c_str())) {
    elem = get<T>(node);
  }
}

void Preset::SetupTemplate(xmlNode *doc) {
  SetFromXml<>(template_, doc, "id_template");
}
void Preset::SetupChannel(std::string name, xmlNode *channel_node) {
  int id = std::atoi(&name.back());
  xmlNode *cur_node = NULL;
  Channel c;
  spdlog::info("Setup channel {}", name);
  for (cur_node = channel_node->children; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      SetFromXml<>(c.id, cur_node, "id_audio");
      SetFromXml<>(c.volume, cur_node, "volume");
      SetFromXml<>(c.name, cur_node, "name_audio");
      SetFromXml<>(c.mute, cur_node, "mute");
      SetFromXml<>(c.random, cur_node, "random");
      SetFromXml<>(c.balance, cur_node, "balance");
      SetFromXml<>(c.random_count, cur_node, "random_counter");
      if (!xmlStrcmp(cur_node->name, (const xmlChar *)"random_unit")) {
        switch (
            reinterpret_cast<const char *>(xmlNodeGetContent(cur_node))[1]) {
          case 'h':
            c.random_unit = RandomUnit::_1h;
            break;
          case 'm':
            c.random_unit = RandomUnit::_1m;
            break;
          case '0':
            c.random_unit = RandomUnit::_10m;
            break;
        }
      }
    }
  }
  if (c.id == 0) return;

  std::string basename = utils::GetEnv("AMBIENT_SOUND_DIR", "sounds");
  std::string fileextension = utils::GetEnv("AMBIENT_SOUND_EXTENSION", ".ogg");

  if (fileextension.front() != '.') fileextension = "." + fileextension;

  std::stringstream fn;
  fn << basename << "/" << c.id << fileextension;
  c.channel_id = id;
  spdlog::info("Channel {} filename {}", id, fn.str());
  c.filename = fn.str();
  c.chunk = nullptr;
  this->push_back(c);
}

Preset::Preset() : std::vector<Channel>() {}

std::future<bool> Preset::Load() {
  return std::async(std::launch::async, [&]() {
    bool loaded_all = true;
    std::vector<std::thread> threads;
    for (auto &c : *this) {
      threads.emplace_back([&c, &loaded_all](){
      spdlog::info("loading audio {}", c.filename);
      c.chunk = Mix_LoadWAV(c.filename.c_str());
      if (c.chunk == nullptr) {
        spdlog::error("loading audio {} failed: {}", c.filename,
                      SDL_GetError());
        loaded_all = false;
      }else{
        Mix_VolumeChunk(c.chunk, int(c.volume / 100.0 * 128));
        spdlog::error("audio {} loaded", c.filename);
      }
      });
    }
    for(auto& t : threads){
      t.join();
    }
    return loaded_all;
  });
}

Preset::~Preset() {
  for (auto &c : *this) {
    if (c.chunk != nullptr) {
      Mix_FreeChunk(c.chunk);
    }
  }
}

}  // namespace ambientmixer
