/*
   Small Program to Simulate a Numpad using a 2.4" TFT Touchscreen
   Program does not act as an USB HID Device, for reference purposes only
   Tested on Arduino UNO Only and 0x9341
   By William Tavares

   Note:
   This version is coplete with styling and numbers,
   if you want the smaller version get the "numpad-layout" program
   from my Github https://github.com/williamtavares/Arduino-Uno-NumPad

   Open the Serial Monitor to see the program running
   Enjoy!
*/

#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// calibration mins and max for raw data when touching edges of screen
// YOU CAN USE THIS SKETCH TO DETERMINE THE RAW X AND Y OF THE EDGES TO GET YOUR HIGHS AND LOWS FOR X AND Y
#define TS_MINX 210
#define TS_MINY 210
#define TS_MAXX 915
#define TS_MAXY 910

//SPI Communication
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

//Color Definitons
#define BLACK     0x0000
#define BLUE      0x001F
#define GREY      0xCE79
#define LIGHTGREY 0xDEDB
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 1
#define MAXPRESSURE 1000



// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// Pins A2-A6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

//Size of key containers 70px
#define BOXSIZE 70

//2.4 = 240 x 320
//Height 319 to fit on screen

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//Container variables for touch coordinates
int X, Y, Z;

//Screen height without hidden pixel
double tHeight = tft.height() - 1;
//Centering the mid square
double center = (tft.width() / 2) - (BOXSIZE / 2);
//Space between squares
double padding = 10;
//Position of squares to the left and right of center
double fromCenter = BOXSIZE + padding;
//Second row Y-Axis position
double secondRow = BOXSIZE + padding;
//Third row Y-Axis position
double thirdRow = secondRow + BOXSIZE + padding;
//Fourth row Y-Axis position
double fourthRow = thirdRow + BOXSIZE + padding;
//Y-Axis align for all squares
double verticalAlign = (tHeight - ((BOXSIZE * 4) + (padding * 3))) / 2;
//Left column starting x posision
double leftColPositionX = center - fromCenter;
//Mid column starting x posision
double midColPositionX = center;
//Right column starting x posision
double rightColPositionX = center + fromCenter;

void setup() {
  Serial.begin(9600);

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  //Background color
  tft.fillScreen(LIGHTGREY);

  // draw num pad
  createButtons();
  insertNumbers();
  Serial.println(F("Press any button on the TFT screen: "));



}

void loop() {


  retrieveTouch();
  int boxHeightRow1 = verticalAlign + BOXSIZE;
  int boxHeightRow2 = secondRow + BOXSIZE;
  int boxHeightRow3 = thirdRow + BOXSIZE;
  int boxHeightRow4 = fourthRow + BOXSIZE;

  if (Z > MINPRESSURE && Z < MAXPRESSURE) {

    //redraw numpad to clear old number
    tft.fillScreen(LIGHTGREY);
    createButtons();
    insertNumbers();
    //default text setup for number display on tft
    tft.setCursor(100, 120);
    tft.setTextColor(RED);
    tft.setTextSize(9);

    //Check if element clicked is in left column
    if (X > leftColPositionX && X < (leftColPositionX + BOXSIZE)) {
      //Check if element clicked is in row 1
      if (Y > verticalAlign) {
        if (Y < boxHeightRow1) {
          Serial.println("1");
          tft.println("1");
          delay(150);
        }
        //Check if element clicked is in row 2
        else if (Y < boxHeightRow2) {
          Serial.println("4");
          tft.println("4");
          delay(150);
        }
        //Check if element clicked is in row 3
        else if (Y < boxHeightRow3) {
          Serial.println("7");
          tft.println("7");
          delay(150);
        }
        //Check if element clicked is in row 4
        else if (Y < boxHeightRow4) {
          Serial.println("0");
          tft.println("0");
          delay(150);
        }
      }
      //Check if element clicked is in mid column
    } else if (X > midColPositionX && X < (midColPositionX + BOXSIZE)) {
      //Check if element clicked is in row 1
      if (Y > verticalAlign) {
        if (Y < boxHeightRow1) {
          Serial.println("2");
          tft.println("2");
          delay(150);
        }
        //Check if element clicked is in row 2
        else if (Y < boxHeightRow2) {
          Serial.println("5");
          tft.println("5");
          delay(150);
        }
        //Check if element clicked is in row 3
        else if (Y < boxHeightRow3) {
          Serial.println("8");
          tft.println("8");
          delay(150);
        }
        //Check if element clicked is in row 4
        else if (Y < boxHeightRow4) {
          Serial.println("0");
          tft.println("0");
          delay(150);
        }
      }
      //Check if element clicked is in third column
    } else if (X > rightColPositionX && X < (rightColPositionX + BOXSIZE)) {
      if (Y > verticalAlign) {
        //Check if element clicked is in row 1
        if (Y < boxHeightRow1) {
          Serial.println("3");
          tft.println("3");
          delay(150);
        }
        //Check if element clicked is in row 2
        else if (Y < boxHeightRow2) {
          Serial.println("6");
          tft.println("6");
          delay(150);
        }
        //Check if element clicked is in row 3
        else if (Y < boxHeightRow3) {
          Serial.println("9");
          tft.println("9");
          delay(150);
        }
        //Check if element clicked is in row 3
        else if (Y < boxHeightRow4) {
          Serial.println(".");
          tft.println(".");
          delay(150);
        }
      }
    }

    // good for debuggin, prints out the x,y cordinates of the press
    // tft.setTextSize(3);
    // tft.print("X = "); tft.println(X);
    // tft.print("Y = "); tft.println(Y);

  }
}

