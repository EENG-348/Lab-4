// EENG 348: Digital Systems - Lab 3
// Daniel Esguerra and Mawuli Akpalu

/**********SKETCH_PART1************/

// At 20 degrees Celsius, the speed
// of sound is 343 centimeters per
// microsecond (cm/us)
#define SPEED_OF_SOUND 0.0343

// Maximum distance the ulstrasonic
// rage finder can measure in cm
#define MAX_DISTANCE 60

// Declare global variables
int redPin = 3;
int greenPin = 5;
int bluePin = 6;

int echoPin = 13;
int triggerPin = 10;
int photocellPin = A0;

int redOutput = 0;
int blueOutput = 0;
int greenOutput = 0;
int brightOutput = 0;

double duration = 0;
double distance = 0;
double analogInput = 0;

void setup()
{
  // Set the baud rate for output
  Serial.begin(9600);

  // Set the echo and trigger pins
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
}

void loop()
{
  // Update the analog input value
  analogInput = analogRead(photocellPin);

  // Convert the analog input to a digital value
  // First divide by the max number for analogInput
  // (1023) to get a percentage of max voltage 
  // (5.0 V) at the inputPin.
  // Then multiply this percentage by the max LED
  // brightness (255) in order to express the
  // the analogInput as a digital bit pattern
  brightOutput = (int)(analogInput / 1023 * 255);

  // Send a pulse to initiate a measurement from
  // the ultrasonic range finder
  sendPulse();

  // Measure the duration at which the echo pin
  // is set to HIGH, which indicates how long it
  // took for the ultrasonic wave to return to the
  // range finder
  duration = pulseIn(echoPin, HIGH);

  // Translate the duration into the distance
  // travelled by the ultrasonic wave in one
  // direction
  distance = timeToDistance(duration);

  // Get the appropriate red value that is dependent
  // on the distance
  redOutput = getRedValue(distance);

  // Set the green value as the opposite brightness
  // of the red output
  greenOutput = brightOutput - redOutput;

  // Set the blue value to LOW since it is never
  // in use for this part
  blueOutput = 0;

  // Write the LED outputs to the LED
  // Since the anode of the LED is connected
  // to 5V (HIGH), the values sent to the 
  // ground pins of the LED must be the opposite
  // of the intensity expected
  analogWrite(redPin, 255 - redOutput);
  analogWrite(greenPin, 255 - greenOutput);
  analogWrite(bluePin, 255 - blueOutput);
  
  // Print the current analog input value
  Serial.print("Current Analog Input Value: ");
  Serial.println(analogInput);

  // Print the current brightness level
  Serial.print("Current Brightness Level: ");
  Serial.println(brightOutput);

  // Print the current distance measured
  Serial.print("Current Distance in Centimeters: ");
  Serial.println(distance);
  
  // Print the current LED output intensities
  Serial.print("Current red LED Output Intensity: ");
  Serial.println(redOutput);
  Serial.print("Current green LED Output Intensity: ");
  Serial.println(greenOutput);
  Serial.print("Current blue LED Output Intensity: ");
  Serial.println(blueOutput);
  Serial.println();

  // Loop every second (1000 milliseconds)
  delay(1000);
}

// Send a pulse to initiate a measurement from
// the ultrasonic range finder for the length
// specified in the datasheet
void sendPulse()
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

// Converts the time elapsed in microseconds
// to a distance traveled in centimeters
double timeToDistance(double duration)
{
  // Divide the time in two since the time
  // covers both the outward and return trip
  // of the ultrasonic wave
  duration = duration / 2;

  // Multiply the time by the speed of sound in cm/us
  // to get how many centimeters the ultrasonic wave
  // traveled in the elapsed time and return this value
  return duration * SPEED_OF_SOUND;
}

// Takes the distance and returns a number between 0
// and 255 depending on how the distance relates to
// the maximum distance the range finder can measure
int getRedValue(double distance)
{
  // If the distance is greater than the MAX_DISTANCE
  // then set the red output to the maximum brightness
  if (distance > MAX_DISTANCE)
    return brightOutput;
    
  // Since the anode of the LED is always set to
  // HIGH, set the brightOutput variable to the
  // opposite of the above calculation
  return (int)(distance / MAX_DISTANCE * brightOutput);
}
