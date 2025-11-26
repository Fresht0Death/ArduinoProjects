#include <TouchScreen.h>
#include <Elegoo_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <Elegoo_GFX.h>
#include <millisDelay.h>




// global y values for top and bottom half
int y0 = 320;
int y1 = 0;

// count for number of pomodoros a user has done
int number_of_pomodoros = 0;

bool freshStart = false;   // true = start new pomodoro, false = resume


// state variables
enum HourGlassState{
  TOP_Q1,
  TOP_Q2,
  TOP_Q3,
  TOP_Q4,
  DONE,
  WAIT_START
};

// starting state
HourGlassState hgState = TOP_Q1;
HourGlassState previousState = TOP_Q1;  // remember where to resume


bool paused = false;


// Pause button rectangle (right lower box)
#define PAUSE_X      220
#define PAUSE_Y      160
#define PAUSE_W      20
#define PAUSE_H      20

// Reset button rectangle (right upper box)
#define RESET_X      220
#define RESET_Y      140
#define RESET_W      20
#define RESET_H      20







#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);



//Macros for human-readable colors
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define SAND    0xFF80
#define ORANGERED 0xFC00
#define ORANGEYELLOW 0xFE40
#define YELLOWGREEN 0xB7E0



// pin configuration
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


// initialize timers   
millisDelay q1Timer;        
millisDelay q2Timer;
millisDelay q3Timer;
millisDelay q4Timer;
millisDelay breakTimer;



/****************************************************
 * Function: drawHourGlass                          *
 *       Purpose:                                   *
 *              draws the shape of the hourglass    *
 *                                                  *
 ****************************************************/

void drawHourGlass();


/*****************************************
*   Function: updateTopQn                *
*                                        *
*   Purpose: in 6 min and 15 seconds     *
*   empty and fill quadrant of the       *
*   hourglass. This is done by drawing   *
*   sand line or white line.             *
*                                        *
******************************************/

void updateTopQ1();
void updateTopQ2();
void updateTopQ3();
void updateTopQ4();


/*****************************************
*   Function: drawReadyScreen            *
*                                        *
*   Purpose: draws a box and asks user   *
*   if they are ready to begin a pomo-   *
*   doro session. Used at start, pause   *
*   , and post 5 minute break.           * 
*                                        *
******************************************/

void drawReadyScreen();


/*****************************************
*   Function: takeBreak                  *
*                                        *
*   purpose: draws a box and asks user   *
*   if they are ready to begin a pomo-   *
*   doro session. Used at start, pause   *
*   , and post 5 minute break.           * 
*                                        *
******************************************/

// checks if blue and black box has been drawn already
// used in takeBreak
bool blackBlueBoxState = false;
// used for 5 minute break to count each minute that has passed by
int minuteCounter = 0;

void takeBreak(){

  if(blackBlueBoxState == false){


      // initial black and blue box
  tft.fillRect(50, 30, 141, 90, BLUE);
  tft.fillRect(52, 32, 137, 86, BLACK);

  // sets cursor to tell user to take 5 minute break
  tft.setCursor(55, 45);
  tft.setTextColor(WHITE); tft.setTextSize(1);
  tft.println("Take a 5 minute break!");
  blackBlueBoxState = true;

  }



    // check if delay has timed out and toggles LED corresponding to where it is on the interval chain, when reaches end of interval loops back to the begginning 
  if (breakTimer.justFinished()) {
    breakTimer.repeat(); // start delay again without drift
   
   // keeps traack of elapsed intervals
    minuteCounter++;

  
  switch(minuteCounter){ // checks for intervals and turns on corresponding led while turning off others that are on

    // used for debugging
    // Serial.println();
    // Serial.print(minuteCounter);


    case 1: // 1 interval passed

    tft.fillRect(70, 85, 10, 20, RED);

      break;
    
    case 2: // 2 intervals passed

      tft.fillRect(90, 85, 10, 20, ORANGERED);

      break;

    case 3: // 3 intervals passed

      tft.fillRect(110, 85, 10, 20, ORANGEYELLOW);

      break;

    case 4:

      tft.fillRect(130, 85, 10, 20, YELLOWGREEN);

      break;

    case 5:
      tft.fillRect(150, 85, 10, 20, GREEN);

      paused        = true;
      previousState = TOP_Q1;   // next session starts from Q1
      freshStart    = true;     // <<< next YES should reset sand
      drawReadyScreen();
      hgState = WAIT_START;
      blackBlueBoxState = false;
      breakTimer.finish();      // optional: stop break timer
      break;


    default:
      break;


  }
}

}





