#include <Arduino.h>
#include <math.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
//DISPLAY STUFFS

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <ezOutput.h> // ezOutput library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ONE_WIRE_BUS 6

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius = 0;
float Fahrenheit = 0;
//https://lastminuteengineers.com/ds18b20-arduino-tutorial/
//Multiple_temperature_sensors

int air_pump1 = 6;
int air_pump2 = 2;
int liquid_handling_pump = 2;

int temperature_threshold_1 = 40;
int temperature_threshold_2 = 35;
int heater_pin1 = 8;
int heater_pin2 = 9;

ezOutput _air_pump1(air_pump1);   // create ezOutput object that attach to pin 7;
ezOutput _air_pump2(air_pump2);   // create ezOutput object that attach to pin 8;
int _bubling_lenght = .5 * 60;    //   in seconds
int _bubling_frequency1 = 5 * 60; // in seconds
int _bubling_frequency2 = 5 * 60; // in seconds

int _state_heater_1 = 0;
int temp1 = 0;

float current_od = 0;

int deviceCount = 0;
float tempC1;
float tempC2 = 0.0;

const int buttonPin = 10; // the number of the pushbutton pin
int buttonState = 0;      // variable for reading the pushbutton status
const int ledPin = 13;    // the number of the LED pin

int incomingByte = 0; // for incoming serial data

bool enable_screen = false;

bool Heaters_overritte = false;

void display_variable_on_screen()
{
  display.clearDisplay();
  display.setTextSize(1); // Normal 1:1 pixel scale
  //display.setTextColor(WHITE); // Draw white text
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 0); // Start at top-left corner
  display.cp437(true);     // Use full 256 char 'Code Page 437' font
  display.print("BIO_REACTOR 1");

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  display.setCursor(10, 10); // Start at top-left corner
  //String temperature1 = "Temp1 = " + String(temp1) + " *C";
  String temperature1 = "Temp1 = ";
  display.print(temperature1);

  display.setCursor(60, 10); // Start at top-left corner
  display.print(tempC1);

  display.setCursor(0, 40); // Start at top-left corner
  String _s_OD = "OD = ";
  display.print(current_od);

  display.display();
}

void int_test_screen()
{

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  for (int16_t i = 0; i < 256; i++)
  {
    if (i == '\n')
      display.write(' ');
    else
      display.write(i);
  }

  display.display();
}

int read_temperature(bool debug = false, bool verbose = false)
{
  if (verbose)
  {
    Serial.println("_____________________");
    Serial.println("_____Temperature_____");
    Serial.println("_____________________");
  }

  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celcius);
  if (debug)
  {
    Serial.print(" C ");
    Serial.print(Celcius);
    // Serial.print(" F  ");
    // Serial.println(Fahrenheit);
    // delay(1000);
  }

  if (verbose)
  {
    Serial.print(" C ");
    Serial.print(Celcius);

    Serial.println("_____________________");
    Serial.println("_____Temperature_____");
    Serial.println("_____________________");
  }

  return Celcius;
}

void read_multiple_temperature_sensors()
{
  // bool data1_is_valid = false;
  // bool data2_is_valid = false;
  // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures();

  // Display temperature from each sensor
  //https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/
  for (int i = 0; i < deviceCount; i++)
  {
    if (i == 0)
    {
      
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" : ");
      tempC2 = sensors.getTempCByIndex(i);
      Serial.print(tempC2);
      Serial.print("C  |  ");
     
    }
    else if (i == 1)
    {
      
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" : ");
      //tempC2 = sensors.getTempCByIndex(i);
      tempC1 = sensors.getTempCByIndex(i);
      Serial.print(tempC1);
      //Serial.print(tempC2);
      //Serial.print((char)176);//shows degrees character
      Serial.println("C  |  ");
      
    }
  }
}

