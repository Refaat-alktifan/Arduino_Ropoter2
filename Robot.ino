#include <Servo.h>
#include <NewPing.h>

const int speed_A = 11;  // PWM an den Pins: 3, 5, 6, 9, 10, 11. 9 u. 10 sind durch Servo-Library blockiert!
const int speed_B = 5;   // PWM 

// Servo-Drehung rechts => Servowinkel < 90 Grad

//Pins für Motor A 
const int MotorA1 = 8;
const int MotorA2 = 7;   
 
//Pins für Motor B 
const int MotorB1 = 6;
const int MotorB2 = 12;

//Pins für den ultrasonic sensor
const int trigger=A3;
const int echo=A4;

const int speedTrimm = A6;         // Einstellung der Geschwindigkeit 
const int  MAX_DISTANCE = 200;     // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
const int  MinDistance = 25;       // Minimaldistanz zum Objekt in gerader Fahrtrichtung         
const int  Servo_rechts = 10;
const int  Servo_links = 170;
const int  FullSpeed = 220;
const int  TurnSpeed = 200;
const int Trimm = 10;             // Servo-Trimm
const int to_Front = 90+Trimm;    // exakte 90-Grad Position 
int DangerPos;
int Distance;
int leftscanval, centerscanval, rightscanval, ldiagonalscanval, rdiagonalscanval;
char choice;

int servopin = 9;

int val;
int sum = 0;

NewPing sonar(trigger, echo, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo myServo;

void setup(){
  myServo.attach(servopin);
  myServo.write(to_Front);
  pinMode(MotorA1, OUTPUT); 
  pinMode(MotorA2, OUTPUT); 
  pinMode(MotorB1, OUTPUT); 
  pinMode(MotorB2, OUTPUT);
  pinMode(trigger,OUTPUT);
  pinMode(speed_A,OUTPUT);
  pinMode(speed_B,OUTPUT);
  pinMode(echo,INPUT);

  digitalWrite(MotorA1,LOW);
  digitalWrite(MotorA2,LOW);
  digitalWrite(MotorB1,LOW);
  digitalWrite(MotorB2,LOW);
  digitalWrite(trigger,LOW);
  analogWrite(speed_A, FullSpeed);   // rechter Motor 
  analogWrite(speed_B, FullSpeed);   // linker Motor
//  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
//  while (MinDistance > watch()) 
  { delay(100); }
}

int watch(){
  byte n = 0;
  long dist = 0;
  long howfar = 0;
  while (n < 5){      // Mittelwert aus 5 Messungen
    while (howfar < 1) { howfar = sonar.ping_cm();} // 0-Werte übergehen
    dist += howfar;
    n = n+1;
  }
  return round(dist / 5);
}

void setSpeed_A(int speed_M){
  analogWrite(speed_A, speed_M);      // rechter Motor 
}

void setSpeed_B(int speed_M){
  analogWrite(speed_B, speed_M);      // linker Motor
} 

void go(){   // Fahrt geradeaus
//   analogWrite(speed_A, speedA());   // rechter Motor 
   setSpeed_A(FullSpeed);
   setSpeed_B(FullSpeed);
   digitalWrite (MotorA1, HIGH);                              
   digitalWrite (MotorA2, LOW); 
   digitalWrite (MotorB1, HIGH); 
   digitalWrite (MotorB2, LOW);
}

void goback(){
  digitalWrite (MotorA1, LOW);                              
  digitalWrite (MotorA2, HIGH); 
  digitalWrite (MotorB1, LOW); 
  digitalWrite (MotorB2, HIGH);
}

void turnright(){
  digitalWrite (MotorA1, LOW);                              
  digitalWrite (MotorA2, HIGH); 
  digitalWrite (MotorB1, HIGH); 
  digitalWrite (MotorB2, LOW);
}

void turnleft(){
  digitalWrite (MotorA1, HIGH);                              
  digitalWrite (MotorA2, LOW); 
  digitalWrite (MotorB1, LOW); 
  digitalWrite (MotorB2, HIGH);
}  

void stopmove(){
  digitalWrite (MotorA1 ,LOW);                              
  digitalWrite (MotorA2, LOW); 
  digitalWrite (MotorB1, LOW); 
  digitalWrite (MotorB2, LOW);
}  

int BestPos(){
  stopmove();
  int ServoPos;
  int Dist;
  int MinimumDistance = MinDistance;
  int MinServoPos = 0;
  for (ServoPos = Servo_rechts; ServoPos <= Servo_links; ServoPos++){
    myServo.write(ServoPos);
    delay(10);
    Dist = watch();
    if (Dist < MinimumDistance){
      MinimumDistance = Dist;
      MinServoPos = ServoPos;
    }
  }
  return (MinServoPos);
}

byte ServoPosition = to_Front;
boolean turnServo = HIGH;

void driveForward(){
  setSpeed_A(FullSpeed);
  setSpeed_B(FullSpeed);
  go();
  if (turnServo == LOW) ServoPosition++; 
  if (turnServo == HIGH) ServoPosition--; 
  if (ServoPosition > Servo_links) turnServo = HIGH;
  if (ServoPosition < Servo_rechts) turnServo = LOW;
}

void Turn (boolean Direction){
  stopmove();
  delay(10);
  setSpeed_A(TurnSpeed);
  setSpeed_B(TurnSpeed);
//  l_Blinker(1);
  
  if (Direction == HIGH) { 
    turnright(); 
    while (MinDistance > watch()) { delay(20); }
  }
  else { 
    turnleft(); 
    while (MinDistance > watch()) { delay(20); }
  }
  stopmove();
  setSpeed_A(FullSpeed);
  setSpeed_B(FullSpeed);
//  l_Blinker(0);

  delay(100);
} 

void loop(){
  if (watch() < MinDistance){
    stopmove();
    delay(20);
    goback();
    delay(500);
    DangerPos = BestPos();
    if(DangerPos <= 90)  Turn (LOW); 
    if(DangerPos > 90)  Turn (HIGH); 
  }
  go();
  delay(100);
}
