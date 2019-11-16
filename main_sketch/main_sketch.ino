#define S0 11
#define S1 10
#define S2 8
#define S3 7
#define sensorOut 12

int frequency = 0;
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);
}
void loop() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  frequency = pulseIn(sensorOut, LOW);
  frequency = map(frequency, 550,1050,255,0);

  Serial.print("R= ");
  Serial.print(frequency);
  Serial.print("  ");

  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  frequency = map(frequency, 550,1050,255,0);

  Serial.print("G= ");
  Serial.print(frequency);
  Serial.print("  ");

  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  frequency = map(frequency, 550,1050,255,0);

  Serial.print("B= ");
  Serial.print(frequency);
  Serial.println("  ");
}
