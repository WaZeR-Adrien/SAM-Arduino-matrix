/***************************************************
*
* For 32x16 RGB LED matrix.
*
* @author lg.gang
* @version  V1.0
* @date  2016-10-28
*
* GNU Lesser General Public License.
* See <http://www.gnu.org/licenses/> for details.
* All above must be included in any redistribution
* ****************************************************/

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

static const uint16_t colors[4] = {
  0x001F, // Blue
  0x07E0, // Green
  0xFFE0, // Yellow
  0xFFFF  // White
};

const int BLUE_BUTTON = 13;
const int GREEN_BUTTON = 12;
const int YELLOW_BUTTON = 11;
const int WHITE_BUTTON = 10;

boolean startGame = false;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(2);

  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
}

void loop() {
  // Init counters
  int increment = 75;
  int score = 0;


  // Display start message
  if (!startGame) {
    matrix.setTextColor(matrix.Color333(7,0,0));
    matrix.setCursor(7, 0);
    matrix.setTextSize(1);
    matrix.print("SAM");
  
    matrix.setTextColor(matrix.Color333(7,4,0));
    matrix.setCursor(1, 9);
    matrix.setTextSize(0.5);
    matrix.print("[KEY]");

    if (digitalRead(BLUE_BUTTON) == 0 || digitalRead(GREEN_BUTTON) == 0 || digitalRead(YELLOW_BUTTON) == 0 || digitalRead(WHITE_BUTTON) == 0) {
      startGame = true;
      matrix.fillScreen(0);
      delay(3000);
    }
  }

  // Loop on random colors with specific delay while startGame is true
  while (startGame == true) {
    byte i;
    boolean success = false;
    char state = 0;
    uint16_t color = colors[(int)random(0, 4)];
    
    matrix.fillScreen(color);
    
    for (i=0; i < increment; i++) {
      delay(30);
      if (state != 'B' && digitalRead(BLUE_BUTTON) == 0) {
        state = 'B' ;
      }
      if (state != 'G' && digitalRead(GREEN_BUTTON) == 0) {
        state = 'G';
      }
      if (state != 'Y' && digitalRead(YELLOW_BUTTON) == 0) {
        state = 'Y';
      }
      if (state != 'W' && digitalRead(WHITE_BUTTON) == 0) {
        state = 'W';
      }
    }
  
    if ((color == 0x001F && state == 'B') || (color == 0x07E0 && state == 'G') || (color == 0xFFE0 && state == 'Y') || (color == 0xFFFF && state == 'W')) {
      success = true;
      score++;
    }
  
    if (!success) {
      matrix.fillScreen(0);
      
      matrix.setTextColor(matrix.Color333(7,0,0));
      matrix.setTextSize(1);
      
      matrix.setCursor(4, 0);
      matrix.print("GAME");
      matrix.setCursor(4, 9);
      matrix.print("OVER");
      delay(3000);
      
      matrix.fillScreen(0);
      matrix.setTextColor(matrix.Color333(7,0,0));
      matrix.setCursor(1, 0);
      matrix.print("SCORE");
      
      matrix.setTextColor(matrix.Color333(7,4,0));
      matrix.setCursor(1, 9);
      matrix.print(score);
      delay(7000);
    }
  
    increment = increment - 3;
    
    if (!success) {
      startGame = false;
    }
    
    matrix.fillScreen(0);
    delay(500);
  }

  #if !defined(__AVR__)
    // On non-AVR boards, delay slightly so screen updates aren't too quick.
    delay(20);
  #endif

  // Update display
  matrix.swapBuffers(false);
}
