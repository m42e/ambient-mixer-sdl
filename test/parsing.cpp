#include <SDL2/SDL.h>
#include <gtest/gtest.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdlib.h>

#include <cstdlib>
#include <string>
#include <array>

#include "gmock/gmock.h"
#include "preset_parser.h"

namespace {
using namespace ambientmixer;

xmlNodePtr createSimpleXml(int channels) {
  xmlNodePtr root_node = NULL, node = NULL;
  root_node = xmlNewNode(NULL, BAD_CAST "audio_template");
  std::array<std::string, 3> ru{"1h", "10m", "1m"};
  for (int i = 0; i < channels; i++) {
    node = xmlNewChild(
        root_node, NULL,
        BAD_CAST(std::string("channel") + std::to_string(i + 1)).c_str(), NULL);
    xmlNewChild(node, NULL, BAD_CAST "id_audio",
                BAD_CAST std::to_string(i * 10 + 1).c_str());
    xmlNewChild(node, NULL, BAD_CAST "volume",
                BAD_CAST std::to_string(i * 10 + 2).c_str());
    xmlNewChild(node, NULL, BAD_CAST "mute",
                BAD_CAST(i % 2 == 0 ? "true" : "false"));
    xmlNewChild(node, NULL, BAD_CAST "balance",
                BAD_CAST std::to_string(i * 10 - 30).c_str());
    xmlNewChild(node, NULL, BAD_CAST "random",
                BAD_CAST(i % 3 == 0 ? "true" : "false"));
    xmlNewChild(node, NULL, BAD_CAST "random_counter",
                BAD_CAST(i % 3 == 0 ? "true" : "false"));
    xmlNewChild(node, NULL, BAD_CAST "random_unit", BAD_CAST ru[i % 3].c_str());
  }
  return root_node;
}

TEST(Parser, BasicXml) {
  xmlNodePtr root_node = NULL;
  root_node = xmlNewNode(NULL, BAD_CAST "audio_template");
  std::shared_ptr<Preset> p = Preset::createFromXml(root_node);
  ASSERT_EQ(p->size(), 0);
}
TEST(Parser, XmlOneChannel) {
  xmlNodePtr root_node = createSimpleXml(1);
  std::shared_ptr<Preset> p = Preset::createFromXml(root_node->children);
  ASSERT_EQ(p->size(), 1);
}
TEST(Parser, XmlOneChannelWData) {
  xmlNodePtr root_node = createSimpleXml(1);
  std::shared_ptr<Preset> p = Preset::createFromXml(root_node->children);
  ASSERT_EQ(p->size(), 1);
  ASSERT_EQ((*p)[1].filename, "sounds/1.ogg");
  ASSERT_EQ((*p)[1].volume, 2);
  ASSERT_EQ((*p)[1].mute, true);
  ASSERT_EQ((*p)[1].random, true);
}
TEST(Parser, CheckEnvironmentRespect) {
  xmlNodePtr root_node = createSimpleXml(1);

  ::setenv("AMBIENT_SOUND_DIR", "stangedir", 1);
  ::setenv("AMBIENT_SOUND_EXTENSION", ".noext", 1);

  std::shared_ptr<Preset> p = Preset::createFromXml(root_node->children);
  ASSERT_EQ(p->size(), 1);
  ASSERT_EQ((*p)[1].filename, "stangedir/1.noext");
  ::unsetenv("AMBIENT_SOUND_DIR");
  ::unsetenv("AMBIENT_SOUND_EXTENSION");
}
TEST(Parser, XmlOneChannelWManyData) {
  xmlNodePtr root_node = createSimpleXml(5);
  std::shared_ptr<Preset> p = Preset::createFromXml(root_node->children);
  ASSERT_EQ(p->size(), 5);
  ASSERT_EQ((*p)[1].filename, "sounds/1.ogg");
  ASSERT_EQ((*p)[1].volume, 2);
  ASSERT_EQ((*p)[1].mute, true);
  ASSERT_EQ((*p)[1].random, true);
  ASSERT_EQ((*p)[2].filename, "sounds/11.ogg");
  ASSERT_EQ((*p)[2].volume, 12);
  ASSERT_EQ((*p)[2].mute, false);
  ASSERT_EQ((*p)[2].random, false);
  ASSERT_EQ((*p)[3].filename, "sounds/21.ogg");
  ASSERT_EQ((*p)[3].volume, 22);
  ASSERT_EQ((*p)[3].mute, true);
  ASSERT_EQ((*p)[3].random, false);
}

class TestRequiringAudio : public ::testing::Test {
 protected:
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  void SetUp() override {
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels,
                      audio_buffers) != 0) {
      FAIL();
    }
    Mix_AllocateChannels(128);

    Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD);
  }
  void TearDown() override { SDL_Quit(); };
};

TEST_F(TestRequiringAudio, XmlOneChannelWManyDataLoad) {
  xmlNodePtr root_node = createSimpleXml(5);
  std::shared_ptr<Preset> p = Preset::createFromXml(root_node->children);
  ASSERT_EQ(p->size(), 5);
  auto w = p->Load();
  ASSERT_EQ(true, w.get());
}
}  // namespace