float readOD(bool verbose = false)
{
  if (verbose)
  {
    Serial.println("_____________________");
    Serial.println("_____OD_____");
    Serial.println("_____________________");
  }
  int sensorValue = analogRead(A0);             // read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage);                      // print out the value you read:
  //delay(500);
  if (verbose)
  {
    Serial.println("_____________________");
    Serial.println("_____OD_____");
    Serial.println("_____________________");
  }
  return voltage;
}

void temperateure_sensor_reading1(bool verbose = false)
{
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  //delay(1000);
  temp1 = read_temperature();

  if (temp1 > temperature_threshold_1)
  {
    if (verbose)
    {
      Serial.println("should turn off heater1");
      digitalWrite(heater_pin1, HIGH);
    }

    _state_heater_1 = 1;
  }
  else
  {
    if (verbose)
    {
      Serial.println("should turn ON heater1");
      digitalWrite(heater_pin1, LOW);
    }

    _state_heater_1 = 0;
  }

  digitalWrite(heater_pin1, _state_heater_1);
}

void hande_bubling()
{
  _air_pump1.loop(); // MUST call the led1.loop() function in loop()
  _air_pump2.loop(); // MUST call the led2.loop() function in loop()
}

void handle_liquid_handlingg()
{
  //Need to define when we move culture fron Bio_reactor1 to Bio_reactor2

  /*
  digitalWrite(liquid_handling_pump, HIGH);
  digitalWrite(liquid_handling_pump, LOW);
  */
}

void handle_serial_receive()
{
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.read();

    // prints the received data
    Serial.print("I received: ");
    char message = (char)incomingByte;
    //Serial.println(message);
    if (message == 'p')
    {
      Serial.println("SHOULD ACTIVATE PUMP");
      digitalWrite(ledPin, HIGH);
      Serial.println("button up");
      digitalWrite(liquid_handling_pump, LOW);
    }

    //HEATERS STUFFFS
    if (message == 'h')
    {
      Serial.println("Should_over_wire_heaters");
      Heaters_overritte = true;
    }
    if (message == 'u')
    {
      Serial.println("Heaters_back_to_normal");
      Heaters_overritte = false;
    }

    if (message == '0')
    {
      Serial.println("should_turn_On_heater1");
      digitalWrite(heater_pin1, LOW);
      
    }

    if (message == '1')
    {
      Serial.println("should_turn_off_heater1");
      digitalWrite(heater_pin1, HIGH);
      
    }

    if (message == '2')
    {
      Serial.println("should_turn_On_heater2");
      digitalWrite(heater_pin2, LOW);
      
    }

    if (message == '3')
    {
      Serial.println("should_turn_off_heater2");
      digitalWrite(heater_pin2, HIGH);
      
    }
  //HEATERS STUFFFS

    else if (message == 'v')
    {
      Serial.println("SHOULD DEACTIVATE PUMP");
      digitalWrite(ledPin, LOW);
      Serial.println("button up");
      digitalWrite(liquid_handling_pump, HIGH);
    }
  }
}

void handle_heaters(bool debug = false)
{

  if (tempC1 < temperature_threshold_1)
  {
    digitalWrite(heater_pin1, LOW);
    if (debug)
    {
      Serial.println("HEATER1 IS ON");
    }
  }
  else
  {
    digitalWrite(heater_pin1, HIGH);
    if (debug)
    {
      Serial.println("HEATER1 IS OFF");
    }
  }

  //////////////////////////////

  if (tempC2 < temperature_threshold_2)
  {
    digitalWrite(heater_pin2, LOW);
    if (debug)
    {
      Serial.println("HEATER2 IS ON");
    }
  }
  else
  {
    digitalWrite(heater_pin2, HIGH);
    if (debug)
    {
      Serial.println("HEATER2 IS OFF");
    }
  }
}

void handle_button()
{
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH)
  {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    Serial.println("button up");
    digitalWrite(liquid_handling_pump, LOW);
  }
  else
  {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    digitalWrite(liquid_handling_pump, HIGH);
  }
}

