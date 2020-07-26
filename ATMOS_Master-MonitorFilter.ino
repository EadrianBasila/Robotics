
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

char col;
unsigned int PMSa = 0, FMHDSa = 0, TPSa = 0, HDSa = 0, PMSb = 0, FMHDSb = 0, TPSb = 0, HDSb = 0;
unsigned int FMHDS = 0, TPS = 0, HDS = 0, CR1 = 0, CR2 = 0;
volatile unsigned int PMS = 0;
unsigned char buffer_RTT[40] = {};   //Serial buffer; Received Data
char tempStr[15];
char hmdStr[15];

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

File myFile;
int pinCS = 53;
int condition;

const int PinAirVac = 45;
const int buttonPinAir = 46;
const int buttonPinTemp = 47;
const int buttonPinHumid = 48;
const int buttonPinFMHD = 49;

int buttonStateA = 0;
int buttonStateB = 0;
int buttonStateC = 0;
int buttonStateD = 0;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(pinCS, OUTPUT);  //SD Card
  
  pinMode(46, INPUT);  //Displays Air Particulates
  pinMode(47, INPUT);  //Displays Temperature
  pinMode(48, INPUT);  //Displays Humidity
  pinMode(49, INPUT);  //Displays Formaldehyde Concentration

  pinMode(44, OUTPUT);  //AirVac
  pinMode(45, OUTPUT);  //Buzzer
  pinMode(26, OUTPUT);  //Blue
  pinMode(24, OUTPUT);  //Red
  pinMode(22, OUTPUT);  //Green

  digitalWrite(44, HIGH);
  
  // 12c OLED Config
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  //OLED Splash Screen 
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 0);
      display.print("COVID-19");
      display.setTextSize(1);
      display.setCursor(15, 15);
      display.print("Monitoring Device");
      display.setTextSize(1);
      display.setCursor(15, 24);
      display.print("  Ver. 1.0     ");
      display.display();
      delay(3000);

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(15, 20);
      display.print("Preparing Device...");
      display.display();
      delay(3000);
      display.clearDisplay();
      condition = 0;

   if (SD.begin()){
    Serial.println("SD card is ready to use.");
   } 
   
   else {
    Serial.println("SD card initialization failed");
    return;
   }  

      
  digitalWrite(26, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(22, LOW);
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
      PMSa = buffer_RTT[12];          //Read PM2.5 High 8-bit
      PMSb = buffer_RTT[13];          //Read PM2.5 Low 8-bit
      PMS = (PMSa << 8) + PMSb;       //PM2.5 value
      FMHDSa = buffer_RTT[28];        //Read Formaldehyde High 8-bit
      FMHDSb = buffer_RTT[29];        //Read Formaldehyde Low 8-bit
      FMHDS = (FMHDSa << 8) + FMHDSb; //Formaldehyde value
      TPSa = buffer_RTT[30];          //Read Temperature High 8-bit
      TPSb = buffer_RTT[31];          //Read Temperature Low 8-bit
      TPS = (TPSa << 8) + TPSb;       //Temperature value
      HDSa = buffer_RTT[32];          //Read Humidity High 8-bit
      HDSb = buffer_RTT[33];          //Read Humidity Low 8-bit
      HDS = (HDSa << 8) + HDSb;       //Humidity value
    }
    else
    {
      PMS = 0;
      FMHDS = 0;
      TPS = 0;
      HDS = 0;
    }
  }

  sprintf(tempStr, "%d%d.%d", TPS / 100, (TPS / 10) % 10, TPS % 10);
  sprintf(hmdStr, "%d%d.%d", HDS / 100, (HDS / 10) % 10, HDS % 10);

  Serial.println(String("Temperature: ") + tempStr + "," + String("Humidity: ") + hmdStr + "," + String(" Formaldehyde%: ") + FMHDS + "," +String(" Particulate Matters%: ") + PMS );
  Serial.println();
  
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {    
    myFile.print(tempStr);
    myFile.print(",");
    myFile.print(hmdStr);
    myFile.print(",");   
    myFile.print(FMHDS);
    myFile.print(",");
    myFile.print(PMS);
    myFile.println();
    myFile.close(); // close the file
  }
  else {
    Serial.println("error opening test.txt");
  }
  delay(3000);

    buttonStateA = digitalRead(buttonPinAir);
    buttonStateB = digitalRead(buttonPinTemp);
    buttonStateC = digitalRead(buttonPinHumid);
    buttonStateD = digitalRead(buttonPinFMHD);

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.print("[ ROOM ATMOS ]");
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.print(" Air Particles:");
      display.setTextSize(1);
      display.setCursor(90, 10);
      display.print(PMS);
      display.print("pm");
      if (condition == 1){
         display.setTextSize(1);
         display.setCursor(0 , 20);
         display.print("STATUS:");
         display.print(" Bad");
      }
      else if (condition == 0){
         display.setTextSize(1);
         display.setCursor(0 , 20);
         display.print("STATUS:");
         display.print(" Good");
      }
      display.display();
      
    
      if (PMS  >= 10) {
        digitalWrite(44, LOW);
        digitalWrite(26, HIGH);
        digitalWrite(24, LOW);
        digitalWrite(22, HIGH);
        digitalWrite(45, HIGH);
        delay(700);
        digitalWrite(45, LOW);
        condition = 1;
      }
    
      else {
        digitalWrite(44, HIGH);
        digitalWrite(45, HIGH);
        delay(50);
        digitalWrite(45, LOW);
        digitalWrite(26, HIGH);
        digitalWrite(24, HIGH);
        digitalWrite(22, LOW);
        condition = 0;
      }
      
  

}



// 12C = 0x3C
