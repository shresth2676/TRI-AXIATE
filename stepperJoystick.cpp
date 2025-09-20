#define STEP_PIN 2
#define DIR_PIN 3

#define JOY_X A0     // Joystick X-axis analog input
#define JOY_BUTTON 7 // Optional button input

const int MAX_STEPS   = 200;    // Full stepper range
const int THRESHOLD   = 10;     // Ignore joystick noise
int STEP_DELAY        = 2000;   // Microseconds between pulses (slower speed)

long currentSteps = 0;           // Current stepper position
int prevJoy = 0;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(JOY_BUTTON, INPUT_PULLUP); // optional button

  prevJoy = analogRead(JOY_X);
}

void loop() {
  int currJoy = analogRead(JOY_X);

  // Only react if joystick moved enough
  if (abs(currJoy - prevJoy) > THRESHOLD) {
    // Map joystick to step range
    long targetSteps = map(currJoy, 0, 1023, 0, MAX_STEPS);
    long stepsToMove = targetSteps - currentSteps;

    if (stepsToMove != 0) {
      // Set direction
      digitalWrite(DIR_PIN, stepsToMove > 0 ? HIGH : LOW);

      // Move the required steps
      for (long i = 0; i < abs(stepsToMove); i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(STEP_DELAY);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(STEP_DELAY);
      }

      currentSteps = targetSteps;
    }

    prevJoy = currJoy;
  }

  // Optional: check joystick button
  if (digitalRead(JOY_BUTTON) == LOW) {
    // Do something when pressed (like reset position or stop)
  }
}
