#define BLYNK_TEMPLATE_ID "TMPL6IOqF_tDR"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "ZV91ESQgZblFTHYOb12t787cP2J24gUm"
#include <BlynkSimpleEsp8266.h>

#include<SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DISPLAY_ADDRESS   0x3C
#define DISPLAY_SCL_PIN   5
#define DISPLAY_SDA_PIN   4
#define DISPLAY_WIDTH     128
#define DISPLAY_HEIGHT    64
#define DISPLAY_RESET_PIN -1
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, DISPLAY_RESET_PIN);

SoftwareSerial dataSerial(12, 13); //(TX, RX);

char ssid[] = "mokapos";
char pass[] = "mokapos123450";
char auth[] = BLYNK_AUTH_TOKEN;
unsigned long previousMillis = 0;
const long interval = 1000;

float voltage1, current1, power1, energy1, frequency1, temp1;
String arrData[7];

void setup() {
  Serial.begin(9600);
  dataSerial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  setDisplay();
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

   String data = "";
  while(dataSerial.available()>0){
    data += char(dataSerial.read());
  }

  data.trim();

  if(data != ""){
    int index = 0;
    for(int i=0; i<= data.length(); i++){
      char delimiter = '#';
      if(data[i] != delimiter){
        arrData[index] += data[i];
      }
      else{
        index++;
      }
    }

    if(index == 6){
      Serial.println("AC Voltage     : " + arrData[0] + "V");
      Serial.println("AC Current     : " + arrData[1] + "A");
      Serial.println("AC Power       : " + arrData[2] + "Watt");
      Serial.println("Energy Total   : " + arrData[3] + "kWh");
      Serial.println("Frequency      : " + arrData[4] + "Hz");
      Serial.println("Temperature    : " + arrData[5] + "C");
      Serial.println();
    }

    voltage1   = arrData[0].toFloat();
    current1   = arrData[1].toFloat();
    power1     = arrData[2].toFloat();
    energy1    = arrData[3].toFloat();
    frequency1 = arrData[4].toFloat();
    temp1      = arrData[5].toFloat();

    arrData[0] = "";
    arrData[1] = "";
    arrData[2] = "";
    arrData[3] = "";
    arrData[4] = "";
    arrData[5] = "";

    Blynk.run();
    Blynk.virtualWrite(V0, voltage1);
    Blynk.virtualWrite(V1, current1);
    Blynk.virtualWrite(V2, power1);
    Blynk.virtualWrite(V3, energy1);
    Blynk.virtualWrite(V4, frequency1);
    Blynk.virtualWrite(V5, temp1);
  
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Solar Cell Test");
    display.println("CEK STATUS...");
    display.printf("Voltage   : %.2f\ V\n", voltage1);
    display.printf("Current   : %.2f\ A\n", current1);
    display.printf("Power     : %.2f\ W\n", power1);
    display.printf("Energy    : %.2f\ kWh\n", energy1);
    display.printf("Frequency : %.2f\ Hz\n", frequency1);
    display.printf("Temp      : %.2f\ C\n", temp1);
    display.display();
    delay(2000);
    }
  dataSerial.println("yes"); 
  }
}

void setDisplay(){
  Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);

  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("PZEM-004T");
  display.setCursor(10, 36);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Connecting Blynk..");
  display.display();
  delay(1000);
}
