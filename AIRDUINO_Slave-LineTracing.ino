
/*
 * Created by ASHIQ SHIBU (MAKER Q)
 * Project: Line following robot with obstacle avoidance
 * https://www.youtube.com/watch?v=Py2lBehF9rA
 */
  int vSpeed = 60;                      
  int turn_speed = 230;       // 0 - 255  max
  int t_p_speed = 125;
  int stop_distance = 12;
  int turn_delay = 10;


//HC-SR04 Sensor connection
  const int trigPin = 2;
  const int echoPin = 13;

//L293 Connection   
  const int motorA1      = 4;  
  const int motorA2      = 12; 
  const int motorAspeed  = 3;
  const int motorA2speed  = 11;
  const int motorB1      = 8; 
  const int motorB2      = 7; 
  const int motorBspeed  = 5;
  const int motorB2speed  = 6;

//Sensor Connection
  const int left_sensor_pin =9;
  const int right_sensor_pin =10;

  
  int turnspeed;
/]  int left_sensor_state;
  int right_sensor_state;

  long duration;
  int distance;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
    
  Serial.begin(9600);

  delay(3000);                              
  
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);


left_sensor_state = digitalRead(left_sensor_pin);
right_sensor_state = digitalRead(right_sensor_pin);



if(right_sensor_state == HIGH && left_sensor_state == LOW)
{
  Serial.println("turning right");

  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,HIGH);

  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, turn_speed);
  analogWrite (motorA2speed, vSpeed);
  analogWrite (motorB2speed, turn_speed);
  
  }
if(right_sensor_state == LOW && left_sensor_state == HIGH)
{
  Serial.println("turning left");
  
  digitalWrite (motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite (motorB1,HIGH);
  digitalWrite(motorB2,LOW);

  analogWrite (motorAspeed, turn_speed);
  analogWrite (motorBspeed, vSpeed);
  analogWrite (motorA2speed, turn_speed);
  analogWrite (motorB2speed, vSpeed);

  delay(turn_delay);
  }

if(right_sensor_state == LOW && left_sensor_state == LOW)
{
  Serial.println("going forward");

  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite (motorB1,HIGH);
  digitalWrite(motorB2,LOW);

  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  analogWrite (motorA2speed, vSpeed);
  analogWrite (motorB2speed, vSpeed);


  delay(turn_delay);
  
  }

if(right_sensor_state == HIGH && left_sensor_state == HIGH)
{ 
  Serial.println("stop");
  
  analogWrite (motorAspeed, 0);
  analogWrite (motorBspeed, 0);
  analogWrite (motorA2speed, 0);
  analogWrite (motorB2speed, 0);
  while(true){
  
 }
 
  }

  if(distance <= stop_distance)
 {

  analogWrite (motorAspeed, 0);
  analogWrite (motorBspeed, 0);
  analogWrite (motorA2speed, 0);
  analogWrite (motorB2speed, 0);
  while(true){

}

  digitalWrite (motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  delay(500); //100
  
  digitalWrite (motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite (motorB1,HIGH);
  digitalWrite(motorB2,LOW);
  delay (500);

  }
}
