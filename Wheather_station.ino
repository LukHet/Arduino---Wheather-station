 #include  <ESP8266WiFi.h> 
#include <ArduinoHttpClient.h> 
#include <LiquidCrystal.h>
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>

LiquidCrystal lcd(D6, D5, D1, D2, D3, D4); 
 
const char* ssid     ="tp-link"; 
const char* password = "hetman9399"; 

int buttonPushCounter = 0;   
int buttonState = 0;         
int lastButtonState = 0;

WiFiUDP ntpUDP;


NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

char Date[ ] = "00.00.2000";
byte day_, month_;
int year_;

int ledPin = 2; 
int failed=0;
WiFiServer server(80);
char serverAddress[] = "192.168.1.107";  // server address
int port = 80;


WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
 
void setup() {
lcd.begin(16, 2);
Serial.begin(115200);
pinMode(D7, INPUT);

pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);

 

  lcd.setCursor(0,0);
  lcd.print("Connecting");
  lcd.setCursor(0,1);
  lcd.print(ssid);
int timer=0;
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
lcd.print(".");
timer=timer+1;
if(timer>15){
  lcd.clear();
  lcd.print("Failed");
  failed=1;
  return;}
}
 
lcd.clear();
lcd.print("Wifi connected");
client.get("/php1/SM.php");
int statusCode = client.responseStatusCode();
String response = client.responseBody();
 
 if(WiFi.status() == WL_CONNECTED){
  Serial.begin(115200);
  lcd.begin(16, 2);                 // Initialize 16x2 LCD Display
  lcd.clear();
  }
}

void loop() {
  unsigned long unix_epoch = timeClient.getEpochTime();
  if(failed!=1){
  if(WiFi.status() != WL_CONNECTED){
    delay(50);
    if(WiFi.status() != WL_CONNECTED){
      lcd.clear();
      lcd.print("Disconnected");
      return;
      }
    }
  }

 if(WiFi.status() == WL_CONNECTED){
     buttonState = digitalRead(D7);

    if (buttonState != lastButtonState) {
    
     if (buttonState == HIGH) {
      
        buttonPushCounter++;

        } 
        delay(50);
      }
    lastButtonState = buttonState;

    if(buttonPushCounter == 2){
      client.get("/php1/SM.php");
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();
      lcd.clear();
      lcd.print("Temperatura: ");
      lcd.setCursor(0,1);
      if (response[101]==45){
       lcd.print(response[101]); }
      if (isDigit(response[102])){
        lcd.print(response[102]);}
      if (isDigit(response[103])){
        lcd.print(response[103]);}
      lcd.print((char)223);
      lcd.print("C");
    }
      else if(buttonPushCounter == 3){
          client.get("/php1/SM.php");
          int statusCode = client.responseStatusCode();
          String response = client.responseBody();
          lcd.clear();
          lcd.print("Wiatr: ");
          lcd.print(response[539]);
          lcd.print(response[540]);
          lcd.print(" km/h");
        }
      else if(buttonPushCounter == 4){
          client.get("/php1/SM.php");
          int statusCode = client.responseStatusCode();
          String response = client.responseBody();
          lcd.clear();
          lcd.print("Cisnienie: ");
          lcd.setCursor(0,1);
          lcd.print(response[943]);
          lcd.print(response[944]);
          lcd.print(response[945]);
          lcd.print(response[946]);
          lcd.print(" hPa");
          }

        else if(buttonPushCounter == 5){
          client.get("/php1/SM.php");
          int statusCode = client.responseStatusCode();
          String response = client.responseBody();
          lcd.clear();
          lcd.print("Zachmurzenie: ");
          lcd.setCursor(0,1);
          lcd.print(response[203]);
          lcd.print(response[204]);
          lcd.print(response[205]);
          lcd.print(response[206]);
          lcd.print(response[207]);
          lcd.print(response[208]);
          lcd.print(response[209]);
          lcd.print(response[210]);
          lcd.print(response[211]);
          }
          
    else if (buttonPushCounter == 1 || buttonPushCounter == 6){
          lcd.clear();
          day_    = day(unix_epoch);
          month_  = month(unix_epoch);
          year_   = year(unix_epoch);
          Date[0]  = day_   / 10 + 48;
          Date[1]  = day_   % 10 + 48;
          Date[3]  = month_  / 10 + 48;
          Date[4]  = month_  % 10 + 48;
          Date[8] = (year_   / 10) % 10 + 48;
          Date[9] = year_   % 10 % 10 + 48;
          timeClient.update();
          lcd.setCursor(0, 0);
          lcd.print(timeClient.getFormattedTime());
          lcd.setCursor(0, 1);
          lcd.print(Date);
          lcd.print("r.");
          buttonPushCounter = 1;
          }
          delay(1000);
          }
  }
 
