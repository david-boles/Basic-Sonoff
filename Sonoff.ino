void setupSonoff() {
  pinMode(13, OUTPUT);//LED
  pinMode(12, OUTPUT);//RELAY
  pinMode(0, INPUT);//RELAY
}

void setLED(boolean set) {
  digitalWrite(13, !set);
}

void setRelay(boolean set) {
  digitalWrite(12, set);
}

bool getButton() {
  return !digitalRead(0);
}