#define MINPRESSURE 10
#define MAXPRESSURE 1000

bool getTouch(int &sx, int &sy) {
  TSPoint p = ts.getPoint();

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
    return false;
  }

  // direct mapping
  sx = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  sy = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  return true;
}




// generic rectangle hit test
bool inRect(int x, int y, int rx, int ry, int rw, int rh) {
  return (x >= rx && x < rx + rw &&
          y >= ry && y < ry + rh);
}









/*===========================\\
//                           \\
//    Visual of hourglass    \\
//        with (x,y)         \\
//        sand = ***         \\
//                           \\
//===========================\\

__________________________
|   |================|   |                 x = 50-190, y = 320     
|   |****************|   |                             y-- ⬇
|   |****************|   |                             y-- ⬇
|   |****************|   |                 x = 50-190, y = 209
|    ==|**********|==    |                 x = 65-175, y = 210  -> 194
|     ==|********|==     |                 x = 80-160, y = 194  -> 179
|      ==|******|==      |                 x = 95-145, y = 179  -> 169
|       ==|    |==       |                             
|      ==|******|==      |                 X = 95-145, y = 140  -> 155           
|     ==|********|==     |                 x = 80-160, y = 125  -> 140
|    ==|**********|==    |                 x = 65-175, y = 110  -> 125
|   |****************|   |                 x = 50-190, y = 110 
|   |****************|   |                             y++ ⬆
|   |****************|   |                             y++ ⬆
|   |================|   |                 x = 50-190, y = 0
--------------------------

*/

/*************************************************************
 * Function:                                                 *
 * fillBottomHourGlass:                                      *
 *              fills in bottom of hourglass with sand which *
 *                  this begins at x = [50,190] & y =[0,155] *   
 *                      x is fixed in each quadrant          *
 *                          y changes while it draws a line  *
 *                                                           *
 *************************************************************/

void fillBottomHourGlass();


/*************************************************************
 * Function:                                                 *
 * fillTopHourGlass:                                         *
 *              fills in top of hourglass with sand which    *
 *              this begins at x = [50,320] & y =[0,169]     *   
 *              x is fixed in each quadrant                  *
 *              y changes while it draws a line              *
 *              this is fixed over 25 min interval where     *
 *              each quadrant fills in in 6 mins 15 sec      *
 *                                                           *
 *************************************************************/

void fillTopHourGlass();


/****************************************************************
 * Function:                                                    *
 *     drawBoxesWithVariables:                                  *
 *              draws the boxes which keep are on the left      *
 *              and right side of the screen. These boxes       *
 *              keep track of number of pomodoros completed,    *
 *               number sign, R = reset button, and || = pause. * 
 *                                                              *  
 ****************************************************************/

void drawBoxesWithVariables();


/**************************************************************
 * Function:                                                  *
 *     updateNumberOfPomodoros:                               *
 *              increments the count on the screen by 1       *
 *               everytime a pomodoro has been completed.     *
 *                                                            * 
 **************************************************************/

void updateNumberOfPomodoros();





