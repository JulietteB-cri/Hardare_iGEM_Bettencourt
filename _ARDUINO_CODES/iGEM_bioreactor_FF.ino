#include <FlowMeter.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "rgb_lcd.h"


////////////////////////////////////////////////////////////Variables////////////////////////////////////////////////////////////
rgb_lcd lcd;

OneWire oneWire(8); //ONE_WIRE_BUS
DallasTemperature temperature_sensor(&oneWire);

FlowMeter *Meter;
int period = 1000; //time to get a flowrate in ms

struct sensors_value { 
  float Temperature;
  float OD;
  float Water_Flow = 0;
} sensors; //Structure for storing all the sensors values. You can access it with sensors.Temperature, sensors.OD...

bool pump_working = false;
int millis_start = 0;


////////////////////////////////////////////////////////////Execution Code////////////////////////////////////////////////////////////
void setup() {
  temperature_sensor.begin();
  Serial.begin(9600);
  pinMode(4,OUTPUT); //Heating system
  pinMode(13,OUTPUT); //LED
  pinMode(8, INPUT); //Temp reception
  pinMode(A2, INPUT); //Valve control
  pinMode(6, INPUT); //Turbidity control
  pinMode(2,OUTPUT); //AirPump
  

  Meter = new FlowMeter(digitalPinToInterrupt(10), UncalibratedSensor, MeterISR, RISING);

  lcd.begin(16,2); 
  lcd.setRGB(255,0,0);


}

void loop() {
  sensors = ReadSensors();
  run_output();
  update_LCD();
}

////////////////////////////////////////////////////////////Other functions////////////////////////////////////////////////////////////
void update_LCD(){
  //Print
  lcd.setCursor(0,0);
  lcd.print(sensors.Temperature);

  lcd.setCursor(8,0);
  lcd.print("TbV:"); //Sensor Output (V)
  lcd.print(sensors.OD);
}

void run_output(){
  //Heater
  if (sensors.Temperature > 28)
  {
    digitalWrite(4,LOW);
    digitalWrite(12,LOW);
  }
  else
  {
    digitalWrite(4,HIGH);
    digitalWrite(13,LOW);
  }

  //AirPump
  if (pump_working && (millis() - millis_start > 60000)) //pump was working for more than 1 min
   {
    digitalWrite(2,LOW); 
    millis_start = millis();
   } else if (!pump_working && (millis() - millis_start > 10000)) {
    digitalWrite(2,HIGH); 
    millis_start = millis();
   }
  
}


struct sensors_value ReadSensors(){
  sensors_value result;
  
  //Temp
  temperature_sensor.requestTemperatures();
  result.Temperature = temperature_sensor.getTempCByIndex(0);

  //OD
  result.OD = analogRead(A2) * (5.0 / 1024.0); //OD

  //WaterFlow
  Meter->tick(period);
  delay(period);
  Meter->tick(period);
  result.Water_Flow = Meter->getCurrentFlowrate();
  
  return result;
}

void MeterISR() {
    // let our flow meter count the pulses
    Meter->count();
}
