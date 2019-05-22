/*
Arduino Car: very simple variant
*/

// US sensor
#define trig_pin 11
#define echo_pin 12

// motors
#define enA 10
#define in1 9
#define in2 8

#define enB 5
#define in3 7
#define in4 6

// speed of sound: cm/millisecond
#define SOS 0.0343
// constant for turning
// TODO: Wert anpassen, so dass er tatsächlich um diesen winkel dreht
#define C 5

// global variables
float view_direction = 0.0;
const float MIN_DIST = 10.0; // cm
// default speed: currently, we're running at full speed only. 
const int V0 = 255;
float current_dist;
int current_speed; // -255 to 255

// delay between loops 
int DELAY = 100;

void setup() {

  // left engine
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // left engine
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enB, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // sensor
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  // put your setup code here, to run once:
  // current_speed = 0;
  view_direction = 0.0;
  halte();
  setze_blickrichtung(view_direction);

}

float messe_abstand() {
  long duration;
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  return float(duration * SOS / 2);
}

// changes state of vehicle: make it move
void fahre(int v) {
  current_speed = v;
  motor_1(v);
  motor_2(v);
}

// changes state of vehicle: make it stop
void halte() {
  current_speed = 0;
  motor_1(0);
  motor_2(0);
}

// change state of vehicle: make it turn by winkel (degrees)
void drehe(int winkel) {
  if (winkel > 0) {
    motor_1(V0);
    motor_2(0);
  } else {
    motor_1(0);
    motor_2(V0);
  }
  delay(int(C*abs(winkel))); // debug
  motor_1(0);
  motor_2(0);
  current_speed = 0;
}

// not implemented yet
void setze_blickrichtung(float v) {
  
}

// set speed and drehsinn of first motor
// TODO: make just one function for both motors
void motor_1(int v) {
  if (v > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, v);
  } else if (v < 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, abs(v));
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
    analogWrite(enA, 0);
  }
}

// set speed and drehsinn of second motor
void motor_2(int v) {
  if (v > 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, v);
  } else if (v < 0) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, abs(v));
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);  
  }
}

// PROGRAMM 1: fahre geradeaus; wenn Hindernis: bleib stehen.
//void loop() {
//  // main:
//  d = messe_abstand(); // cm
//
//  if (d < MIN_DIST) {
//    setze_geschwindigkeit(0);
//    v = 0;
//  } else {
//    if (v == 0) {
//      setze_geschwindigkeit(v0);
//      v = v0;
//    }
//  }
//  delay(DELAY);
//}

// DADDEL
//void loop() {
//  int wait = 1000;
//  //fahre(255);
//  //delay(wait);
//  halte();
//  //fahre(-255);
//  //delay(wait);
//  drehe(-90);
//  delay(wait);
//  //halte();
//}

// PROGRAMM 2: fahre geradeaus. wenn Hindernis, drehe 90°
void loop() {
  // main:
  d = messe_abstand(); // cm

  if (d < MIN_DIST) {
    halte();
    delay(500);
    drehe(90);
  } else {    if (current_speed == 0) {
      fahre(V0);
    }
  }
  delay(DELAY);
}
