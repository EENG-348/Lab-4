// EENG 348: Digital Systems - Lab 3
// Daniel Esguerra and Mawuli Akpalu

/**********SKETCH_PART3************/

#include <Adafruit_SSD1306.h>

#define TOP_EDGE 4
#define BOT_EDGE 60
#define LEFT_EDGE 4
#define RIGHT_EDGE 124

int xCord = 10;
int yCord = 10;

int oledCS = 4;
int oledDC = 5;
int oledSI = 7;
int oledRST = 6;
int oledCLK = 8;

int rotaryPinA = 2;
int rotaryPinB = 3;

int ballVelocity = 0;

int currentRotaryInputA = 0;
int currentRotaryInputB = 0;

volatile int velocity = 128;
volatile int currentState = 0;
volatile int previousState = 0;
volatile long combinedState = 0;

bool movingLeft = false;
bool movingDown = true;
bool positiveVelocity = false;

// Set up the display for the OLED screen
Adafruit_SSD1306 disp(128, 64, oledSI, oledCLK, oledDC, oledRST, oledCS);

void setup()
{
  // Set the baud rate for output
  Serial.begin(9600);

  // Set rotary pins for input
  pinMode(rotaryPinA, INPUT);
  pinMode(rotaryPinB, INPUT);

  // Set both pins to passive pull up;
  digitalWrite(rotaryPinA, HIGH);
  digitalWrite(rotaryPinB, HIGH);

  // Call interrupts if there is any change to the values of our pins
  attachInterrupt(digitalPinToInterrupt(2), getNewState, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), getNewState, CHANGE);

  // Start and clear the display
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
}

void loop()
{
  // Clear the display and display a new ball
  disp.clearDisplay();
  disp.fillCircle(xCord, yCord, 4, WHITE);
  disp.display();

  // Calculate the ball velocity using 128
  // as stopped, and anything greater or lower
  // as 2 different directions of travel
  ballVelocity = (128 - velocity) / 20;

  // If the ball velocity changed sign,
  // change the direction of travel
  if (ballVelocity < 0 && positiveVelocity)
  {
    movingLeft = !movingLeft;
    movingDown = !movingDown;
    positiveVelocity = false;
  }
  else if (ballVelocity > 0 && !positiveVelocity)
  {
    movingLeft = !movingLeft;
    movingDown = !movingDown;
    positiveVelocity = true;
  }

  // Keep the absolute value of the ball velocity
  if (ballVelocity < 0)
    ballVelocity = ballVelocity - 2 * ballVelocity;

  // Modify the xCord based on the current direction
  // of the ball
  if (movingLeft)
    xCord -= ballVelocity;
  else
    xCord += ballVelocity;

  // Modify the yCord based on the current direction
  // of the ball
  if (movingDown)
    yCord += ballVelocity;
  else
    yCord -= ballVelocity;

  // If the ball has reached a boundary, switch the
  // direction of travel
  if (xCord < LEFT_EDGE)
    movingLeft = false;
  else if (xCord > RIGHT_EDGE)
    movingLeft = true;
  if (yCord < TOP_EDGE)
    movingDown = true;
  else if (yCord > BOT_EDGE)
    movingDown = false;

  // Print the current velocity
  Serial.println(positiveVelocity);
  Serial.println(ballVelocity);
  Serial.println();
}

void getNewState()
{
  // Update the current rotary inputs for both
  // Pin A and Pin B of the rotary encoder
  currentRotaryInputA = digitalRead(rotaryPinA);
  currentRotaryInputB = digitalRead(rotaryPinB);

  // Concatenate the state of both pins into the 2 least significant
  // bits of the current state
  currentState = (currentRotaryInputA << 1) | currentRotaryInputB;

  // Concatenate the current state of both pins with the previous
  // state of both pins into the 4 least significant bits of the
  // combined state
  combinedState = (previousState << 2) | currentState;

  // If the combined state signifies a clockwise rotation
  // and the velocity does not exceed 255, increase the
  // current velocity
  if (combinedState == 0b1101 || combinedState == 0b0100 || 
      combinedState == 0b0010 || combinedState == 0b1011)
      velocity = (velocity < 255) ? (velocity+1) : 255;

  // If the combined state signifies a counter-clockwise 
  // rotation and the velocity does not exceed 255, increase
  // the current velocity
  if (combinedState == 0b1110 || combinedState == 0b0111 || 
      combinedState == 0b0001 || combinedState == 0b1000)
      velocity = (velocity > 0) ? (velocity-1) : 0;

  // Set the previous state to the current state
  // for the next interation of the interrupt
  previousState = currentState;
}
