#include <DHT.h>
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
//#define Offset 0.00            //deviation compensate
DHT dht (DHTPIN, DHTTYPE);
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
/* water level */
#define trigPin 12
 #define echoPin 11
 #define buzzer 9 
 
const int DOUTpin11=10;//the DOUT pin of the mq135 sensor goes into digital pin 24 of the arduino
const int AOUTpin11=A3;//the AOUT pin of the mq135 sensor goes into analog pin A3 of the arduino
int digitalValue;
/* MQ-7 Carbon Monoxide Sensor Circuit with Arduino */

const int AOUTpin=A4;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int DOUTpin=13;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino
const int ledPin=10;//the anode of the LED connects to digital pin D13 of the arduino

int limit,duration,distance,waterleve;
int value;
 
int sensorValue2,i,turbidityValue;
float turbidityV,t,h;
 
 
void setup()

{
 Serial.begin(9600);  
 pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 Serial.println("pH meter experiment!");  //Test the serial monitor
dht.begin();
pinMode(buzzer, OUTPUT);
pinMode(DOUTpin, INPUT);
pinMode(DOUTpin11, INPUT);
}

void loop(void)

{
ph1();
delay(1000);
mq77();
delay(1000);
mq1355();
delay(1000);
dhtvalue();
delay(1000);
turbidity();
delay(1000);
waterlevel();
delay(1000);
}
void ph1()
{for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*6.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 //float phValue = 3.3*phValue+Offset;
 Serial.print("sensor = ");
 Serial.println(phValue);
 
 delay(20);
 if(phValue >= 7.30){
    digitalWrite(buzzer, HIGH);
    Serial.print("PH VALUE: ");
    Serial.println(phValue); 
    Serial.println("Water Alkalinity high");
    delay(1000);
   }

  if(phValue >= 6.90 && phValue <= 7.19){
    digitalWrite(buzzer, LOW);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    Serial.println("Water Is  neutral (safe)");
  }

  if(phValue < 6.89){
    
    digitalWrite(buzzer, HIGH);
    Serial.print("PH VALUE: ");
    Serial.println(phValue); 
    Serial.println("Water Acidity High");
    delay(1000);
   }
}
void waterlevel(){

digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343;
waterleve=map(distance,0,15,100,0);
 Serial.println(" .............WATER LEVEL..................");
  Serial.print(waterleve);
  Serial.println("%");
  delay(1000);  
  if(waterleve <20 )
  {
    digitalWrite(buzzer,HIGH); 
  }
  else if(waterleve >80 )
  {
        digitalWrite(buzzer,HIGH); 
  }
  else{
    digitalWrite(buzzer,LOW); 
  }
}
void mq77(){

value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
Serial.print("CO value: ");
Serial.println(value);//prints the CO value
Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);
if (limit == HIGH){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
}
else{
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
} 
}
void mq1355(){
  
  sensorValue2 = analogRead(AOUTpin11);       // read analog input pin 0

  digitalValue = digitalRead(DOUTpin11); 
Serial.print("Gas value: ");
  Serial.println(sensorValue2, DEC);  // prints the value read
Serial.print("Limit: ");
  Serial.println(digitalValue, DEC);

  delay(2000);                        // wait 100ms for next reading


}
void dhtvalue()
{
  delay(2000);
 h = dht.readHumidity();
 t = dht.readTemperature();

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
}

void turbidity(){

   turbidityValue = analogRead(A1);// read the input on analog pin 0:
   turbidityV = turbidityValue/100; // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(turbidityV); // print out the value you read:
  delay(500);

  Serial.print("Turbidity level: ");
  Serial.println(turbidityV);

  if( turbidityV > 9){
    digitalWrite(buzzer, HIGH);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Very Clean ");
    delay(1000);
  }

  if( turbidityV >= 6 && turbidityValue/100 <= 9 ){
    digitalWrite(buzzer, LOW);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Clean ");
  }

  if( turbidityV < 6){
    digitalWrite(buzzer, HIGH);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Very Dirty ");
    delay(1000);
   }
  delay (2000);
}


