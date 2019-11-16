#include <IRremoteInt.h>
#include <IRremote.h>

#define S0 11
#define S1 10
#define S2 8
#define S3 7
#define sensorOut 12
#define RECV_PIN 2

int frequency = 0;
IRrecv irrecv(RECV_PIN);
uint8_t locations[8];

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  irrecv.enableIRIn();
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);
}

bool locations_was_recieved = false;

uint8_t dehamm(uint8_t b) {
  size_t s1 = ((b >> 6) & 1) ^ ((b >> 4) & 1) ^ ((b >> 2) & 1) ^ ((b >> 0) & 1);
  size_t s2 = ((b >> 5) & 1) ^ ((b >> 4) & 1) ^ ((b >> 1) & 1) ^ ((b >> 0) & 1);
  size_t s3 = ((b >> 3) & 1) ^ ((b >> 2) & 1) ^ ((b >> 1) & 1) ^ ((b >> 0) & 1);
  if (s1 + s2 + s3) {
      b ^= 1 << (7 - s1 * 1 - s2 * 2 - s3 * 4);
  }
  uint8_t result = 0;
  result |= b & 7;
  result |= (b >> 1) & 8;
  return result;
}

void decode(uint8_t *bytes, size_t n) {
  for (size_t i = 0; i < n; i++) {
      bytes[i] = dehamm(bytes[i]);
  }
}

bool try_recieve_locations() {
  static enum Recieve_Stage {
    START = -1,
    FIRST_FIELD = 0,
    SECOND_FIELD = 1,
    FINISHED = 2,
  } stage = START;
  static decode_results results;
  if (irrecv.decode(&results)) {
    if (stage == START) {
      if (results.value == 0xffffffff) {
        stage = FIRST_FIELD;
      }
    } else if (stage == FIRST_FIELD || stage == SECOND_FIELD) {
      int start = stage * 4;
      locations[start]     = (uint8_t)(results.value >> 24);
      locations[start + 1] = (uint8_t)(results.value >> 16);
      locations[start + 2] = (uint8_t)(results.value >> 8);
      locations[start + 3] = (uint8_t)(results.value);
      stage = Recieve_Stage(int(stage) + 1);
    } else {
      decode(locations, 8);
      return true;
    }
    irrecv.resume();
  }
  return false;
}

void loop() {
  if (!locations_was_recieved && !(locations_was_recieved = try_recieve_locations())) {
    Serial.println("still getting info");
    return;
  }
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,LOW);
//  frequency = pulseIn(sensorOut, LOW);
//  frequency = map(frequency, 550,1050,255,0);
//
//  Serial.print("R= ");
//  Serial.print(frequency);
//  Serial.print("  ");
//
//  digitalWrite(S2,HIGH);
//  digitalWrite(S3,HIGH);
//  frequency = pulseIn(sensorOut, LOW);
//  frequency = map(frequency, 550,1050,255,0);
//
//  Serial.print("G= ");
//  Serial.print(frequency);
//  Serial.print("  ");
//
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,HIGH);
//  frequency = pulseIn(sensorOut, LOW);
//  frequency = map(frequency, 550,1050,255,0);
//
//  Serial.print("B= ");
//  Serial.print(frequency);
//  Serial.println("  ");
}