void checkPauseButton() {

  int x, y;
  if (!getTouch(x, y)) {
    return;  // no touch this loop
  }

  // ----- RESET button (right upper box) -----
  if (inRect(x, y, RESET_X, RESET_Y, RESET_W, RESET_H)) {
    // full reset of timer + UI
    paused        = false;
    hgState       = TOP_Q1;
    previousState = TOP_Q1;
    minuteCounter = 0;
    blackBlueBoxState = false;
    number_of_pomodoros = 0;
    freshStart    = true;   // next YES should start from a clean hourglass

    // clear and redraw everything
    tft.fillScreen(WHITE);
    drawHourGlass();
    drawBoxesWithVariables();
    initialfillBottomHourGlass();

    // reset sand positions
    y0 = 320;
    y1 = 0;

    // stop all timers
    q1Timer.finish();
    q2Timer.finish();
    q3Timer.finish();
    q4Timer.finish();
    breakTimer.finish();

    // show "Ready?" again
    paused  = true;
    hgState = WAIT_START;
    drawReadyScreen();
    return;
  }

  // ----- PAUSE BUTTON -----
if (inRect(x, y, PAUSE_X, PAUSE_Y, PAUSE_W, PAUSE_H)) {

  // Don't let pause/start bypass the READY dialog
  if (hgState == WAIT_START) return;

  if (!paused) {
    // we were running → PAUSE
    previousState = hgState;
    paused        = true;

    q1Timer.finish();
    q2Timer.finish();
    q3Timer.finish();
    q4Timer.finish();

    drawPauseButton();     // turns green, shows ▶

  } else {
    // we were paused → RESUME from same spot
    paused = false;

    switch (previousState) {
      case TOP_Q1: q1Timer.start(3378);  break;
      case TOP_Q2: q2Timer.start(23438); break;
      case TOP_Q3: q3Timer.start(25000); break;
      case TOP_Q4: q4Timer.start(37500); break;
      default: break;
    }

    hgState = previousState;
    drawPauseButton();     // back to black "||"
  }

  return;
}


}



void checkStartButton() {
  int x, y;
  if (!getTouch(x, y)) {
    return; // no touch
  }

  // DEBUG (keep this while tuning):
  Serial.print("WAIT_START touch: x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.println(y);

  // YES button region in TOUCH coords.
  // You saw about (110, 220) in the serial log, so give it a fat box:
  if (inRect(x, y, 80, 200, 80, 60)) {
    Serial.println("YES HIT!");

    paused = false;

    // clear the dialog box area (same place you drew it)
    tft.fillRect(50, 30, 141, 90, WHITE);

    // If this is a "fresh" session (boot or after break or reset),
    // redraw and refill the hourglass.
    if (freshStart) {
      tft.fillScreen(WHITE);
      drawHourGlass();
      drawBoxesWithVariables();
      initialfillBottomHourGlass();

      y0 = 320;
      y1 = 0;

      previousState = TOP_Q1;  // new sessions always start at Q1
      freshStart    = false;   // consumed the fresh start
    }

    // restart the correct timer
    switch (previousState) {
      case TOP_Q1: q1Timer.start(3378);  break;
      case TOP_Q2: q2Timer.start(23438); break;
      case TOP_Q3: q3Timer.start(25000); break;
      case TOP_Q4: q4Timer.start(37500); break;
      default: break;
    }

    hgState = previousState;
  }
}







void setup() {

  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);
  tft.fillScreen(WHITE);

  drawHourGlass();
  drawBoxesWithVariables();
  initialfillBottomHourGlass();

  // ---- NEW: start in WAIT_START, not running ----
  hgState       = WAIT_START;
  previousState = TOP_Q1;   // first session always starts at Q1
  paused        = true;
  freshStart    = true;     // first YES should start a new pomodoro
  // DO NOT start q1Timer here
    drawReadyScreen();        // <<< show "Ready to begin?" box

}






void loop(void) {

  // put your main code here, to run repeatedly:

   // Always listen for Pause taps
  checkPauseButton();

  // If we're in WAIT_START, we *don't* want to early-return; we want to check YES.
  if (hgState != WAIT_START && paused) {
    return;   // frozen during RUN only
  }


  switch(hgState){
    case TOP_Q1:
      updateTopQ1();
      break;
    case TOP_Q2:
      updateTopQ2();
      break;
    case TOP_Q3:
      updateTopQ3();
      break;
    case TOP_Q4:
      updateTopQ4();
      break;
    case DONE:
      takeBreak();
      break;
    case WAIT_START:
      checkStartButton();
      break;

    default:

      break;


  }




}





void updateTopQ1(){

  if (q1Timer.justFinished()) {
    int drawnY = y0;  // remember which line we’re drawing

    tft.drawLine(50, drawnY, 190, drawnY, SAND);
    y0--;
    tft.drawLine(50, y1, 190, y1, WHITE);
    y1++;
    q1Timer.repeat();

    // if we just drew the last line of this band
    if (drawnY == 211) {
      hgState = TOP_Q2;
      q2Timer.start(23438);
      y0 = 210;   // starting Y for Q2 band
      y1 = 110;   // new starting bottom Y for erase
      tft.drawLine(50, y0, 190, y0, WHITE);
    }
  }

}


