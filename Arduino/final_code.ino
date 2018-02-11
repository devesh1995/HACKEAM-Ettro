#include <SoftwareSerial.h>
#include <math.h>

//bluetoothh connection
SoftwareSerial BTserial(10, 11); // RX | TX
int sensorPin = A0;
int sensorValue = 0;


int sensorPinlight = A6; // select the input pin for the temperature sensor

int sensorValuelight = 0; // select the input pin for the light sensor

int sensorPintemp = A5; //Analog temperatre connection. select the input pin for the potentiometer
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
   //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return Temp;
}


int buttonpinflames = 3; // define the flame sensor interface
int analoogflames = A3; // define the flame sensor interface
 
int val ;// define numeric variables val
float sensor; //read analoog value
 

//function for humidity sensor
int DHpin = 8;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}
 



void setup() {
//bluetooth serial begin
BTserial.begin(9600); 
pinMode (buttonpinflames, INPUT) ;// output interface defines the flame sensor
pinMode (analoogflames, INPUT) ;//flame sensor input
pinMode (DHpin, OUTPUT);//humidity sensor input
}

void loop() {

sensorValuelight = analogRead(sensorPinlight);


//code for analog temperature sensor

int readVal=analogRead(sensorPintemp);
double temp =  Thermistor(readVal);
BTserial.print(temp);

BTserial.print(",");

//code for light sensor

BTserial.print(sensorValuelight, DEC);

BTserial.print(",");

//code for humidity sensor

start_test ();
Serial.print (dat [0], DEC); // display the humidity-bit integer;
Serial.print ('.');
Serial.print (dat [1], DEC); // display the humidity decimal places;

BTserial.print(",");

//code for analog temperature sensor

sensor = analogRead(analoogflames);
Serial.println(sensor);  // display tempature
 
val = digitalRead (buttonpinflames) ;// digital interface will be assigned a value of 3 to read val
    if (val == HIGH) // When the flame sensor detects a signal, message sended
  {
    BTserial.print("There is no fire!");

  }
  else
  {
     BTserial.print("There is fire!");
  }

//BTserial.print("7.6 Ph");


BTserial.print(";");

//message to the receiving device

delay(2000);

}