void retrieveTouch()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //If sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);




  //Scale from 0->1023 to tft.width
    //X = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
  //  Y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());

  // on my tft the numbers are reversed so this is used instead of the above
  X = tft.width() - map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
  Y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
  Z = p.z;



}

void createButtons() {
  //(initial x,initial y,width,height,color)
  double secondRowVertialAlign = secondRow + verticalAlign;
  double thirdRowVertialAlign = thirdRow + verticalAlign;
  double fourthRowVertialAlign = fourthRow + verticalAlign;

  /***Draw filled squares with specified dimensions and position***/
  //First Row
  tft.fillRect(leftColPositionX, verticalAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(midColPositionX, verticalAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(rightColPositionX, verticalAlign, BOXSIZE, BOXSIZE, GREY);

  //Second Row
  tft.fillRect(leftColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(midColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(rightColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, GREY);

  //Third Row
  tft.fillRect(leftColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(midColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, GREY);
  tft.fillRect(rightColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, GREY);

  //Fourth Row
  tft.fillRect(leftColPositionX, fourthRowVertialAlign, (BOXSIZE * 2) + padding, BOXSIZE, GREY);
  tft.fillRect(rightColPositionX, fourthRowVertialAlign, BOXSIZE, BOXSIZE, GREY);

  /***Draw Borders around squares***/
  //First Row
  tft.drawRect(leftColPositionX, verticalAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(midColPositionX, verticalAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(rightColPositionX, verticalAlign, BOXSIZE, BOXSIZE, BLACK);

  //Second Row
  tft.drawRect(leftColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(midColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(rightColPositionX, secondRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);

  //Third Row
  tft.drawRect(leftColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(midColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);
  tft.drawRect(rightColPositionX, thirdRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);

  //Fourth Row
  tft.drawRect(leftColPositionX, fourthRowVertialAlign, (BOXSIZE * 2) + padding, BOXSIZE, BLACK);
  tft.drawRect(rightColPositionX, fourthRowVertialAlign, BOXSIZE, BOXSIZE, BLACK);
}

void insertNumbers() {
  //Centers text horizontally on all three columns
  double leftColCursorX   = leftColPositionX + (BOXSIZE / 3);
  double midColCursorX    = midColPositionX  + (BOXSIZE / 3);
  double rightColCursorX  = rightColPositionX + (BOXSIZE / 3);
  //Centers text horizontally on all four rows
  double firstRowCursorY  = verticalAlign + (BOXSIZE / 3);
  double secondRowCursorY = secondRow + firstRowCursorY;
  double thirdRowCursorY  = thirdRow  + firstRowCursorY;
  double fourthRowCursorY = fourthRow + firstRowCursorY;

  tft.setTextSize(4);
  tft.setTextColor(BLACK);

  //Insert Number 1
  tft.setCursor(leftColCursorX, firstRowCursorY);
  tft.println("1");

  tft.setCursor(midColCursorX, firstRowCursorY);
  tft.println("2");

  //Insert Number 3
  tft.setCursor(rightColCursorX, firstRowCursorY);
  tft.println("3");

  //Insert Number 4
  tft.setCursor(leftColCursorX, secondRowCursorY);
  tft.println("4");

  //Insert Number 5
  tft.setCursor(midColCursorX, secondRowCursorY);
  tft.println("5");

  //Insert Number 6
  tft.setCursor(rightColCursorX, secondRowCursorY);
  tft.println("6");

  //Insert Number 7
  tft.setCursor(leftColCursorX, thirdRowCursorY);
  tft.println("7");

  //Insert Number 8
  tft.setCursor(midColCursorX, thirdRowCursorY);
  tft.println("8");

  //Insert Number 9
  tft.setCursor(rightColCursorX, thirdRowCursorY);
  tft.println("9");

  //Insert Number 0
  tft.setCursor(leftColPositionX + BOXSIZE, fourthRowCursorY);
  tft.println("0");

  //Insert Period Character
  tft.setCursor(rightColCursorX, fourthRowCursorY);
  tft.println(".");
}

