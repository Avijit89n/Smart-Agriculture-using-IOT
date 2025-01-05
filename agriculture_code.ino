#define BLYNK_TEMPLATE_ID "TMPL3sVWqTRIW"
#define BLYNK_TEMPLATE_NAME "Agriculture"
#define BLYNK_AUTH_TOKEN "ZZ9ypsfz_NchUcOOZHSyeIOtR6QIkZ5o"

#include <DHT.h>
#include <WiFiManager.h>
#include <LiquidCrystal.h>
#include <BlynkSimpleEsp8266.h>

#define rain D4
#define soil A0
#define motor D7
#define dhtPin D8

DHT dht(dhtPin, DHT11);

const int rs = D6, en = D5, d4 = D0, d5 = D1, d6 = D2, d7 = D3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char auth[] = "ZZ9ypsfz_NchUcOOZHSyeIOtR6QIkZ5o";

void setup() {

  Serial.begin(115200);
  WiFiManager wm;
  
  bool res;
  res = wm.autoConnect("AutoConnectAP","password");

  if(!res) Serial.println("Failed to connect");
  else {
    Serial.println("connected...yeey :)");
  }
  
  lcd.begin(16, 2);
  dht.begin();
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop() {
  bool inputRain = digitalRead(rain);
  Blynk.virtualWrite(V3, !inputRain);

  pinMode(rain, INPUT);
  pinMode(soil, INPUT);
  pinMode(motor, INPUT);
  pinMode(dhtpin, INPUT);

  int input = analogRead(soil);
  int moisture = map(input,300,1024,100,0);
  Serial.println(moisture);
  Blynk.virtualWrite(V2, moisture);
  if(moisture < 50) {
    digitalWrite(motor,LOW);
  }
  else {
    digitalWrite(motor,HIGH);
  }

  int Temperature = dht.readTemperature();
  int Humidity = dht.readHumidity();
  Serial.print("Current Humidity: ");
  Serial.println(Humidity);
  Serial.print("Current Temperature: ");
  Serial.println(Temperature);
  Blynk.virtualWrite(V0, Temperature);
  Blynk.virtualWrite(V1, Humidity);


  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(2, 0); 
  lcd.print(Temperature);
  if(Temperature < 100 ) {
    lcd.setCursor(4, 0);
    lcd.print((char)223);
    lcd.setCursor(5, 0);
    lcd.print("C ");
  }
  else{
    lcd.setCursor(5, 0);
    lcd.print((char)223);
    lcd.setCursor(6, 0);
    lcd.print("C");
  }

  lcd.setCursor(7, 0);
  lcd.print("   H:");
  lcd.setCursor(12, 0);
  lcd.print(Humidity);
  if(Humidity < 100 ) {
    lcd.setCursor(14, 0);
    lcd.print("% ");
  }
  else{
    lcd.setCursor(15, 0);
    lcd.print("%");
  }

  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.setCursor(2, 1);
  lcd.print(moisture);
  if(moisture < 100 ) {
    lcd.setCursor(4, 1);
    lcd.print("% ");
  }
  else{
    lcd.setCursor(5, 1);
    lcd.print("%");
  }

  lcd.setCursor(6, 1);
  if(inputRain == false){
    lcd.print("   (RAIN) ");
  } else {
    lcd.print("   (CLEAR)");
  }


  Blynk.run();
  delay(1000);
}
