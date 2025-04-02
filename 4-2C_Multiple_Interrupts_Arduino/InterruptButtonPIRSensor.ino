byte buttonPin = 3;
byte motionPin = 2;
byte ledButtonPin = 4;
byte ledMotionPin = 8;

bool buttonState = LOW;
bool motionState = LOW;

bool buttonStateChanged = false;

void setup() 
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(motionPin, INPUT);
  pinMode(ledButtonPin, OUTPUT);
  pinMode(ledMotionPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(motionPin), motionISR, RISING);
}

void buttonISR()
{
  buttonStateChanged = true;
}

void motionISR()
{
  motionState = HIGH;
}

bool readButtonState()
{
  return digitalRead(buttonPin);
}

bool readMotionState()
{
  return digitalRead(motionPin);
}

void buttonPressed()
{
  if (readButtonState() == LOW)
  {
    Serial.println("The button is pressed");
    digitalWrite(ledButtonPin, HIGH);
    delay(2000);
    buttonState = LOW;
  }
  else
    digitalWrite(ledButtonPin, LOW);
}

void motionDetected()
{
  if (readMotionState() == HIGH)
  {
    Serial.println("Movement detected!");
    digitalWrite(ledMotionPin, HIGH);
    delay(2000);
    motionState = LOW;
  }
  else
    digitalWrite(ledMotionPin, LOW);
}

void loop() 
{
  buttonPressed();
  motionDetected();
}
