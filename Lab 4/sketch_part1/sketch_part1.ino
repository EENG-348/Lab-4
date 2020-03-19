// EENG 348: Digital Systems - Lab 3
// Daniel Esguerra and Mawuli Akpalu

/**********SKETCH_PART1************/

// Declare global variables
int ledPin = 3;
int inputPin = A0;
int ledVoltage = 0;
int digitalOutput = 0;
double analogInput = 0;

void setup()
{
  // Set the baud rate for output
  Serial.begin(9600);
}

void loop()
{
  // Update the analog input value
  // In order to read the analog input
  // without using the function analogRead(),
  // one could set an analog pin as an input
  // pin and read the value at that pin
  analogInput = analogRead(inputPin);

  // Convert the analog input to a digital value
  // First divide by the max number for analogInput
  // (1023) to get a percentage of max voltage 
  // (5.0 V) at the inputPin.
  // Then multiply this percentage by the max LED
  // brightness (255) in order to express the
  // the analogInput as a digital bit pattern
  digitalOutput = (int)(analogInput / 1023 * 255);

  // Set the LED voltage using the value of
  // digitalOutput and the PWM of the Arduino
  analogWrite(ledPin, digitalOutput);
  
  // Print the current analog input value
  Serial.print("Current Analog Input Value: ");
  Serial.println(analogInput);

  // Print the current digital output value
  Serial.print("Current Digital Output Value: ");
  Serial.println(digitalOutput);
  Serial.println();
  
  // Loop every second (1000 milliseconds)
  delay(1000);
}