void updateTopQ2(){

  if (q2Timer.justFinished()) {
    int drawnY = y0;

    tft.drawLine(65, drawnY, 175, drawnY, SAND);
    y0--;
    tft.drawLine(65, y1, 175, y1, WHITE);
    y1++;
    q2Timer.repeat();

    if (drawnY == 196) {
      hgState = TOP_Q3;
      q3Timer.start(25000);
    
      // next band starts here
      y0 = 195;   // top Q3 start
      y1 = 125;   // bottom Q3 start
      tft.drawLine(50, y1, 190, y1, WHITE);
    }
  }


  


}

void updateTopQ3(){


  if (q3Timer.justFinished()) {
    int drawnY = y0;

    tft.drawLine(80, drawnY, 160, drawnY, SAND);
    y0--;
    tft.drawLine(80, y1, 160, y1, WHITE);
    y1++;

    q3Timer.repeat();

    // when we hit the last line of this band (179), switch to Q4
    if (drawnY == 181) {
      hgState = TOP_Q4;
      q4Timer.start(37500);   // 6.25 min / 10 lines

       // next band starts here
      y0 = 180;   // top Q4 start
      y1 = 140;   // bottom Q4 start
      tft.drawLine(50, y1, 190, y1, WHITE);

    }
  }


}

  void updateTopQ4(){
    if (q4Timer.justFinished()) {
    int drawnY = y0;

    tft.drawLine(95, drawnY, 145, drawnY, SAND);
    y0--;
    tft.drawLine(95, y1, 145, y1, WHITE);
    y1++;

    q4Timer.repeat();

    // if we just drew the last line of this band (169), we're done
    if (drawnY == 166) {
      hgState = DONE;
      q4Timer.finish();
      number_of_pomodoros++;
      updateNumberOfPomodoros();



      // start break logic here
      minuteCounter = 0;
      breakTimer.start(60000);   // 1-minute intervals for the 5-minute break
    }
  }



    
  }

void drawPauseButton() {
  // VISUAL location of the TOP box (pause/play)
  const int PX = 220;   // top box X
  const int PY = 140;   // top box Y
  const int PW = 20;
  const int PH = 20;

  // background color depends on paused state
  uint16_t bg = paused ? GREEN : BLACK;   // green when paused, black when running

  // clear the box area
  tft.fillRect(PX, PY, PW, PH, WHITE);

  // outline
  tft.drawRect(PX, PY, PW, PH, BLUE);

  // inner background
  tft.fillRect(PX + 1, PY + 1, PW - 2, PH - 2, bg);

  if (!paused) {
    // running → show "||" in white
    tft.fillRect(PX + 4,  PY + 3, 3, PH - 6, WHITE);
    tft.fillRect(PX + 11, PY + 3, 3, PH - 6, WHITE);
  } else {
    // paused → show ▶ in white
    tft.fillTriangle(
      PX + 4,          PY + 3,
      PX + 4,          PY + PH - 3,
      PX + PW - 4,     PY + PH / 2,
      WHITE
    );
  }
}





/*=============================================================================================================*/
/***********************************DON'T TOUCH THESE BC THEY WORK AS INTENDED**********************************/
/*=============================================================================================================*/

