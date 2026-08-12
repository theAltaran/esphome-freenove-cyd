#pragma once

#include "esphome/core/component.h"
#include <TFT_eSPI.h>
#include <SPI.h>

namespace esphome {
namespace freenove_cyd {

class FreenoveCYDDisplay : public Component {
 public:
  void setup() override;
  void loop() override;
  void update();
  void draw_text(const char* text, uint16_t x, uint16_t y, uint16_t color);

  float get_setup_priority() const override { return setup_priority::LATE; }

 private:
  TFT_eSPI tft_qspi_;
  TFT_eSprite spr_{&tft_qspi_};
  
  static constexpr int SCREEN_WIDTH = 480;
  static constexpr int SCREEN_HEIGHT = 320;
};

}  // namespace freenove_cyd
}  // namespace esphome
