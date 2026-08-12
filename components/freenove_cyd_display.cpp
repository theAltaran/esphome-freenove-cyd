#include "freenove_cyd_display.h"
#include "esphome/core/log.h"

namespace esphome {
namespace freenove_cyd {

static const char* TAG = "freenove_cyd_display";

void FreenoveCYDDisplay::setup() {
  ESP_LOGI(TAG, "Initializing Freenove CYD 3.5\" Display");
  
  // Initialize the QSPI display
  tft_qspi_.begin();
  
  // Create sprite for double-buffering (uses PSRAM)
  spr_.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
  spr_.setSwapBytes(true);
  
  // Set to portrait mode (90 degree rotation)
  tft_qspi_.setRotation(1);
  
  // Fill with black background
  spr_.fillSprite(TFT_BLACK);
  
  // Draw initial "test" text centered
  draw_text("test", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, TFT_WHITE);
  
  // Push sprite to display
  spr_.pushSprite(0, 0);
  
  ESP_LOGI(TAG, "Display initialized - Resolution: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
  
  // Log PSRAM info
  if (psramFound()) {
    ESP_LOGI(TAG, "PSRAM found - Total: %d KB, Free: %d KB", 
             ESP.getPsramSize() / 1024, 
             ESP.getFreePsram() / 1024);
  } else {
    ESP_LOGW(TAG, "No PSRAM detected - display may have limited capabilities");
  }
}

void FreenoveCYDDisplay::loop() {
  // Display updates can be triggered by calling update()
}

void FreenoveCYDDisplay::update() {
  // Redraw and push sprite to display
  spr_.pushSprite(0, 0);
  ESP_LOGD(TAG, "Display updated");
}

void FreenoveCYDDisplay::draw_text(const char* text, uint16_t x, uint16_t y, uint16_t color) {
  // Set text properties
  spr_.setTextSize(1);
  spr_.setTextColor(color);
  spr_.setTextFont(4);  // Font 4 (medium sized)
  
  // Get text bounds
  int16_t x1, y1;
  uint16_t text_width, text_height;
  spr_.getTextBounds(text, 0, 0, &x1, &y1, &text_width, &text_height);
  
  // Calculate centered position (x, y are center points)
  int16_t draw_x = x - (text_width / 2);
  int16_t draw_y = y - (text_height / 2);
  
  // Draw the text
  spr_.drawString(text, draw_x, draw_y);
  
  ESP_LOGD(TAG, "Text '%s' drawn at (%d, %d) - size: %dx%d", 
           text, draw_x, draw_y, text_width, text_height);
}

}  // namespace freenove_cyd
}  // namespace esphome
