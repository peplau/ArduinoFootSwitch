uint8_t buf[8] = { 0 }; 

const int pinScrollDown   = 8; // SCROLL DOWN
const int pinScrollUp     = 9; // SCROLL UP
const int pinTurnPrev     = 2; // TURN PREVIOUS
const int pinTurnNext     = 3; // TURN NEXT
const int pinTurnSwitch   = 4; // Turn switch
const int pinScrollSwitch = 5; // Scroll switch
const int pinLed          = 6; // Led

bool isUpPressed = false;
bool isDownPressed = false;
bool isNextPressed = false;
bool isPrevPressed = false;
bool isShiftTabPressed = false;
bool isCloseWindowPressed = false;
bool isShutdownPressed = false;
bool isFullscreenPressed = false;
bool isEnterPressed = false;
bool isTabPressed = false;

void setup() {
  Serial.begin(9600);

  pinMode(pinScrollDown, INPUT_PULLUP);
  pinMode(pinScrollUp, INPUT_PULLUP);
  pinMode(pinTurnPrev, INPUT_PULLUP);
  pinMode(pinTurnNext, INPUT_PULLUP);
  pinMode(pinTurnSwitch, INPUT_PULLUP);
  pinMode(pinScrollSwitch, INPUT_PULLUP);
  pinMode(pinLed, INPUT_PULLUP);
}

void loop() {
  // 4 keys combinations
  handleShutdown();

  // 3 keys combinations
  handleShiftTab();
  handleCloseWindow();

  // 2 keys combinations
  handleFullscreen();
  handleEnter();
  handleTab();

  // 1 keys
  handleDown();
  handleUp();
  handlePrevious();
  handleNext();

  delay(50);
}

// 4 keys combinations

void handleShutdown(){
  if (digitalRead(pinScrollDown) == LOW && digitalRead(pinScrollUp) == LOW && digitalRead(pinTurnNext) == LOW && digitalRead(pinTurnPrev) == LOW && !isShutdownPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isShutdownPressed = true;
    isDownPressed = true;
    isUpPressed = true;
    isNextPressed = true;
    isPrevPressed = true;
    buf[2] = 224; // CTRL
    buf[3] = 226; // ALT
    buf[4] = 76;  // DEL
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollDown) == HIGH && digitalRead(pinScrollUp) == HIGH && digitalRead(pinTurnNext) == HIGH && digitalRead(pinTurnPrev) == HIGH && isShutdownPressed)
    releaseKey();
}

// 3 keys combinations

void handleCloseWindow(){
  if (digitalRead(pinScrollDown) == LOW && digitalRead(pinTurnNext) == LOW && digitalRead(pinTurnPrev) == LOW && !isCloseWindowPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isCloseWindowPressed = true;
    isDownPressed = true;
    isNextPressed = true;
    isPrevPressed = true;
    buf[2] = 226; // ALT
    buf[3] = 61;  // F4
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollDown) == HIGH && digitalRead(pinTurnNext) == HIGH && digitalRead(pinTurnPrev) == HIGH && isCloseWindowPressed)
    releaseKey();
}

void handleShiftTab(){
  if (digitalRead(pinScrollUp) == LOW && digitalRead(pinScrollDown) == LOW && digitalRead(pinTurnNext) == LOW && !isShiftTabPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isShiftTabPressed = true;
    isUpPressed = true;
    isDownPressed = true;
    isNextPressed = true;
    buf[2] = 225; // SHIFT
    buf[3] = 43;  // TAB
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollUp) == HIGH && digitalRead(pinScrollDown) == HIGH && digitalRead(pinTurnNext) == HIGH && isShiftTabPressed)
    releaseKey();
}

// 2 keys combinations

void handleTab(){
  if (digitalRead(pinScrollDown) == LOW && digitalRead(pinTurnNext) == LOW && !isTabPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isTabPressed = true;
    isDownPressed = true;
    isNextPressed = true;
    buf[2] = 43; // TAB
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollDown) == HIGH && digitalRead(pinTurnNext) == HIGH && isTabPressed)
    releaseKey();
}

void handleEnter(){
  if (digitalRead(pinTurnPrev) == LOW && digitalRead(pinTurnNext) == LOW && !isEnterPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isEnterPressed = true;
    isPrevPressed = true;
    isNextPressed = true;
    buf[2] = 40; // ENTER
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinTurnPrev) == HIGH && digitalRead(pinTurnNext) == HIGH && isEnterPressed)
    releaseKey();
}

void handleFullscreen(){
  if (digitalRead(pinScrollDown) == LOW && digitalRead(pinScrollUp) == LOW && !isFullscreenPressed) {
    releaseKey(); // Combinations must nullify anything previously pressed
    isFullscreenPressed = true;
    isUpPressed = true;
    isDownPressed = true;
    buf[2] = 68; // F11
    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollDown) == HIGH && digitalRead(pinScrollUp) == HIGH && isFullscreenPressed)
    releaseKey();
}

// 1 keys

void handleDown(){
  if (digitalRead(pinScrollDown) == LOW && !isDownPressed) {
    isDownPressed = true;

    if (digitalRead(pinScrollSwitch) == LOW)
      buf[2] = 81; // DOWN
    else 
      buf[2] = 78; // PageDown

    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollDown) == HIGH && isDownPressed)
    releaseKey();
}

void handleUp(){
  if (digitalRead(pinScrollUp) == LOW && !isUpPressed) {
    isUpPressed = true;

    if (digitalRead(pinScrollSwitch) == LOW)
      buf[2] = 82; // UP
    else
      buf[2] = 75; // PageUp

    Serial.write(buf, 8); 
  }
  else if (digitalRead(pinScrollUp) == HIGH && isUpPressed)
    releaseKey();
}

void handlePrevious(){
  if (digitalRead(pinTurnPrev) == LOW && !isPrevPressed) {
    isPrevPressed = true;

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
  else if (digitalRead(pinTurnPrev) == HIGH && isPrevPressed)
    releaseKey();
}

void handleNext(){
  if (digitalRead(pinTurnNext) == LOW && !isNextPressed) {
    isNextPressed = true;

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
  else if (digitalRead(pinTurnNext) == HIGH && isNextPressed)
    releaseKey();
}

// Function for Key Release
void releaseKey() {
  // Clear buffer and keys pressed
  for (int i=0; i<8; i++)
    buf[i] = 0;
  Serial.write(buf, 8); 

  // Clear all pressed keys
  isUpPressed = false;
  isDownPressed = false;
  isNextPressed = false;
  isPrevPressed = false;
  isShiftTabPressed = false;
  isCloseWindowPressed = false;
  isShutdownPressed = false;
  isFullscreenPressed = false;
  isEnterPressed = false;
  isTabPressed = false;
}