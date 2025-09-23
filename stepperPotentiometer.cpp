#define STEP_PIN 2
#define DIR_PIN 3
#define POT_PIN A0

const int MAX_STEPS   = 200;    // Full stepper range
const int THRESHOLD   = 2;      // Ignore small noise
int STEP_DELAY        = 2000;   // Microseconds between step pulses (increase = slower)

long currentSteps = 0;  // Current position
int prevPot = 0;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  prevPot = analogRead(POT_PIN);
}

void loop() {
  int currPot = analogRead(POT_PIN);

  if (abs(currPot - prevPot) > THRESHOLD) {
    long targetSteps = map(currPot, 0, 1023, 0, MAX_STEPS);
    long stepsToMove = targetSteps - currentSteps;

    if (stepsToMove != 0) {
      digitalWrite(DIR_PIN, stepsToMove > 0 ? HIGH : LOW);

      for (long i = 0; i < abs(stepsToMove); i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(STEP_DELAY);  // control speed
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(STEP_DELAY);
      }

      currentSteps = targetSteps;
    }

    prevPot = currPot;
  }
}
