// ESP32 Speaker Test on GPIO 18
const int speakerPin = 18;  // Speaker signal pin

void setup() {
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // Play a 1 kHz tone for 500 ms
  tone(speakerPin, 1000);  
  delay(500);

  // Stop the tone
  noTone(speakerPin);      
  delay(500);
}

