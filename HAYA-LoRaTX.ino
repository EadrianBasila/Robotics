#include <SPI.h>
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <string.h>
#include <avr/dtostrf.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RH_RF95.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define RF95_FREQ 915.0
#define GPSSerial Serial1
#define GPSECHO false
int flag =0;
int flag2 = 0;
int flag3 = 0;
int flag4= 0;
int hr = 0;
int mine = 0;
String sec;
uint32_t timer = millis();
uint32_t lastsend = millis();
Adafruit_GPS GPS(&GPSSerial);
RH_RF95 rf95(RFM95_CS, RFM95_INT);
double batt;
int percent;
char radiopacket[50];

   int y=0;
   int y2=0;
   int x2=1;
   int btA;
   int btB;
   int btC; 
   bool btAl;
   bool btBl;
   bool btCl; 
   int x=0;
   int crntDisp =0;
   String message = "";
   float Long = 0;
   float Lat = 0;
   int sats = 0;
   int fix = 0;
   float alt = 0;
   float spd = 0;
void setup() {
  Serial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(6,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  Serial.println("Feather LoRa TX Test!");
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
    while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0,0);       
  display.println("RADIO OK");
  display.display();
  delay (1000);
   if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  display.setCursor(0,16);       
  display.println("Freq915mHz");
  display.display();
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

int16_t packetnum = 0;  // packet counter, we increment per xmission
delay (1000);
 GPSSerial.println(PMTK_Q_RELEASE);
}
void loop(){

  if (millis() - lastsend > 5000) {
  lastsend = millis(); 
  consend();
  }
  timee();
  batt = voltage();
  Gps();
    if (flag4 >=1){
    yisnu();
  }else{
  switch (crntDisp) {
  case 0:
    dispScreen();
    break;
  case 1:
    mainMenu();
    break;
  case 2:
    Loc();
    break;
  case 3:
  Send();
  break;
    case 4:
  Pin();
  break;
  default:
  display.clearDisplay();
    if ( y!=0 || x2 ==0){
    y=0;
    crntDisp=0;
  }
  break;
}
  }
}
void set (int x){
    if (x == 0){
    display.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
  }else{
    display.setTextColor(SSD1306_WHITE); 
  }
}
void dispScreen(){
  if ( y <-1){
    y=1;
  }else if (y > 1){
   y=-1;
  }
  readPins();
  int curr = y;
  if (y == -1){
  display.clearDisplay();
  display.setTextSize(4);
  set(1);     
  display.setCursor(0,0);       
  display.println("HAYA");
  display.setTextSize(1);
  display.setCursor(100,0); 
  if (fix == 1){      
  display.println("OK");
  }else {
   display.println("NO"); 
  }
  display.setTextSize(2);
  display.setCursor(100,16);
  display.println(sats); 
  display.display();
  } else if(y==0) {
  display.clearDisplay();
  display.setTextSize(4);
  set(1); 
  if (hr >9){    
  display.setCursor(0,0); 
  }else {
      display.setCursor(20,0);     
  }
  display.println(hr); 
  flag += 1;
  if (flag >= 20){
     display.setCursor(40,32);  
     flag2 += 1;
  }
if (flag2 >= 20){
  display.setCursor(40,0);


  }if (flag2 >= 40){
      flag2 = 0; 
      flag2 = 0; 
  }
  display.println(":"); 
  if (mine <10){
   display.setCursor(60,0);  
   display.println(0);
   display.setCursor(80,0); 
  }else {
    display.setCursor(60,0); 
  }
  display.println(mine);
  display.setCursor(110,0);
  display.setTextSize(1);
  display.println(sec);
  display.display();
  }
  else if(y==1) {
  display.clearDisplay();
  display.setTextSize(2);
  set(1);     
  display.setCursor(80,0);     
  display.println(batt); 
  display.setCursor(80,16);  
  if (percent >100) {
    display.println("USB"); 
  }else if(percent > 20){   
    display.println(" OK"); 
  } else if (percent <= 20){
   display.println("CHARGE");  
  }
  display.setTextSize(4);
  display.setCursor(0,0);     
  display.println(percent);
  display.setCursor(50,0);
  display.println("%");  
  display.display();
  delay(1500);
  }
  if ( x2 ==0){
    y=0;
    crntDisp=1;
  }
}
void mainMenu(){
  if (y>3){
    y=0;
  }else if (y<0){
    y=3;
  }
  readPins();
  display.clearDisplay();
  display.setTextSize(2);
  set(y);     
  display.setCursor(0,0-y2);       
  display.println("LOKASYON:");
  set(y-1);
  display.setCursor(0,16-y2);       
  display.println("MAGPADALA:");
  set(y-2);
  display.setCursor(0,32-y2);       
  display.println("PIN. LOC.");
  set(y-3);
  display.setCursor(0,48-y2);       
  display.println("BUMALIK:");
  display.display();
  delay(10);
  if (x2 == 0){
    switch (y) {
  case 0:
  y=0;
  crntDisp=2;
    break;
  case 1:
    y=0;
  crntDisp=3;
    break;
  case 2:
  y=0;
  crntDisp=4;
    break;
  case 3:
  y=0;
  crntDisp=0;
  break;
}
  }
}
void Loc(){
  //gps code here
  
    if (y>3){
    y=0;
  }else if (y<0){
    y=3;
  }
  readPins();
  display.clearDisplay();
  display.setTextSize(2);
  set(1);     
  display.setCursor(0,0-y2);       
  display.print(Lat);
  display.println(GPS.lat);
  set(1);
  display.setCursor(0,16-y2);       
  display.print(Long);
  display.println(GPS.lon);
  set(1);
  display.setCursor(0,32-y2);       
  display.println("sats:");
  display.setCursor(60,32-y2);       
  display.println(sats);
  set(y-3);
  display.setCursor(0,48-y2);       
  display.println("BUMALIK:");
  display.display();
  if (x2 == 0 && y == 3){
    y=0;
    crntDisp = 1;
  }
}
void Send(){
    if (y>24){
    y=0;
  }else if (y<0){
    y=24;
  }
  readPins();
  display.clearDisplay();
  display.setTextSize(2);
  set(1); 
  display.setCursor(0,0-y2);       
  display.println("PUMILI NG");
  display.setCursor(30,16-y2);       
  display.println("MENSAHE:");
  set(y-2);
  display.setCursor(0,32-y2);       
  display.println("A)");
  set(1);
  display.setCursor(30,32-y2);       
  display.println("Banka ay");
  display.setCursor(0,48-y2);       
  display.println("lulubog na.");
  set(y-4);
  display.setCursor(0,64-y2);       
  display.println("B)");
  set(1);
  display.setCursor(30,64-y2);       
  display.println("Maraming");
  display.setCursor(0,80-y2);       
  display.println("isda rito.");
  set(y-6);
  display.setCursor(0,96-y2);       
  display.println("C)");
  set(1);
  display.setCursor(30,96-y2);       
  display.println("Ako ay");
  display.setCursor(0,112-y2);       
  display.println(" naliligaw."); 
  set(y-8);
  display.setCursor(0,128-y2);       
  display.println("D)");
  set(1);
  display.setCursor(30,128-y2);       
  display.println("Mayroong");
  display.setCursor(0,144-y2);       
  display.println("Oil Spill."); 
  set(y-10);
  display.setCursor(0,160-y2);       
  display.println("E)");
  set(1);
  display.setCursor(30,160-y2);       
  display.println("Foreign");
  display.setCursor(0,176-y2);       
  display.println(" vessel."); 
  set(y-12);
  display.setCursor(0,192-y2);       
  display.println("F)");
  set(1);
  display.setCursor(30,192-y2);       
  display.println("Malakas");
  display.setCursor(0,208-y2);       
  display.println(" na ulan.");
  set(y-14);
  display.setCursor(0,224-y2);       
  display.println("G)");
  set(1);
  display.setCursor(30,224-y2);       
  display.println("Mayroong");
  display.setCursor(0,240-y2);       
  display.println("pirata.");
  set(y-16);
  display.setCursor(0,256-y2);       
  display.println("H)");
  set(1);
  display.setCursor(30,256-y2);       
  display.println("Over-");
  display.setCursor(0,272-y2);       
  display.println("fishing."); 
  set(y-18);
  display.setCursor(0,288-y2);       
  display.println("I)");
  set(1);
  display.setCursor(30,288-y2);       
  display.println("Malakas");
  display.setCursor(0,304-y2);       
  display.println("alon."); 
    set(y-20);
  display.setCursor(0,320-y2);       
  display.println("J)");
  set(1);
  display.setCursor(30,320-y2);       
  display.println("Dynamite");
  display.setCursor(0,336-y2);       
  display.println("Fishing"); 
  set(y-22);
  display.setCursor(0,352-y2);       
  display.println("K)");
  set(1);
  display.setCursor(30,352-y2);       
  display.println("OKAY");
  display.setCursor(0,368-y2);       
  display.println("na ako."); 
  set(y-24);
  display.setCursor(0,384-y2);       
  display.println("BUMALIK");
  display.display();
  
 if (x2 ==0){  
   switch (y) {
  case 2:
    y=0;
    strcpy(radiopacket,strdup("D|BLN[HAYA-1Xt]."));
    flag4 =1;
    break;
  case 4:
    y=0; 
    strcpy(radiopacket,strdup("T|MRN[HAYA-1Xt]"));
    flag4 =1;
    break;
    case 6:
    y=0;
    strcpy(radiopacket,strdup("D|NWA[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 8:
    y=0; 
    strcpy(radiopacket,strdup("R|OIL[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 24:
    y=0;
    crntDisp = 1;  
  break;
  case 22:
    y=0;
    strcpy(radiopacket,strdup("T|OKY[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 20:
    y=0; 
    strcpy(radiopacket,strdup("R|DNF[HAYA-1Xt]"));
    flag4 =1;
    break;
    case 18:
    y=0;
    strcpy(radiopacket,strdup("W|MAW[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 16:
    y=0; 
    strcpy(radiopacket,strdup("R|OVF[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 14:
    y=0; 
    strcpy(radiopacket,strdup("R|MPT[HAYA-1Xt]"));
    flag4 =1;  
     case 12:
    y=0; 
    strcpy(radiopacket,strdup("W|MAU[HAYA-1Xt]"));
    flag4 =1;
    break;
  case 10:
    y=0; 
    strcpy(radiopacket,strdup("R|FFV[HAYA-1Xt]"));
    flag4 =1;
  break;
}
}
}
void readPins(){
  btB = digitalRead(6);
  btC = digitalRead(5);
  Serial.print (btA);
  btA = digitalRead(10);
   if(btB == 0){
    btBl = true;
   }
   if(btB == 1 && btBl){
    btBl = false;
    x2 = 0;
   }else {
    x2 =1;
   }
   if(btC == 0){
    btCl = true;
   }
   if(btC == 1 && btCl){
    btCl = false;
    x = 1;
   }
   if(btA == 0){
    btAl = true;
   }
   if(btA == 1 && btAl){
    btAl = false;
    x = -1;
   }
  y = y + x;
  x = 0;
  if(y > 25){
    y2=416;
    }else if(y > 23){
    y2=384;
    }else if(y > 21){
    y2=352;
  }else if(y > 19){
    y2=320;
    }else if(y > 17){
    y2=288;
    }else if(y > 15){
    y2=256;
  }else if(y > 13){
    y2=224;
  }else if(y > 11){
    y2=192;
  }else if(y > 9){
    y2=160;
    }else if(y > 7){
    y2=128;
    }else if(y > 5){
    y2=96;
  }else if(y > 3){
    y2=64;
  }else if(y > 1){
    y2=32;
  }else{
    y2=0;
  }
}
void Gps(){
    char c = GPS.read();
  if (GPSECHO)
    if (c) Serial.print(c);
  
  if (GPS.newNMEAreceived()) {
   
    Serial.println(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA()))
      return; 
  }Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      fix = 1;
      Long = (GPS.longitude);
      Lat = (GPS.latitude);
      sats = ((int)GPS.satellites);
      alt = GPS.altitude;
      spd = GPS.speed;
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }else{
      sats =0;
    }
    fix = ((int)GPS.fix);
    
    }

int16_t packetnum = 0;  
void yisnu(){
    if (y >1){
    y=0;
  }else if ( y <0){
    y=1;
  }
  readPins();
  display.clearDisplay();
  display.setTextSize(2);
  set(1); 
  display.setCursor(40,0);       
  display.println("IPADALA?");
  set(y);
  display.setCursor(30,16);      
  display.print("OO");
  set (1);
  display.print("/");
  set (y-1);
  display.println("HINDI");
  display.display();
  if (x2 == 0){
    if (y == 0){
   message = "1";
  Lora();
      flag4 =0;
  }else if(y == 1) {
    y = 0;
    crntDisp=3;
     flag4 =0;
  }
  }
}
void Lora(){
  // Send a message to rf95_server
  itoa(packetnum++, radiopacket+30, 30);
  Serial.print("IPINAPADALA..");
  Serial.println(radiopacket);
  radiopacket[19] = 0;
  Serial.println("IPINAPADALA..");
  delay (10);
  display.clearDisplay();
  display.setTextSize(2);
 for (int i=0 ;i<=5;i++){
  if(i%2 == 0){
    set(1); 
  }else {
    set(0);
  }
  display.setCursor(0,0);       
  display.println("IPINAPADALA.");
  display.display();
  delay (200);
  }
  rf95.send((uint8_t *)radiopacket, 30);
  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  display.clearDisplay();
  set(1);
  display.setCursor(0,0);       
  display.println("NAIPADALA.");
  display.display();
  delay (2000);
}
void Pin(){
  if (y >1){
    y=0;
  }else if ( y <0){
    y=1;
  }
  readPins();
  display.clearDisplay();
  display.setTextSize(2);
  set(1); 
  display.setCursor(40,0);       
  display.println("IPADALA?");
  set(y);
  display.setCursor(30,16);      
  display.print("OO");
  set (1);
  display.print("/");
  set (y-1);
  display.println("HINDI");
  display.display();
  if (x2 == 0){
    if (y == 0){
  dtostrf (Lat, 10,4,radiopacket);
  Lora();
  dtostrf (Long, 10,4,radiopacket);
  Lora();
  }else if(y == 1) {
    y = 0;
    crntDisp=1;
  }
  }
}
double voltage(){
  double g= analogRead(9);
  double x = (g/780) * 5.00;
  int gg = x *100;
  percent = map ( gg,300,420,0,100);
  return x;
}
void timee(){
   hr = GPS.hour + 8;
   mine = GPS.minute; 
   if ( hr > 24){
     hr-=24;
     sec = "AM";
     }else if ( hr > 12){
    hr -=12;
    sec = "PM";
   }else{
    sec = "AM";
}
}
void consend(){
  if (message = ""){
  itoa(packetnum++, radiopacket+20, 20);
  Serial.println(radiopacket);
  radiopacket[19] = 0;
  delay (10);
  rf95.send((uint8_t *)radiopacket, 30);
  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  display.display();
  return;
}
}
void conreci(){
    if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {

      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    }
    flag3 += 1;
    if ( flag3 >= 4){
  display.clearDisplay();
  display.setTextSize(3);
  set(1);     
  display.setCursor(0,0);       
  display.println("MESSAGE RECIEVED");
  display.setTextSize(1);
  display.display();
  delay (5000);
    }
  }
}
