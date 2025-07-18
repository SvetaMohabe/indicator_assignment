#define LEFT_BUTTON 2
#define RIGHT_BUTTON 3
#define LEFT_LED 9
#define RIGHT_LED 10

unsigned long previousMillis = 0;
const long blinkInterval = 300;

bool leftOn = false;
bool rightOn = false;
bool hazardOn = false;
bool ledState = false;

unsigned long leftPressStart = 0;
unsigned long rightPressStart = 0;

void setup() {
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  bool leftPressed = digitalRead(LEFT_BUTTON) == LOW;
  bool rightPressed = digitalRead(RIGHT_BUTTON) == LOW;

  // Button Press for 1 sec
  if (leftPressed) {
    if (leftPressStart == 0) leftPressStart = currentMillis;
    if (currentMillis - leftPressStart >= 1000) {
      handleLeftButton();
      leftPressStart = 0;
      delay(300); // debounce
    }
  } else leftPressStart = 0;

  if (rightPressed) {
    if (rightPressStart == 0) rightPressStart = currentMillis;
    if (currentMillis - rightPressStart >= 1000) {
      handleRightButton();
      rightPressStart = 0;
      delay(300); // debounce
    }
  } else rightPressStart = 0;

  // Blink logic
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    if (hazardOn || leftOn || rightOn) {
      ledState = !ledState;
      digitalWrite(LEFT_LED, (leftOn || hazardOn) && ledState);
      digitalWrite(RIGHT_LED, (rightOn || hazardOn) && ledState);
    }
  }
}

void handleLeftButton() {
  if (hazardOn) {
    hazardOn = false;
    Serial.println("Hazard OFF");
    return;
  }
  if (rightOn) {
    rightOn = false;
    leftOn = true;
    Serial.println("Switched to LEFT Indicator");
  } else {
    leftOn = !leftOn;
    Serial.print("Left Indicator ");
    Serial.println(leftOn ? "ON" : "OFF");
  }

  // Check for hazard
  if (rightPressStart != 0) {
    hazardOn = true;
    leftOn = false;
    rightOn = false;
    Serial.println("Hazard Mode ON");
  }
}

void handleRightButton() {
  if (hazardOn) {
    hazardOn = false;
    Serial.println("Hazard OFF");
    return;
  }
  if (leftOn) {
    leftOn = false;
    rightOn = true;
    Serial.println("Switched to RIGHT Indicator");
  } else {
    rightOn = !rightOn;
    Serial.print("Right Indicator ");
    Serial.println(rightOn ? "ON" : "OFF");
  }

  // Check for hazard
  if (leftPressStart != 0) {
    hazardOn = true;
    leftOn = false;
    rightOn = false;
    Serial.println("Hazard Mode ON");
  }
}