void log_data()
{
  String Data_to_log = "d,";
  int air_pump1_state = digitalRead(air_pump1);
  int air_pump2_state = digitalRead(air_pump2);
  int air_pump_Liquid_handling_state = digitalRead(liquid_handling_pump);

  Data_to_log = "d," + String(tempC1) + "," + String(tempC2) + "," + String(temperature_threshold_1) + "," + String(temperature_threshold_2) + "," + String(current_od) + "," + String(air_pump1_state) + "," + String(air_pump2_state) + "," + String(air_pump_Liquid_handling_state);

  Serial.println(Data_to_log);
}

void log_data2()
{
  //temperature1,temperature2,temp_treshold1,temp_treshold2,OD,Air_pump1_state,Air_pump2_state,Liquid_handling_pump_state
  int air_pump1_state = 1 - digitalRead(air_pump1);
  int air_pump2_state = 1 - digitalRead(air_pump2);
  int air_pump_Liquid_handling_state = 1 - digitalRead(liquid_handling_pump);

  int heater1_state = 1 - digitalRead(heater_pin1);
  int heater2_state = 1 - digitalRead(heater_pin2);

  Serial.println("");
  Serial.print("d,");
  Serial.print(tempC1);
  Serial.print(",");
  Serial.print(tempC2);
  Serial.print(",");
  Serial.print(temperature_threshold_1);
  Serial.print(",");
  Serial.print(temperature_threshold_2);
  Serial.print(",");
  Serial.print(current_od);
  Serial.print(",");
  Serial.print(air_pump1_state);
  Serial.print(",");
  Serial.print(air_pump2_state);
  Serial.print(",");
  Serial.print(air_pump_Liquid_handling_state);

  Serial.print(",");
  Serial.print(heater1_state);
  Serial.print(",");
  Serial.print(heater2_state);

  Serial.println("");
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(liquid_handling_pump, OUTPUT);
  digitalWrite(liquid_handling_pump, HIGH);

  Serial.begin(9600); //Baud rate: 9600

  if (enable_screen)
  {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
    }
    int_test_screen();
  }

  //Setup blinkers without delay on the frequency settings
  _bubling_lenght = _bubling_lenght * 1000;
  _bubling_frequency1 = _bubling_frequency1 * 1000;
  _bubling_frequency2 = _bubling_frequency2 * 1000;
  _air_pump1.blink(_bubling_lenght, _bubling_frequency1); // 500 milliseconds ON, 250 milliseconds OFF
  _air_pump2.blink(_bubling_lenght, _bubling_frequency2); // 250 milliseconds ON, 250 milliseconds OFF

  Serial.println("_____________________");
  Serial.println("_____Hello Wolrd_____");
  Serial.println("_____________________");

  sensors.begin();

  pinMode(air_pump1, OUTPUT);
  pinMode(air_pump2, OUTPUT);

  pinMode(liquid_handling_pump, OUTPUT);
  digitalWrite(liquid_handling_pump, HIGH);
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(heater_pin1, OUTPUT);
  pinMode(heater_pin2, OUTPUT);

  if (enable_screen)
  {
    display_variable_on_screen();
  }

  //MULTIPLE_TEMPERATURE_SENSORS
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
}

void loop()
{

  current_od = readOD();
  //temperateure_sensor_reading1();

  bool data_is_valid = false;
  while (data_is_valid == false)
  {
    read_multiple_temperature_sensors();
    if (tempC1 > 0 && tempC1 < 80)
    {
      if (tempC2 > 0 && tempC2 < 80)
      {
        data_is_valid = true;
      }
    }
    if (data_is_valid == false)
    {
      Serial.println("invalid_temperature_data, START OVER");
    }
  }

  //read_multiple_temperature_sensors();
  hande_bubling();

  //delay(1000);
  if (Heaters_overritte == false)
  {
    handle_heaters(false);
  }
  

  if (enable_screen)
  {
    display_variable_on_screen();
  }
  //display_variable_on_screen();
  //handle_button();

  log_data2();
  handle_serial_receive();
}