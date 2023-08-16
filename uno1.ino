#include <PZEM004Tv30.h>
#include <DHT.h>

#define DHTPIN 9
#define DHTTYPE DHT22

PZEM004Tv30 pzem(2, 3); //(TX, RX);
DHT dht(DHTPIN, DHTTYPE);

void setup (){
  Serial.begin(9600);
  dht.begin();
}

void loop(){
  String request = "";
  while(Serial.available()>0){
    request += char(Serial.read());
  }
  request.trim();

  if(request == "yes"){
    sendData();
  }
  request = "";
  delay(500);
}

void sendData(){
  float voltage0   = pzem.voltage();
  float current0   = pzem.current();
  float power0     = pzem.power();
  float energy0    = pzem.energy();
  float frequency0 = pzem.frequency();
  float temp0      = dht.readTemperature();
  String dataSend  = String(voltage0)+"#"+String(current0)+"#"+String(power0)+"#"+String(energy0)+"#"+String(frequency0)+"#"+String(temp0);
  Serial.println(dataSend);
}
