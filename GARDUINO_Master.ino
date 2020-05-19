#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(A1,A0,A2,A3,A4,A5);
#include <Servo.h> 
Servo myservo;
int pos = 0;
int ledState = HIGH;         // the current state of the output pin
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 1000;    // the debounce time; increase if the output flickers

int TableMotorDir = 8;
int TableMotor = 9;
long previousMillis = 0;    
int TrapMotor = 22;
int WindowMoter = 24;
int In1 = 10;
int In2 = 11;

const int Stop = 0;
const int OpenWindow = 1;
const int CloseWindow = 2;
const int OpenTrap = 3;
const int CloseTrap = 4;

int TableLimitSelect = 7;
int TableLimitIndex = 6;
int WindowSensor = 5;
int MotorSpeed = 200;
const int sensor1 = A0;
const int sensor2 = A1;
const int triger = 4;
const int echo = 3;

void setup() {
    Serial.begin(9600); 
     myservo.attach(5);  // attaches the servo on pin 9 to the servo object 
     pinMode(WindowSensor, INPUT_PULLUP);
     pinMode(sensor1, INPUT_PULLUP);
     pinMode(sensor2, INPUT_PULLUP);
//     pinMode(slaveRESET, OUTPUT);
//     digitalWrite(slaveRESET, LOW);
      pinMode(triger, OUTPUT);
      pinMode(echo, INPUT_PULLUP);
     /////////////////////
     SPI.begin();
 //    pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
//     digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
     //pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
     //digitalWrite(NRSTPD, HIGH);
//     myRFID.AddicoreRFID_Init();  
//     digitalWrite(slaveRESET, HIGH);
     myservo.write(180);
     //delay(7000);
     Serial.println("system run");

}
void init0(){
      lcd.setCursor(0, 1);
      lcd.print("Select L  ");
      delay(1000);
      while (digitalRead(TableLimitSelect)== LOW){
                       delay(100);
     }
      lcd.setCursor(0, 1);
      lcd.print("Select H  ");
      delay(1000);
      while (digitalRead(TableLimitSelect)== HIGH){
                       delay(100);
     }
     /////////////////////////////
      lcd.setCursor(0, 1);
      lcd.print("Index L  ");
      delay(1000);
      while (digitalRead(TableLimitIndex)== LOW){
                       delay(100);
     }
      lcd.setCursor(0, 1);
      lcd.print("Index H  ");
      delay(1000);
      while (digitalRead(TableLimitIndex)== HIGH){
                       delay(100);
     }
      lcd.setCursor(0, 1);
      lcd.print("Index H 0K ");
  
}
void loop() {

   //analogWrite(In1, MotorSpeed);  
  int reading = digitalRead(WindowSensor);
  if (reading != lastButtonState) {
     lastDebounceTime = millis();
  } 
  if ((millis() - lastDebounceTime) > debounceDelay) {
      if ((reading == HIGH)&&(buttonState != reading)){
             /*lcd.setCursor(0, 1);
             lcd.print("Sensor Triggered    ");
             gearmotor(CloseWindow);
             delay(1200);
             //*/
             Serial.println("Window Open");
             
             debounceDelay = 1000;
      }
      else if ((reading == LOW)&&(buttonState != reading)){
             /*lcd.setCursor(0, 1);
             lcd.print("Open Window");
             gearmotor(OpenWindow);
             delay(1500);
             gearmotor(Stop);
             //*/
             Serial.println("Window Close");
             debounceDelay = 2000;
      }
      buttonState = reading;
  }
  lastButtonState = reading;
}
void gearmotor(int stat){
     if (stat == 1){
          digitalWrite(WindowMoter,HIGH);
          analogWrite(In1, 255); 
          analogWrite(In2, 0);   
     }
     else if (stat == 2){
          digitalWrite(WindowMoter,HIGH);
          analogWrite(In2,255);   
          analogWrite(In1, 0); 
     }
     else if (stat == 3){
          digitalWrite(TrapMotor,HIGH);
          analogWrite(In1, 200);   
          analogWrite(In2, 0); 
     }
     else if (stat == 4){
          digitalWrite(TrapMotor,HIGH);
          analogWrite(In2,200);   
          analogWrite(In1, 0); 
     }
     else {
          digitalWrite(WindowMoter,LOW);
          digitalWrite(TrapMotor,LOW);
          analogWrite(In1, 0); 
          analogWrite(In2, 0);  
     }
}
void tableSelect(int bincount){
           Serial.print("move to start/index" );
           lcd.setCursor(0, 1); //////////////////////////////
           lcd.print("move to index    ");
           delay(1000);
           boolean LimitIndexS = HIGH;
           digitalWrite(TableMotorDir,LOW);
           //////////////////////////
            int dly = 0;
            while(dly < 100){
                    if (digitalRead(TableLimitIndex)== LOW){
                           delay(1);
                           dly++;
                     }
                     else {
                           dly = 0; 
                           analogWrite(TableMotor, MotorSpeed);      
                     }
            } 
            /////////////////////////////
            Serial.print("motor stop table at index" );   
           lcd.setCursor(0, 1);////////////////////////////////
           lcd.print(" motor stop    "); 
            analogWrite(TableMotor, 0);  
            //motor off
            delay(1000);
            Serial.print("select bin" );
           lcd.setCursor(0, 1);/////////////////////////////
           lcd.print(" select bin    ");  
            digitalWrite(TableMotorDir,HIGH);  
    for (int bins = 0; bins < bincount; bins++) {
           ///motor on
            analogWrite(TableMotor, MotorSpeed);  
            if (bins != 0){
                     dly = 0;
                   while(dly < 100){
                          if (digitalRead(TableLimitSelect)== LOW){
                                 delay(1);
                                 dly++;
                           }
                           else {
                                 dly = 0; 
                           }
                  } 
                  delay(50);
            }
              dly = 0;
             while(dly < 100){
                    if (digitalRead(TableLimitSelect)== HIGH){
                           delay(1);
                           dly++;
                     }
                     else {
                           dly = 0; 
                     }
            } 
           //motor off 
           delay(50);
            
           analogWrite(TableMotor, 0);      
           Serial.print("bin = " );                       
           Serial.print(bins + 1); 
            lcd.setCursor(0, 1);//////////////////////////////////
            lcd.print("Bin =                ");
            lcd.setCursor(7, 1);
            lcd.print(bins + 1);
          
      }
           delay(700);
            lcd.setCursor(0, 1);//////////////////////////////////
            lcd.print("                     ");
            delay(1000);
}
int Scanning(){
        int senval = 0;
        delay(500);
      
            myservo.write(180); 
            delay(500);
            Serial.println("LCD Scanning    ");     
            for(pos = 0; pos < 180; pos += 1){                                  // in steps of 1 degree 
                    myservo.write(180 - pos);              // tell servo to go to position in variable 'pos' 
                    //delay(35); 
                    //Serial.print("val = ");                       
                   // Serial.println(pos);      
                   unsigned long currentMillis = millis(); 
                   previousMillis = currentMillis;
                    while(currentMillis - previousMillis < 35){
                          currentMillis = millis();
                          int reading = digitalRead(sensor2);
                          if (reading != lastButtonState) {
                                lastDebounceTime = millis();
                          } 
                          if ((millis() - lastDebounceTime) > 20) {
                                 buttonState = reading;
                          }
                          lastButtonState = reading;
                          if (buttonState == LOW){
                              Serial.print("LCD Reading = ");                       
                              Serial.println(pos);    
                              senval = pos;
                              pos = 180;
                             delay(3000);
                                   currentMillis = millis() + 35;
                                   /////////////////////////////
                                   if (senval < 54){
                                           if ((digitalRead(sensor1) == HIGH)) {
                                                  //Serial.println("LCD Chuckie = 2");                       
                                                  //BinV = 2;
                                                  Serial.println("LCD Can = 1");                       
                                           }
                                           else{
                                                  Serial.println("LCD Wrappers = 3");                       
                                           }
                                  }
                                   else  if ((senval >= 54 )&&(senval < 70)){////bin papers chuckie
                                          
                                           if ((digitalRead(sensor1) == HIGH)&&(senval < 60)) {
                                                  Serial.println("LCD Can = 1");                       
                                           }
                                           else if ((digitalRead(sensor1) == HIGH)) {
                                                  //Serial.println("LCD Chuckie = 2");                       
                                                  //BinV = 2;
                                                  Serial.println("LCD Can = 1");                       
                                                  
                                           }
                                           else{
                                                  Serial.println("LCD Wrappers = 3");                       
                                           }
                                  }
                                  else  if ((senval >= 80 )&&(senval < 105)){/////bin papers  if ((senval >= 107 )&&(senval < 126)
                                          Serial.println("LCD Papers = 2");                       
                                 }
                                  else  if ((senval >= 120 )&&(senval < 180)){
                                    /// bin plastic
                                          Serial.println("LCD Plastics  = 4");                       
                                  }
                                  else {
                                  Serial.println("LCD Unknown 4");  
                                  } 
                           }
                           else {                                    /// bin plastic
                                
                           }  
                    } 
              } 
        delay(1000);     
        myservo.write(180);        
}

