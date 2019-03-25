

float value = 0;
void setup(){
   pinMode(A0, INPUT_PULLUP);
   Serial.begin(1000000);
}
void loop(){
   // read the value at analog input
   value = analogRead(A0);
   value = value/1023*5;
   Serial.println(value);
   } 
