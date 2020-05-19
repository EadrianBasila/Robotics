#include <SD.h> 
#include <SPI.h>

const int chipSelect = 4;
const bool debug = true;

#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;
int pos = 0;
SoftwareSerial Serial1(10, 9); // Software RX, TX
char col;
unsigned int PMSa = 0, FMHDSa = 0, TPSa = 0, HDSa = 0, PMSb = 0, FMHDSb = 0, TPSb = 0, HDSb = 0;
unsigned int FMHDS = 0, TPS = 0, HDS = 0, CR1 = 0, CR2 = 0;
volatile unsigned int PMS = 0;
unsigned char buffer_RTT[40] = {};   //Serial buffer; Received Data
char tempStr[15];
char hmdStr[15];



void setup()
{

  //Actual Air quality processes
  pinMode(2, OUTPUT);  // Buzzer
  pinMode(3,OUTPUT); 

  // LED
  pinMode(6, OUTPUT);  //Green
  pinMode(7, OUTPUT);  //Red
  pinMode(8, OUTPUT);  //Blue
  
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  
  digitalWrite(2, LOW);
  myservo.attach(3);
  myservo.write(99);
  delay(4000);
  myservo.write(10);
  delay(4000);
  myservo.detach();
  Serial.begin(9600);
  Serial1.begin(9600);

  /*// For SD Card Write Function
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

*/


}

void loop()
{

  
  while (!Serial1.available());
  while (Serial1.available() > 0)     //Data check: weather there is any Data in Serial1
  {
    for (int i = 0; i < 40; i++)
    {
      col = Serial1.read();
      buffer_RTT[i] = (char)col;
      delay(2);
    }

    Serial1.flush();

    CR1 = (buffer_RTT[38] << 8) + buffer_RTT[39];
    CR2 = 0;
    for (int i = 0; i < 38; i++)
      CR2 += buffer_RTT[i];
    if (CR1 == CR2)                        //Check
    {
      PMSa = buffer_RTT[12];       //Read PM2.5 High 8-bit
      PMSb = buffer_RTT[13];       //Read PM2.5 Low 8-bit
      PMS = (PMSa << 8) + PMSb;    //PM2.5 value
      FMHDSa = buffer_RTT[28];       //Read Formaldehyde High 8-bit
      FMHDSb = buffer_RTT[29];       //Read Formaldehyde Low 8-bit
      FMHDS = (FMHDSa << 8) + FMHDSb; //Formaldehyde value
      TPSa = buffer_RTT[30];        //Read Temperature High 8-bit
      TPSb = buffer_RTT[31];        //Read Temperature Low 8-bit
      TPS = (TPSa << 8) + TPSb;  //Temperature value
      HDSa = buffer_RTT[32];        //Read Humidity High 8-bit
      HDSb = buffer_RTT[33];        //Read Humidity Low 8-bit
      HDS = (HDSa << 8) + HDSb; //Humidity value
    }
    else
    {
      PMS = 0;
      FMHDS = 0;
      TPS = 0;
      HDS = 0;
    }
  }

  Serial.println("-----------------------uart--------------------------");
  Serial.print("Temp : ");
  sprintf(tempStr, "%d%d.%d", TPS / 100, (TPS / 10) % 10, TPS % 10);
  Serial.print(tempStr);
  Serial.println(" C");              //Serial pring Temperature
  Serial.print("RH   : ");
  sprintf(hmdStr, "%d%d.%d", HDS / 100, (HDS / 10) % 10, HDS % 10);
  Serial.print(hmdStr);          //Serial print humidity
  Serial.println(" %");
  Serial.print("HCHO : ");
  Serial.print(FMHDS);
  Serial.println(" ug/m3");       //Serial print formaldehyde, unit: ug/m³


  // If PMS fluctuates more than the specified threshold, it'd turn on the airway valves, the air vac, and the negative-ion generator


  Serial.print("PM2.5: ");
  Serial.print(PMS);
  Serial.println(" ug/m3");       //Serial print PM2.5, unit: ug/m³
  Serial.println();

  /*
  File dataFile = SD.open("AirQualityLog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(tempStr);
    dataFile.print(",");
    dataFile.print(hmdStr);
    dataFile.print(",");
    dataFile.print(FMHDS);
    dataFile.println("");
    dataFile.close();
                }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
        }

*/





  if (PMS  > 10) {
    digitalWrite(2, HIGH); 
      
    digitalWrite(8, HIGH);
      
    digitalWrite(7, LOW);
    
    Serial1.end();
    myservo.attach(3);
    myservo.write(50);                  // sets the servo position according to the scaled value
    delay(5000);
    myservo.detach();
    Serial1.begin(9600);
    delay(100);
  }

  else if (PMS < 10) {
    digitalWrite(2, LOW);
    
    digitalWrite(8, HIGH);
     
    digitalWrite(6, LOW);
  
  }








}