void drawHourGlass(){

  tft.fillRect(30, 210, 20, 320, BLACK); // bottom left BIG rectangle
  tft.fillRect(190, 210, 20, 320, BLACK); // bottom right BIG rectangle


  tft.fillRect(30, 0, 20, 110, BLACK); // top left BIG rectangle
  tft.fillRect(190, 0, 20, 110, BLACK); // top right BIG rectangle

  tft.fillRect(30, 0, 20, 110, BLACK); // top left BIG rectangle
  tft.fillRect(50, 110, 15, 15, BLACK); // top left SML rectangle
  tft.fillRect(65, 125, 15, 15, BLACK); // top left SML rectangle #2
  tft.fillRect(80, 140, 15, 15, BLACK); // top left SML rectangle #3



  tft.fillRect(30, 210, 20, 320, BLACK); // bottom left BIG rectangle
  tft.fillRect(50, 195, 15, 15, BLACK); // bottom left SML rectangle
  tft.fillRect(65, 180, 15, 15, BLACK); // bottom left SML rectangle #2
  tft.fillRect(80, 165, 15, 15, BLACK); // bottom left SML rectangle #3

  tft.fillRect(190, 210, 20, 320, BLACK); // bottom right BIG rectangle
  tft.fillRect(175, 195, 15, 15, BLACK); // bottom right SML rectangle
  tft.fillRect(160, 180, 15, 15, BLACK); // bottom right SML rectangle #2
  tft.fillRect(145, 165, 15, 15, BLACK); // bottom right SML rectangle #3


  tft.fillRect(190, 0, 20, 110, BLACK); // top right BIG rectangle
  tft.fillRect(175, 110, 15, 15, BLACK); // top right SML rectangle
  tft.fillRect(160, 125, 15, 15, BLACK); // top right SML rectangle #2
  tft.fillRect(145, 140, 15, 15, BLACK); // top right SML rectangle #3

  // in between shape
  tft.fillRect(90, 155, 15,15, BLACK);
  tft.fillRect(135, 155, 15,15, BLACK);

}



void initialfillBottomHourGlass(){



  int y = 0; // start bottom of hourglass                                         range(x  = fixed for each quadrant, y changes while it draws lines)


  while(y != 110){ // first set of rectangles for BOTTOM of hour glass            range(x = 50-190, y= 0-110)

  tft.drawLine(50, y, 190, y, SAND);

  y +=1;

  }

  while(y != 125){ // second set of rectangle for BOTTOM of hour glass            range(x = 65-175, y = 110-125)

  tft.drawLine(65, y, 175, y, SAND);

  y +=1;
 
  }

  while(y != 140){ // third set of rectangle for BOTTOM of hour glass             range(x = 80-160, y = 125 - 140)
 
  tft.drawLine(80, y, 160, y, SAND);

  y +=1;

  }
    
  while(y != 155){ // fourth set of rectangle for BOTTOM of hour glass           range(x = 95-145, y = 140-155)

  tft.drawLine(95, y, 145, y, SAND);

  y +=1;

  }

}

void drawBoxesWithVariables(){

  // Boxes and numbers of pomodoros
  tft.drawRect(0, 140, 20, 20, BLUE);
  tft.drawRect(0, 160, 20, 20, BLACK);

  // "#"
  tft.setCursor(5, 142);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("#");

  // current pomodoro count
  // clear the number area first
  tft.fillRect(5, 162, 20, 16, WHITE);
  tft.setCursor(5, 162);
  tft.setTextSize(2);
  // if you want first 0 in black and later counts in red:
  if (number_of_pomodoros == 0) {
    tft.setTextColor(BLACK);
  } else {
    tft.setTextColor(RED);
  }
  tft.print(number_of_pomodoros);

  // Boxes for reset and pause
  tft.drawRect(220, 140, 20, 20, BLUE);   // top box
  tft.drawRect(220, 160, 20, 20, BLACK);  // bottom box

  // pause/play icon in top box
  drawPauseButton();

  // "R" in bottom box
  tft.setCursor(225,162);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("R");
}



  void updateNumberOfPomodoros(){
      tft.fillRect(5, 162, 10, 15, WHITE);
      tft.setCursor(5, 162);
      tft.setTextColor(RED);
      tft.setTextSize(2);
      tft.print(number_of_pomodoros);

  }

  void drawReadyScreen(){

  // background box
  tft.fillRect(50, 30, 141, 90, BLUE);
  tft.fillRect(52, 32, 137, 86, BLACK);

  tft.setCursor(55, 45);
  tft.setTextColor(WHITE); tft.setTextSize(1);
  tft.println("   Ready to begin?");

  // YES button
  tft.fillRect(100, 85, 30, 20, GREEN);
  tft.drawRect(100, 85, 30, 20, WHITE);
  tft.setCursor(107, 92);
  tft.setTextColor(BLACK); tft.setTextSize(1);
  tft.println("YES");


}




