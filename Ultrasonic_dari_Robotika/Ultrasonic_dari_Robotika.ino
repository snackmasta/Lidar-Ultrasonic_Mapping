int trig_pin = 12; //Mendefinisikan pin trigger sensor ke pin 2 arduino
int echo_pin = 11; //Mendefinisikan pin echo sensor ke pin 3 arduino
long echotime; //Mendefinisikan output sensor dalam bentuk satuan waktu
float distance; //Mendefinisikan output sensor dalam bentuk jarak

void setup() {
  Serial.begin(9600); //inisisasi
  pinMode(trig_pin, OUTPUT); //Mendefinisikan trigger sebagai output
  pinMode(echo_pin, INPUT); //Mendefinisikan echp sebagai input
  digitalWrite(trig_pin, LOW); //memposisikan trigger ke mode off
}
void loop() {
  digitalWrite(trig_pin, HIGH); //memposisikan trigger ke mode on
  delayMicroseconds(10); //memberi delay 10mS
  digitalWrite(trig_pin, LOW); //memposisikan trigger ke mode off
  echotime= pulseIn(echo_pin, HIGH); //mendefinisikan pulsa yang masuk sebagai echotime
  distance= 0.0001*((float)echotime*340.0)/2.0; //mengkonversi echotime ke dalam satuan jarak
  Serial.print(distance); //Mencetak hasil pembacaan ke serial monitor
  Serial.println(" cm"); //Memberi satuan cm ke hasil pembacaan
  delay(1000);
}