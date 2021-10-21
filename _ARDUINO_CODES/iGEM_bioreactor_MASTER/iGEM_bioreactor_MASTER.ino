#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius=0;
float Fahrenheit=0;
//https://lastminuteengineers.com/ds18b20-arduino-tutorial/

int air_pump1 = 3;
int air_pump2 =2;
int liquid_handling_pump = 5;

int temperature_threshold_1 = 26;
int heater_pin1 = 8;
int heater_pin2 = 9;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); //Baud rate: 9600

  Serial.println("_____________________");
  Serial.println("_____Hello Wolrd_____");
  Serial.println("_____________________");

  sensors.begin();
  
  pinMode(air_pump1, OUTPUT);
  pinMode(air_pump2, OUTPUT);
  pinMode(liquid_handling_pump, OUTPUT);

  pinMode(heater_pin1, OUTPUT);
  pinMode(heater_pin2, OUTPUT);
}


int read_temperature(){
  Serial.println("_____________________");
  Serial.println("_____Temperature_____");
  Serial.println("_____________________");
  
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);

  Serial.println("_____________________");
  Serial.println("_____Temperature_____");
  Serial.println("_____________________");

  return Celcius;
} 

float readOD(){
  Serial.println("_____________________");
  Serial.println("_____OD_____");
  Serial.println("_____________________");

  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage); // print out the value you read:
  delay(500);
  Serial.println("_____________________");
  Serial.println("_____OD_____");
  Serial.println("_____________________");
  return voltage;
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(air_pump1, HIGH);
  digitalWrite(air_pump2, HIGH);
  digitalWrite(liquid_handling_pump, HIGH);
/*
  digitalWrite(air_pump1, HIGH);
  digitalWrite(air_pump2, HIGH);
  digitalWrite(liquid_handling_pump, HIGH);

  delay(1000);
  
  digitalWrite(air_pump1, LOW);
  digitalWrite(air_pump2, LOW);
  digitalWrite(liquid_handling_pump, LOW);
  delay(1000);
*/
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);
  int temp1 = read_temperature();

  if  (temp1 > temperature_threshold_1){
    Serial.println("should turn off heater1");
    digitalWrite(heater_pin1,HIGH);
  }else{
    Serial.println("should turn ON heater1");
    digitalWrite(heater_pin1,LOW);
  }

}
  
  
