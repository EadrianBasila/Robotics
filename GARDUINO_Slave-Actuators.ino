
#include <LiquidCrystal.h>
LiquidCrystal lcd(A1,A0,A2,A3,A4,A5);

int ledState = HIGH;         // the current state of the output pin
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 1000;    // the debounce time; increase if the output flickers

int TableMotorDir = 8;
int TableMotor = 9;

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

void setup() {
    Serial.begin(9600); 
    pinMode(TableLimitSelect, INPUT_PULLUP);
    pinMode(TableLimitIndex, INPUT_PULLUP);
    pinMode(WindowSensor, INPUT);
    
    pinMode(WindowMoter, OUTPUT);
    pinMode(TableMotorDir, OUTPUT);
    pinMode(TrapMotor, OUTPUT);
    analogWrite(In1,0);
    analogWrite(In2,0);
    
    lcd.begin(20, 4);
    lcd.print("GarDuino");
    delay(1000);
    //init0();
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
             lcd.setCursor(0, 1);
             lcd.print("Sensor Triggered    ");
             gearmotor(CloseWindow);
             delay(1200);
             gearmotor(Stop);
             delay(1000);
             
             //sensor task

             tableSelect(3);
             
             gearmotor(OpenTrap);
             delay(1200);
             gearmotor(Stop);
             delay(1000);
             
             gearmotor(CloseTrap);
             delay(1200);
             gearmotor(Stop);
             delay(1200);

            // Ultrasonic Sonar
             
             debounceDelay = 1000;
      }
      else if ((reading == LOW)&&(buttonState != reading)){
             lcd.setCursor(0, 1);
             lcd.print("Open Window");
             gearmotor(OpenWindow);
             delay(1500);
             gearmotor(Stop);
             debounceDelay = 3000;
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
