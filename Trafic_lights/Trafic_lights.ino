// Define pin numbers here
int green_pedestrian = 3;
int red_pedestrian = 4;

int green_car = 5;
int yellow_car = 6;
int red_car = 7;

int button = 2;

// Initially the flag is low
int pedestrian_requested_crossing = LOW;

int on = LOW;
int off = HIGH;
 
void setup() {
  // Initialize necessary pins as digital outputs
  // Reset pins with digitalWrite
  pinMode(green_pedestrian, OUTPUT);
  digitalWrite(green_pedestrian, off);
  pinMode(red_pedestrian, OUTPUT);
  digitalWrite(red_pedestrian, off);

  pinMode(green_car, OUTPUT);
  digitalWrite(green_car, off);
  pinMode(yellow_car, OUTPUT);
  digitalWrite(yellow_car, off);
  pinMode(red_car, OUTPUT);
  digitalWrite(red_car, off);

  // Initialize button pin as digital input
  // Enable pull-up resistor on button pin
  // Associate button interrupt handler function (onButtonPressed) below with the event on the button
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), onButtonPressed, FALLING);

}

void onButtonPressed() {
  pedestrian_requested_crossing = HIGH;
}

void loop() {
    Serial.println("Cars green, pedestrians red for 10 seconds");
    digitalWrite(red_car, off);
    digitalWrite(green_car, on);

    digitalWrite(green_pedestrian, off);
    digitalWrite(red_pedestrian, on);
    
    delay(5000);

    if (pedestrian_requested_crossing) {

        Serial.println("Green for cars is blinking 5 seconds");
        for(int j = 0; j < 8; j++) {
            digitalWrite(green_car, j % 2 == 0);
            delay(500);
        }

        Serial.println("Cars red, pedestrians green for 5 seconds");
        digitalWrite(green_car, off);
        
        digitalWrite(yellow_car, on);
        delay(2000);
        digitalWrite(yellow_car, off);
        
        digitalWrite(red_car, on);
        delay(500);
        digitalWrite(red_pedestrian, off);
        delay(500);
        digitalWrite(green_pedestrian, on);
        
        delay(5000);
        
        pedestrian_requested_crossing = LOW;

        for(int j = 0; j < 10; j++) {
            digitalWrite(green_pedestrian, j % 2 == 0);
            delay(500);
        }
        digitalWrite(green_pedestrian, off);
        digitalWrite(red_pedestrian, on);

        digitalWrite(red_car, off);
        digitalWrite(yellow_car, on);
        delay(1000);
        digitalWrite(yellow_car, off);
        digitalWrite(green_car, on);
    }
}
