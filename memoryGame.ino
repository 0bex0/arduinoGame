#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
int state, change, seqLength = 4, symbols = 2, randNum, turn, seconds = 2, d = 1000, pass, level = 1;
unsigned long now = 0;
boolean selPress, rightPress, leftPress, upPress, downPress, waitingPress, storyMode;

#define START 0
#define MENU 1
#define PRACTICE_MENU 2
#define STORY 3
#define GAME 4

#define SEQUENCE 10
#define CHARACTERS 20 
#define COMP 30
#define HUMAN 40
#define TIMER 50

#define buttonPress(x) (button_state & (x))

//Functions to print different menus onto LCD.

void printStartMenu() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("SELECT MODE:");
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.print("PRACTICE STORY");
  lcd.write(2);
}
void printSequenceMenu() {
  lcd.clear();
  lcd.print("SEQUENCE LENGTH");
  lcd.setCursor(0,1);
  lcd.print(seqLength);
  lcd.setCursor(15,0);
  lcd.write(2);
  lcd.setCursor(15,1);
  lcd.write(2);
}
void printSymbolMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(3);
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.setCursor(15,0);
  lcd.write(2);
  lcd.setCursor(15,1);
  lcd.write(2);
  lcd.setCursor(3,0);
  lcd.print("# OF SYMBOLS");
  lcd.setCursor(14,1);
  lcd.print(symbols);
}
void printTimerMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(3);
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.setCursor(11,0);
  lcd.print("TIMER");
  lcd.setCursor(12,1);
  lcd.print ("secs");
  lcd.setCursor(10,1);
  lcd.print(seconds);
}

//Function to reset variables when a game is lost.
void reset() {
  d = 1000;
  seqLength = 4;
  symbols = 2;
  seconds = 2;
  pass = 1;
  level = 1;
  storyMode = false;
}


void startGame() {
  state = GAME;
  turn = COMP; 
  lcd.clear();
  delay(1000);
}


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  byte up[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100
  };
  byte down[8] = {
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b11111,
    0b01110,
    0b00100
  };
  byte right[8] = {
    0b00000,
    0b00100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b00100,
    0b00000
  };
  byte left[8] = {
    0b00000,
    0b00100,
    0b01100,
    0b11111,
    0b11111,
    0b01100, 
    0b00100,
    0b00000
  };
  lcd.createChar(0, up);
  lcd.createChar(1, down);
  lcd.createChar(2, right);
  lcd.createChar(3, left);
  state = START;
}

