/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
// defines pins numbers

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
const int LED_Pin = 1;  //LED
bool state = false;
// defines variables
long duration;
int distance;
int intensity = 1024;
bool object_detect = false;
const int t_on = 3000;//multiple of 1ms
const int t_off = 7000;//multiple of 1ms
bool light_on = false;
int buffer_on_time = 0;
int buffer_off_time = 0;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  analogWrite(LED_BUILTIN, 1024);
  Serial.begin(115200); // Starts the serial communication
  delay(100);
  pinMode(LED_Pin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  analogWrite(LED_Pin, 1024);
}

// the loop function runs over and over again forever
void loop()
{
  on_off();
}
void read_distance()
{
  static int old_distance = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  if (distance > 100)
    object_detect = false;
  else
    object_detect = true;
  delay(30);
  if (distance > 2000)
    distance = old_distance;
  else
    old_distance = distance;
  Serial.println(distance);
}

void on_off()
{
  read_distance();
  if (object_detect)
  {
    buffer_on_time++;
    buffer_off_time = 0;
  }
  else
  {
    buffer_on_time = 0;
    buffer_off_time++;
  }
  if ((buffer_on_time > 40) && (!light_on))
  {
    light_on = true;
    buffer_off_time = 0;
    buffer_on_time = 0;
    slow_on();
  }
  if ((buffer_off_time > 70) && (light_on))
  {
    light_on = false;
    buffer_off_time = 0;
    buffer_on_time = 0;
    slow_off();
  }
}

void slow_off()
{
  int noise = 0;
  for (int i = 0; i < 1024; i++)
  {
    intensity = i;
    analogWrite(LED_BUILTIN, i);
    analogWrite(LED_Pin, i);
    delay(1);
    if ((i % 10) == 0)
    {
      read_distance();
      if (distance < 100)
        noise++;
      if ((distance < 100) && (noise > 1))
      {
        light_on = true;
        buffer_off_time = 0;
        buffer_on_time = 0;
        slow_on();
        break;
      }

    }
  }
}
void slow_on()
{
  for (int i = intensity; i > 0; i--)
  {
    analogWrite(LED_BUILTIN, i);
    analogWrite(LED_Pin, i);
    delay(1);
  }
}
