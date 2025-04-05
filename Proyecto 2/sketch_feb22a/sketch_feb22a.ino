#include <Servo.h>  // Libreria para el servo motor
#include <TaskScheduler.h>

Servo servo;  // objeto de servo
Scheduler RealTimeCore;

// pines del HC-SR05
const int trigPin = 11;
const int echoPin = 12;

static unsigned long startTime = millis();


float angle_motor;    // angulo del servomotor
long duration;        // duracion de la trayec: HC-SR05
int distance;         // distancia: HC-SR05
int past_dist, past_dist2 = 0;

// --- parametros del controlador --- //

float uk = 0;
float uk_p, uk_1, uk_2 = 0.0;
float ek, ek_p, ek_1, ek_2 = 0;
float uik, uik_p, uik_1, uik_2 = 0;
float yk = 0;
float rkf = 25;
float rk = 25;

float ukm_1, ukmp = 0;
int senseAve = 0;
int senseSum = 0;
int i = 0;
bool goread = 0;
int numReads = 5;

void controlador();
void distanceread();
void mapfloat();
void escalon();

Task controltarea(100, TASK_FOREVER, &controlador, &RealTimeCore, true);  // freq: 1kHz
Task distancia(5, TASK_FOREVER, &distanceread, &RealTimeCore, true);    
Task escalontask(10000, TASK_FOREVER, &escalon, &RealTimeCore, true);   

void controlador() {
  distanceread();
  
  ek = rk - yk;
  uk = 1.47*uk_1 - 0.4737*uk_2 + 0.0553*ek - 0.103*ek_1 +0.0479*ek_2 ;
  
  float ukm = uk + 2;
  ukm = mapfloat(ukm, 1.5, 2.5, 30, 165);
  ukm = constrain(ukm, 30, 165) ;
  servo.write(ukm);

  uk_2 = uk_1;
  uk_1 = uk;
  uk_p = uk;

  ek_2 = ek_1;
  ek_1 = ek;
  ek_p = ek;
  ukm_1 = ukmp;
  ukmp = ukm;
  past_dist = past_dist2;
  past_dist2 = yk;
  
  Serial.print (ukm);    // limites: -7 a 7
  Serial.print(",");
  
  Serial.print (yk);    // limites: -7 a 7
  Serial.print(",");

  Serial.print (rk);    // limites: -7 a 7
  Serial.print(",");

  Serial.println();
}

void distanceread() {
  if (i > numReads and goread == 1 ){
    i = 0;
    goread = 0;
    senseSum = 0;
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (distance > 46) {
    distance = past_dist;
  }
  else if (distance < 3) distance = 3;
  if (i >= numReads) {
    //senseSum += distance;
    senseAve = senseSum / numReads;
    goread = 1;
  }
  else {
    senseSum += distance;
  }
  if (goread) 
  {
    yk = senseAve;
    yk = constrain(yk, 3, 39) ;
  }
  i = i + 1;
};

void escalon() {
   if (rk == 25) rk = 15;
   else if (rk == 15) rk = 25;
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(9);  // pin 9 del arduino PWM
  servo.write(angle_motor);
}

void loop() {
  RealTimeCore.execute();  // Ejecución de tareas con task scheduler
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
