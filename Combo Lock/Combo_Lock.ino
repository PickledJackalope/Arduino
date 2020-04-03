 
  // Pin definitions.
  #define GREEN         2
  #define YELLOW        3
  #define RED           4
  #define BUTTON        13
  const int DIGITS[8] = {5,6,7,8,9,10,11,12};

  // Global variable initializations.
  bool combo[8]       = {0,0,0,0,0,0,0,0};
  int mode            = 0;

  // Modes
  const int UNLOCKED  = 97;
  const int MODIFY    = 98;
  const int LOCKED    = 99;

void setup() {
  Serial.begin(9600);
  
  // Pin Setup
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  for(int i = 0; i < 8; i++) {
    pinMode(DIGITS[i], INPUT);
  }
  pinMode(BUTTON, INPUT);

 // Lock initialization;
 mode = MODIFY;
}

void loop() {
  if(mode == UNLOCKED) {
    mode = unlocked();
  }
  else if(mode == MODIFY) {
    mode = modify();
  }
  else if(mode == LOCKED) {
    mode = locked();
  }
}


int unlocked() {  // Maintains unlocked state until button is pressed. Returns mode to switch to.
  greenLight();
  int buttonPress = getButtonPress();
  if (buttonPress == 1) {
    return LOCKED;
  }
  if (buttonPress == 2) {
    return MODIFY;
  }
}

int locked() {   // Checks for correct input when button is pressed. Returns mode to switch to.
  redLight();
  getButtonPress();
  bool unlock = true;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(DIGITS[i]) != combo[i]){
      unlock = false;
    }
  }
  if (unlock) {
    return UNLOCKED;
  }
  redFlashOn();
  return LOCKED;
}

int modify() {  // Modifies combo based on user input of DIGITS pins. Returns mode to switch to.
  yellowLight();
  getButtonPress();
  for(int i = 0; i < 8; i++) {
    combo[i] = digitalRead(DIGITS[i]); 
    Serial.print(combo[i]);
  }
  Serial.print('\n');
  return UNLOCKED;
}

int getButtonPress() {
  while(!digitalRead(BUTTON)){}
  unsigned long startTime = millis();
  while(digitalRead(BUTTON)){}
  unsigned long totalTime = millis() - startTime;
  if (totalTime < 3000) {
    return 1;
  }
  else {
    return 2;
  }
}



void greenLight() {
  digitalWrite(GREEN,  HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED,    LOW);
}
void yellowLight() {
  digitalWrite(GREEN,  LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED,    LOW);
}
void redLight() {
  digitalWrite(GREEN,  LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED,    HIGH);
}
void redFlashOn() {
  int counter = 0;
  while (counter < 20) {
    digitalWrite(RED, LOW);
    delay(50);
    digitalWrite(RED, HIGH);
    delay(50);
    counter++;
  }
}
