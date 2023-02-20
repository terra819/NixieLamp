int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
boolean state = false;
extern volatile unsigned long timer0_millis;
unsigned long readTime;
int hour, min, sec;

byte dec_digits[10][2] = {
  {0b00000000, 0b00000001}, // 0
  {0b00000000, 0b00000010}, // 1
  {0b00000000, 0b00000100}, // 2
  {0b00000000, 0b00001000}, // 3
  {0b00000000, 0b00010000}, // 4
  {0b00000000, 0b00100000}, // 5
  {0b00000000, 0b01000000}, // 6
  {0b00000000, 0b10000000}, // 7
  {0b00000001, 0b00000000}, // 8
  {0b00000010, 0b00000000}  // 9
};

void setup() {
  Serial.begin(9600);
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String inString = Serial.readStringUntil('\n');
    int index1 = inString.indexOf(':');
    int index2 = inString.indexOf(':', index1 + 1);
    int index3 = inString.length();

    hour = inString.substring(0, index1).toInt();
    min = inString.substring(index1 + 1, index2).toInt();
    sec = inString.substring(index2 + 1, index3).toInt();

    timer0_millis = ((long)hour * 3600 + min * 60 + sec) * 1000;
  }
  readTime = millis() / 1000;
  if (millis() >= 86400000) {
    timer0_millis = 0;
  }
  sec = readTime % 60;
  min = (readTime / 60) % 60;
  hour = (readTime / (60 * 60)) % 24;

  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(latchPin, LOW);
  // shift out the bits in order of right to left:
  for (int i = 0; i < 2; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[min % 10][i]);
  }
  for (int i = 0; i < 2; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[min / 10][i]);
  }
  for (int i = 0; i < 2; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour % 10][i]);
  }
  for (int i = 0; i < 2; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour / 10][i]);
  }
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}

