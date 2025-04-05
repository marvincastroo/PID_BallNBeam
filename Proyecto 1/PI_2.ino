# define pi 3.1415926535
#define alpha 0.05 
#include <Servo.h> // Libreria para el servo motor
#include <TaskScheduler.h> 

Servo servo; // objeto de servo
Scheduler RealTimeCore; 

// pines del HC-SR05
const int trigPin = 11; 
const int echoPin = 12;
// pines del acelerometro  
const int ypin = A2;
static unsigned long startTime = millis();

unsigned int yvalue; // angulo y del acelerometro
float angle_motor; // angulo del servomotor
float accel_angle; // angulo del accel
long duration;  // duracion de la trayec: HC-SR05
int distance; // distancia: HC-SR05

// --- parametros del controlador --- //

float uk = 0;
float uk_p, uk_1, uk_2 = 0.0;
float ek, ek_p, ek_1, ek_2 = 0;
float uik, uik_p, uik_1, uik_2 = 0;
float yk = 0;
float rk = 0;
float umax = 180;
float umin = 0;

void controlador();
void accelread();


Task controltarea(100, TASK_FOREVER, &controlador, &RealTimeCore, true); // freq: 1kHz

void controlador(){
  accelread();
  rk = -10;
  ek = rk - yk;
  uk = uk_1 - 5.54*ek -3.143*ek_1;
  if (uk > 180) {uk = 180;
  
  }
  else if (uk < 0) uk = 0;
  servo.write(uk);
  
  // Guardar Valores Anteriores
  uk_2 = uk_1;
  uk_1 = uk_p;
  uk_p = uk;
  ek_2 = ek_1;
  ek_1 = ek_p;
  ek_p = ek;

  unsigned long currentTime = millis(); // Get the current time
  unsigned long elapsedTime = currentTime - startTime; // Calculate elapsed time

  // Convert milliseconds to seconds for easier reading
  unsigned long elapsedSeconds = elapsedTime;

  // Display the elapsed time
//  Serial.print("Elapsed Time: ");
//  Serial.print(elapsedSeconds);
//  Serial.println(" seconds");
  
}
void accelread(){
  yvalue = analogRead (ypin ) ;
  int y = map (yvalue , 269 , 403 , -100 , 100) ;
  accel_angle = (float ) y/(-100.00) ; 
  float angley = asin(accel_angle) * 180/pi;
  yk = angley;
  yk = (alpha*yk) + ((1-alpha)*yk);
  Serial.print("uk: ");
  Serial.print(uk);
  Serial.print(" , ");

  Serial.print("rk: ");
  Serial.print(rk);
  Serial.print(" , ");

  Serial.print("yk: ");
  Serial.print(yk);
  Serial.print(" , ");
  
  Serial.println();
}

void setup() {
  Serial.begin (9600) ;
  pinMode (ypin, INPUT ) ;
  servo.attach(9);    // pin 9 del arduino PWM
  servo.write(angle_motor); 
}

void loop() {
  RealTimeCore.execute(); // Ejecución de tareas con task scheduler
}
