int incomingByteMSB = 0;   // for incoming serial data
int incomingByteLSB = 0; 
int incomingByte = 0; 

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
        
        // send data only when you receive data:
        if (Serial.available() == 2) {
                incomingByteMSB = Serial.read();  
                incomingByteLSB = Serial.read();

                incomingByte = incomingByteMSB;             
                incomingByte = incomingByte<<8;         
                incomingByte |= incomingByteLSB;               
                
                Serial.write(incomingByte);
                Serial.write(incomingByte>>8);
                
        }
        else{
          digitalWrite(LED_BUILTIN, LOW);
          }
}