void loop() {
  // put your main code here, to run repeatedly:
  int button_state = lcd.readButtons();
  switch(state) {
    
    case START:
      reset();
      lcd.setBacklight(7);
      lcd.setCursor(0,0);
      lcd.print("                ");
      if (millis() - now > 400) {
        now = millis();
        lcd.print("PRESS SELECT TO BEGIN"); 
        lcd.scrollDisplayLeft();
      }
      if (buttonPress(BUTTON_SELECT)) {selPress = true;}
      else {
        if (selPress) {
          state = MENU;
          printStartMenu();
        }
        selPress = false;
      }
      break;
      
    case MENU:
    //Presents a choice of 'Practice' or 'Story' mode to user.
      if (buttonPress(BUTTON_LEFT)) {leftPress = true;}
      else {
        if (leftPress) {
          state = PRACTICE_MENU;
          change = SEQUENCE;
          printSequenceMenu();
        }
        leftPress = false;
      }
      if (buttonPress(BUTTON_RIGHT)) {rightPress = true;}
      else {
        if (rightPress) {
          storyMode = true;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("LEVEL 1.");
          lcd.print(pass);
          delay(500);
          startGame();
        }
        rightPress = false;
      }
      break;
      
    case PRACTICE_MENU:
    //Presents a three part menu to change length of sequence, time and number of symbols/characters.
      switch(change) {
        
        case SEQUENCE:
          if (buttonPress(BUTTON_UP)) {upPress = true;}
          else {
            if (upPress & seqLength < 10) {
              lcd.setCursor(0,1);
              seqLength++;
              lcd.print(seqLength);
            }
            upPress = false;
          }
          if (buttonPress(BUTTON_DOWN)) {downPress = true;}
          else {
            if (downPress & seqLength > 4) {
              lcd.setCursor(0,1);
              seqLength--;
              lcd.print(seqLength);
              lcd.print(" ");
            }
            downPress = false;
          }
          if (buttonPress(BUTTON_RIGHT)) {rightPress = true;}
          else {
            if (rightPress) {
              change = CHARACTERS;
              printSymbolMenu();
            }
            rightPress = false;
          }
          if (buttonPress(BUTTON_SELECT)) {selPress = true;}
          else {
            if (selPress) {
              startGame();
            }
            selPress = false;
          }
          break;
          
        case CHARACTERS:
          if (buttonPress(BUTTON_UP)) {upPress = true;}
          else {
            if (upPress & symbols < 4) {
              lcd.setCursor(14,1);
              symbols++;
              lcd.print(symbols);
            }
            upPress = false;
          }
          if (buttonPress(BUTTON_DOWN)) {downPress = true;}
          else {
            if (downPress & symbols > 2) {
              lcd.setCursor(14,1);
              symbols--;
              lcd.print(symbols);
            }
            downPress = false;
          }
          if (buttonPress(BUTTON_LEFT)) {leftPress = true;}
          else {
            if (leftPress) {
              change = SEQUENCE;
              lcd.clear();
              printSequenceMenu();
            }
            leftPress = false;
          }
          if (buttonPress(BUTTON_RIGHT)) {rightPress = true;}
          else {
            if (rightPress) {
              change = TIMER;
              lcd.clear();
              printTimerMenu();
            }
            rightPress = false;
          }
          if (buttonPress(BUTTON_SELECT)) {selPress = true;}
          else {
            if (selPress) {
              startGame();
            }
            selPress = false;
          }
          break;
      
        case TIMER:
          if (buttonPress(BUTTON_UP)) {upPress = true;}
          else {
            if (upPress & seconds < 9) {
              lcd.setCursor(10,1);
              seconds++;
              lcd.print(seconds);
            }
            upPress = false;
          }
          if (buttonPress(BUTTON_DOWN)) {downPress = true;}
          else {
            if (downPress & seconds > 2) {
              lcd.setCursor(10,1);
              seconds--;
              lcd.print(seconds);
            }
            downPress = false;
          }
          if (buttonPress(BUTTON_LEFT)) {leftPress = true;}
          else {
            if (leftPress) {
              change = CHARACTERS;
              lcd.clear();
              printSymbolMenu();
            }
          leftPress = false;
          }
          if (buttonPress(BUTTON_SELECT)) {selPress = true;}
          else {
            if (selPress) {
              startGame();
            }
            selPress = false;
          }
          break;
      }
      break;

    case STORY:
    //Code to move up levels when game is being played in story mode.
      lcd.setBacklight(7);
      lcd.setCursor(0,0);
      lcd.print("LEVEL ");
      
      switch(level) {
        case 1:
          pass++;
          lcd.print("1.");
          lcd.print(pass);
          delay(500);
          if (seqLength < 8) {
            seqLength++;
            startGame();
          }
          else {
            seqLength = 5;
            symbols = 3;
            d = 0.9;
            level = 2;
            pass = 1;
            lcd.print("2.");
            lcd.print(pass);
            delay(500);
            startGame();
          }
          break;
        
        case 2:
          pass++;
          lcd.print("2.");
          lcd.print(pass);
          delay(500);
          if(seqLength < 9) {
            seqLength++;
            startGame();
          }
          else {
            seqLength = 6;
            symbols = 4;
            d = 0.8;
            level = 3;
            pass = 1;
            lcd.print("3.");
            lcd.print(pass);
            delay(500);
            startGame();
          }
          break;
        
        case 3:
          pass++;
          lcd.print("3.");
          lcd.print(pass);
          delay(500);
          if(seqLength < 10) {
            seqLength++;
            startGame();
          }
          else {
            lcd.clear();
            lcd.setBacklight(2);
            lcd.print("YOU BEAT STORY!!");
            delay(2000);
            state = START;
          }
          break;   
      }
      break;
      
    case GAME:
      //Code to create a sequence of symbols to display on the LCD screen.
      int sequence[seqLength];
      for (int i = 0; i < seqLength; i++) {
        sequence[i] = random(symbols);
      }
      for (int j = 0; j <seqLength; j++) {
        lcd.setCursor(0,0);
        lcd.write(sequence[j]);
        delay(d/2);
        lcd.clear();
        delay(d/2);
      }
      
      //Code to receive input form user in aim to recreate sequence created by computer.
      int input[seqLength];
      for (int x = 0; x < seqLength; x++) {
        waitingPress = true;
        now = millis();
        lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("Time:");
        while (waitingPress) {
          button_state = lcd.readButtons();

          //Code for an on-screen countdown
          int countdown = ((seconds*10)-(millis()-now)/100);
          lcd.setCursor(14,0);
          if (countdown <10 ) {
            lcd.print("  ");
            lcd.setCursor(15,0);
            lcd.print(countdown);
          }
          else {lcd.print(countdown);};
          
          if (buttonPress(BUTTON_UP)) {upPress = true;}
          else {
            if(upPress){
              input[x] = 0;
              waitingPress = false;
            }
            upPress = false;
          }
          if (buttonPress(BUTTON_DOWN)) {downPress = true;}
          else {
            if(downPress){
              input[x] = 1;
              waitingPress = false;
            }
            downPress = false;
          }
          if (buttonPress(BUTTON_LEFT)) {leftPress = true;}
          else {
            if(leftPress){
              input[x] = 3;
              waitingPress = false;
            }
            leftPress = false;
          }
          if (buttonPress(BUTTON_RIGHT)) {rightPress = true;}
          else {
            if(rightPress){
              input[x] = 2;
              waitingPress = false;
            }
            rightPress = false;
          }
          if ((millis() - now) > (seconds*1000)) {
            waitingPress = false;
            lcd.setCursor(14,0);
            lcd.print("  ");
          }
        } 
      }
      
      //Code to compare two seperate sequences, and react to comparison accordingly.
      boolean equal = true;
      for (int y = 0; y<seqLength; y++) {
        Serial.print(sequence[y]);
        Serial.print("---");
        Serial.println(input[y]);
        if (sequence[y] != input[y]) equal = false;
      }
      if (equal) {
        lcd.setBacklight(2);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CORRECT!");
        delay(2000);
      }
      else{
        lcd.setBacklight(1);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("UNLUCKY!");
        if (storyMode) {
          lcd.setCursor(0,1);
          int score = (level*pass)-1;
          lcd.print("Score: ");
          lcd.print(score);
        }
        delay(2000);
        lcd.clear();
      }
      if (storyMode & equal) {
        state = STORY;
      }
      else {state = START;}
      break;





  }
}
