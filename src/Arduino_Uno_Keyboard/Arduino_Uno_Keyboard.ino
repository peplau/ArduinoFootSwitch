// PINS SETUP
const int pinScrollDown   = 8; // SCROLL DOWN
const int pinScrollUp     = 9; // SCROLL UP
const int pinTurnPrev     = 2; // TURN PREVIOUS
const int pinTurnNext     = 3; // TURN NEXT
const int pinTurnSwitch   = 4; // Turn switch
const int pinScrollSwitch = 5; // Scroll switch
const int pinLed          = 6; // Led

// PINS STATES
int stateUp;
int stateDown;
int stateNext;
int statePrev;

// Key press state
bool isUpPressed = false;
bool isDownPressed = false;
bool isNextPressed = false;
bool isPrevPressed = false;

// Keyboard buffer
uint8_t buf[8] = { 0 }; 

// SETUP METHOD
void setup() {
  Serial.begin(9600);

  pinMode(pinScrollDown, INPUT_PULLUP);
  pinMode(pinScrollUp, INPUT_PULLUP);
  pinMode(pinTurnPrev, INPUT_PULLUP);
  pinMode(pinTurnNext, INPUT_PULLUP);
  pinMode(pinTurnSwitch, INPUT_PULLUP);
  pinMode(pinScrollSwitch, INPUT_PULLUP);
  pinMode(pinLed, INPUT_PULLUP);

  // Read initial state
  stateUp   = digitalRead(pinScrollUp);
  stateDown = digitalRead(pinScrollDown);
  stateNext = digitalRead(pinTurnNext);
  statePrev = digitalRead(pinTurnPrev);
}

// LOOP METHOD
void loop() {
  // Any key changes must release all keys and execute the key combinations
  if (digitalRead(pinScrollUp) != stateUp 
      || digitalRead(pinScrollDown) != stateDown 
      || digitalRead(pinTurnNext) != stateNext 
      || digitalRead(pinTurnPrev) != statePrev) {

    // Read current state
    stateUp   = digitalRead(pinScrollUp);
    stateDown = digitalRead(pinScrollDown);
    stateNext = digitalRead(pinTurnNext);
    statePrev = digitalRead(pinTurnPrev);
    isUpPressed = stateUp == LOW;
    isDownPressed = stateDown == LOW;
    isNextPressed = stateNext == LOW;
    isPrevPressed = statePrev == LOW;

    // Release all keys
    releaseKeys(); 

    // Execute the combinations

    // 1 keys only
    if (!isPrevPressed && !isNextPressed && isUpPressed && !isDownPressed) {
      handleUp();
    } else if (!isPrevPressed && !isNextPressed && !isUpPressed && isDownPressed) {
      handleDown();
    } else if (!isPrevPressed && isNextPressed && !isUpPressed && !isDownPressed) {
      handleNext();
    } else if (isPrevPressed && !isNextPressed && !isUpPressed && !isDownPressed) {
      handlePrevious();
    }
    // 2 keys combinations
    else if (isPrevPressed && isNextPressed && !isUpPressed && !isDownPressed) {
      handleFullscreen();
    } else if (!isPrevPressed && !isNextPressed && isUpPressed && isDownPressed) {
      handleEnter();
    } else if (!isPrevPressed && isNextPressed && isUpPressed && !isDownPressed) {
      handleTab();
    }
    // 3 keys combinations
    else if (isPrevPressed && isNextPressed && isUpPressed && !isDownPressed) {
      handleShiftTab();
    } else if (!isPrevPressed && isNextPressed && isUpPressed && isDownPressed) {
      handleCloseWindow();
    }
    // 4 keys combinations
    else if (isPrevPressed && isNextPressed && isUpPressed && isDownPressed) {
      handleShutdown();
    }
  }

  delay(50);
}

void handleDown(){
  if (digitalRead(pinScrollSwitch) == LOW)
    buf[2] = 81; // DOWN
  else 
    buf[2] = 78; // PageDown
  Serial.write(buf, 8); 
}
void handleUp(){
    if (digitalRead(pinScrollSwitch) == LOW)
      buf[2] = 82; // UP
    else
      buf[2] = 75; // PageUp
    Serial.write(buf, 8); 
}
void handlePrevious(){
    if (digitalRead(pinTurnSwitch) == LOW){
      buf[2] = 224; // CTRL
      buf[3] = 75;  // PgUp
    }
    else {
      buf[2] = 224; // CTRL
      buf[3] = 225; // SHIFT
      buf[4] = 43;  // TAB
    }
    Serial.write(buf, 8); 
}
void handleNext(){
    if (digitalRead(pinTurnSwitch) == LOW){
      buf[2] = 224; // CTRL
      buf[3] = 78;  // PgDown
    }
    else {
      buf[2] = 224; // CTRL
      buf[3] = 43;  // TAB
    }
    Serial.write(buf, 8); 
}

void handleFullscreen(){
    buf[2] = 68; // F11
    Serial.write(buf, 8); 
}

void handleEnter(){
    buf[2] = 40; // ENTER
    Serial.write(buf, 8); 
}

void handleTab(){
    buf[2] = 43; // TAB
    Serial.write(buf, 8); 
}

void handleShutdown(){
    buf[2] = 224; // CTRL
    buf[3] = 226; // ALT
    buf[4] = 76;  // DEL
    Serial.write(buf, 8); 
}

void handleCloseWindow(){
    buf[2] = 226; // ALT
    buf[3] = 61;  // F4
    Serial.write(buf, 8); 
}

void handleShiftTab(){
    buf[2] = 225; // SHIFT
    buf[3] = 43;  // TAB
    Serial.write(buf, 8); 
}


// Function for Key Release
// Clear buffer and keys pressed
void releaseKeys() {
  for (int i=0; i<8; i++)
    buf[i] = 0;
  Serial.write(buf, 8); 
}