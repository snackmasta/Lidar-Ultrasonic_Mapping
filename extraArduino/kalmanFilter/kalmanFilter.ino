* HC-SR04 - Arduino UNO
 * Vcc    -> 5V
 * Trig   -> 9
 * Echo   -> 10
 * GND    -> GND
 */
 
// parameter sensor ultrasonik
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;
 
// parameter kalman filter
float SensorData, KalmanFilterData;
float Xt, Xt_update, Xt_prev;
float Pt, Pt_update, Pt_prev;
float Kt, R, Q;
 
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  R=10;
  Q=0.5;
  Pt_prev=1;
}
  
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
 
  // kalman filter
  SensorData = distance;
  Xt_update = Xt_prev;
  Pt_update = Pt_prev + Q;
  Kt = Pt_update/(Pt_update + R);
  Xt = Xt_update + (Kt * (SensorData - Xt_update));
  Pt = (1-Kt)*Pt_update;
 
  Xt_prev = Xt;
  Pt_prev = Pt;
 
  KalmanFilterData = Xt;
 
  Serial.print(SensorData,3);
  Serial.print(",");
  Serial.print(KalmanFilterData,3);
  Serial.println();
 
  //delayMicroseconds(10);
  
  //Serial.print("Distance: ");
  //Serial.println(distance);
}