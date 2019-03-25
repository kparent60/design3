#include <stdlib.h>
#include <digitalWriteFast.h>

/****** Moteur 1 ******/
// Codeur incrÃƒÂ©mental
#define encodeurPinA_moteur1 21
#define encodeurPinB_moteur1 17
volatile int ticksCodeur_moteur1 = 0;
 
// Commande envoyer
#define commandePin1_moteur1 22 //controle de direction
#define commandePin2_moteur1 23 //controle de direction
#define commandePWM_moteur1  6 //controle de vitesse

float omega_moteur1 = 0;
double vitesse_moteur1 = 0;
double Input_moteur1 =0;
double Output_moteur1 =0;
double Outputf_moteur1 =0;
double Output_moteur1_A =0;
double Output_moteur1_B =0;
double Setpoint_moteur1 =0;

double Erreur_moteur1 = 0;
double ErreurPrecedante_moteur1 =0;

double temps_1 = 0;
double tempsD_1 = 0;

double consigne_position_moteur1 = 0;
double position_moteur1 = 0;
/**********************/

/****** Moteur 2 ******/
// Codeur incrÃƒÂ©mental
#define encodeurPinA_moteur2 20
#define encodeurPinB_moteur2 16
volatile int ticksCodeur_moteur2 = 0;
 
// Commande envoyer
#define commandePin1_moteur2 24 //controle de direction
#define commandePin2_moteur2 25 //controle de direction
#define commandePWM_moteur2 7 //controle de vitesse

double omega_moteur2 = 0;
double Input_moteur2 =0;
double Output_moteur2 =0;
double Outputf_moteur2 =0;
double Output_moteur2_A =0;
double Output_moteur2_B =0;
double Setpoint_moteur2 =0;

double Erreur_moteur2 = 0;
double ErreurPrecedante_moteur2 =0;

double temps_2 = 0;
double tempsD_2 = 0;

double consigne_position_moteur2 = 0;
double position_moteur2 = 0;
/**********************/

/****** Moteur 3 ******/
// Codeur incrÃƒÂ©mental
#define encodeurPinA_moteur3 19 
#define encodeurPinB_moteur3 15
volatile int ticksCodeur_moteur3 = 0; 
 
// Commande envoyer
#define commandePin1_moteur3 26 //controle de direction
#define commandePin2_moteur3 27 //controle de direction
#define commandePWM_moteur3 11 //controle de vitesse

float omega_moteur3 = 0;
double Input_moteur3 =0;
double Output_moteur3 =0;
double Outputf_moteur3 =0;
double Output_moteur3_A =0;
double Output_moteur3_B =0;
double Setpoint_moteur3 =0;

double Erreur_moteur3 = 0;
double ErreurPrecedante_moteur3 =0;

double temps_3 = 0;
double tempsD_3 = 0;

double consigne_position_moteur3 = 0;
double position_moteur3 = 0;
/**********************/

/****** Moteur 4 ******/
// Codeur incrÃƒÂ©mental
#define encodeurPinA_moteur4 18
#define encodeurPinB_moteur4 14
volatile int ticksCodeur_moteur4 = 0;
 
// Commande envoyer
#define commandePin1_moteur4 28 //controle de direction
#define commandePin2_moteur4 29 //controle de direction
#define commandePWM_moteur4 12 //controle de vitesse

float omega_moteur4 = 0;
double Input_moteur4 =0;
double Output_moteur4 =0;
double Outputf_moteur4 =0;
double Output_moteur4_A =0;
double Setpoint_moteur4 =0;

double Erreur_moteur4 = 0;
double ErreurPrecedante_moteur4 =0;

double temps_4 = 0;
double tempsD_4 = 0;

double consigne_position_moteur4 = 0;
double position_moteur4 = 0;
/**********************/
 
double Consigne_vitesse = 40;
double Consigne_rotation = 20;

int consigne = 0;
int consigne1 = 0;
int consigne2 = 0;
int consigne3 = 0;
int consigne4 = 0;
int consigne_angle = 0;
double consigne_Angle = 0;
double Angle = 0;
double Angle_precedant = 0;
double vitesse_moteur = 0;
double Vmax = 6;
// Cadence d'envoi des donnÃ©es en ms
#define TSDATA 100
double tempsDernierEnvoi = 0;
double tempsCourant = 0;

#define Pin_pi_connect 52
int integerValue_X = 0;
int integerValue_Y = 0; 
int integerValue_Angle = 0;
bool negativeNumber=false; // track if number is negative
char incomingByte;

float tension_condensateur = 0;


/*******   PI   *******/
const double Kp = 0.84 ;
const double Ki = 5.17 ;
/**********************/

// Initialisations
void setup(void) {
 
  // Codeur incrÃ©mental
  pinMode(encodeurPinA_moteur1, INPUT_PULLUP);      // entrÃ©e digitale pin A codeur
  pinMode(encodeurPinB_moteur1, INPUT_PULLUP);      // entrÃ©e digitale pin B codeur
  attachInterrupt(digitalPinToInterrupt(encodeurPinA_moteur1), GestionInterruption_moteur1, CHANGE);
  pinMode(encodeurPinA_moteur2, INPUT_PULLUP);      // entrÃ©e digitale pin A codeur
  pinMode(encodeurPinB_moteur2, INPUT_PULLUP);      // entrÃ©e digitale pin B codeur
  attachInterrupt(digitalPinToInterrupt(encodeurPinA_moteur2), GestionInterruption_moteur2, CHANGE);
  pinMode(encodeurPinA_moteur3, INPUT_PULLUP);      // entrÃ©e digitale pin A codeur
  pinMode(encodeurPinB_moteur3, INPUT_PULLUP);      // entrÃ©e digitale pin B codeur
  attachInterrupt(digitalPinToInterrupt(encodeurPinA_moteur3), GestionInterruption_moteur3, CHANGE);
  pinMode(encodeurPinA_moteur4, INPUT_PULLUP);      // entrÃ©e digitale pin A codeur
  pinMode(encodeurPinB_moteur4, INPUT_PULLUP);      // entrÃ©e digitale pin B codeur
  attachInterrupt(digitalPinToInterrupt(encodeurPinA_moteur4), GestionInterruption_moteur4, CHANGE);

  // Commande envoyer
  pinMode(commandePin1_moteur1, OUTPUT);
  pinMode(commandePin2_moteur1, OUTPUT);
  pinMode(commandePWM_moteur1, OUTPUT);
  pinMode(commandePin1_moteur2, OUTPUT);
  pinMode(commandePin2_moteur2, OUTPUT);
  pinMode(commandePWM_moteur2, OUTPUT);
  pinMode(commandePin1_moteur3, OUTPUT);
  pinMode(commandePin2_moteur3, OUTPUT);
  pinMode(commandePWM_moteur3, OUTPUT);
  pinMode(commandePin1_moteur4, OUTPUT);
  pinMode(commandePin2_moteur4, OUTPUT);
  pinMode(commandePWM_moteur4, OUTPUT);


  //com pi
  pinMode(Pin_pi_connect, OUTPUT);
  digitalWrite(Pin_pi_connect, HIGH);

  //tension condensateur
  pinMode(A0, INPUT);

  
  // Liaison sÃ©rie
//  Serial.begin(1000000);
//  Serial.setTimeout(5);
//  Serial.flush();
 
 
}
 
// Boucle principale
void loop() {

  lire_consigne();
  ecritureData();
  
}

void lire_consigne(void){


if (Serial.available() > 0) {   // something came across serial
            integerValue_X = 0;         // throw away previous integerValue
            integerValue_Y = 0;
            integerValue_Angle = 0; 
            negativeNumber = false;  // reset for negative
            while(1) {            // force into a loop until 'n' is received
              incomingByte = Serial.read();
              if (incomingByte == ',') break;   // exit the while(1), we're done receiving
              if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
              if (incomingByte == '-') {
                negativeNumber=true;
                continue;
            }
            integerValue_X *= 10;  // shift left 1 decimal place
            // convert ASCII to integer, add, and shift left 1 decimal place
            integerValue_X = ((incomingByte - 48) + integerValue_X);
          }
    if (negativeNumber){
      integerValue_X = -integerValue_X;
    }
        negativeNumber = false;
          while(1) {            // force into a loop until 'n' is received
            incomingByte = Serial.read();
            if (incomingByte == ',') break;   // exit the while(1), we're done receiving
            if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
            if (incomingByte == '-') {
              negativeNumber=true;
              continue;
          }
            integerValue_Y *= 10;  // shift left 1 decimal place
            // convert ASCII to integer, add, and shift left 1 decimal place
            integerValue_Y = ((incomingByte - 48) + integerValue_Y);
          }
    if (negativeNumber){
      integerValue_Y = -integerValue_Y;
    }
    negativeNumber = false;
          while(1) {            // force into a loop until 'n' is received
            incomingByte = Serial.read();
            if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
            if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
            if (incomingByte == '-') {
              negativeNumber=true;
              continue;
          }
            integerValue_Angle *= 10;  // shift left 1 decimal place
            // convert ASCII to integer, add, and shift left 1 decimal place
            integerValue_Angle = ((incomingByte - 48) + integerValue_Angle);
          }
    if (negativeNumber){
      integerValue_Angle = -integerValue_Angle;
    }
    consigne1 = integerValue_X;
    consigne4 = integerValue_X;
    consigne2 = integerValue_Y;
    consigne3 = integerValue_Y;
    consigne_angle = integerValue_Angle; 
  }

    consigne_position_moteur1 = consigne1;
    consigne_position_moteur4 = consigne4;
    consigne_position_moteur2 = consigne2;
    consigne_position_moteur3 = consigne3;
    consigne_Angle = consigne_angle;

    if (consigne_Angle == 0){
      
      if (consigne_position_moteur1 < 0){
        Setpoint_moteur1 = -Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur1 > 0){
        Setpoint_moteur1 = Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur1 == 0){
        Setpoint_moteur1 = 0;
      }
  
      if (consigne_position_moteur2 < 0){
        Setpoint_moteur2 = -Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur2 > 0){
        Setpoint_moteur2 = Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur2 == 0){
        Setpoint_moteur2 = 0;
      }
  
      if (consigne_position_moteur3 < 0){
        Setpoint_moteur3 = -Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur3 > 0){
        Setpoint_moteur3 = Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur3 == 0){
        Setpoint_moteur3 = 0;
      } 
  
      if (consigne_position_moteur4 < 0){
        Setpoint_moteur4 = -Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur4 > 0){
        Setpoint_moteur4 = Consigne_vitesse*255/100;
      }
      if (consigne_position_moteur4 == 0){
        Setpoint_moteur4 = 0;
      }
    } 

    if (consigne_Angle  > 0){
      Setpoint_moteur1 = Consigne_rotation*255/100;
      Setpoint_moteur2 = Consigne_rotation*255/100;
      Setpoint_moteur3 = -Consigne_rotation*255/100;
      Setpoint_moteur4 = -Consigne_rotation*255/100;
    }
    
    if (consigne_Angle  < 0){
      Setpoint_moteur1 = -Consigne_rotation*255/100;
      Setpoint_moteur2 = -Consigne_rotation*255/100;
      Setpoint_moteur3 = Consigne_rotation*255/100;
      Setpoint_moteur4 = Consigne_rotation*255/100;
    }   

    if (consigne_Angle == 0 && consigne_position_moteur1 == 0 && consigne_position_moteur2 == 0 && consigne_position_moteur3 == 0 && consigne_position_moteur4 == 0){
      digitalWrite(Pin_pi_connect, HIGH);
    }
    if (consigne_Angle != 0 || consigne_position_moteur1 != 0 || consigne_position_moteur2 != 0 || consigne_position_moteur3 != 0 || consigne_position_moteur4 != 0){
       digitalWrite(Pin_pi_connect, LOW);
    }
    
      
}

 
void isrt(){
 
  double codeurDeltaPos_moteur1; 
  double codeurDeltaPos_moteur2; 
  double codeurDeltaPos_moteur3; 
  double codeurDeltaPos_moteur4;
 
  // Nombre de ticks codeur depuis la derniÃ¨re fois
  codeurDeltaPos_moteur1 = ticksCodeur_moteur1;
  ticksCodeur_moteur1 = ticksCodeur_moteur1-codeurDeltaPos_moteur1;
  codeurDeltaPos_moteur2 = ticksCodeur_moteur2;
  ticksCodeur_moteur2 = ticksCodeur_moteur2-codeurDeltaPos_moteur2;
  codeurDeltaPos_moteur3 = ticksCodeur_moteur3;
  ticksCodeur_moteur3 = ticksCodeur_moteur3-codeurDeltaPos_moteur3;
  codeurDeltaPos_moteur4 = ticksCodeur_moteur4;
  ticksCodeur_moteur4 = ticksCodeur_moteur4-codeurDeltaPos_moteur4;

  
  // Calcul de la vitesse de rotation
  temps_1 = micros();
  omega_moteur1 = ((float)((2.*3.141592/6533*((float)codeurDeltaPos_moteur1)))/(temps_1-tempsD_1)*1000000);  // en rad/s
  Input_moteur1 = omega_moteur1*255/Vmax;
  tempsD_1 = temps_1; 
    
  
  if (consigne_Angle ==0){
    position_moteur1 = position_moteur1 + omega_moteur1*(2*3.141592)*32/31.75;
    if  ( abs(consigne_position_moteur1) <= abs(position_moteur1+ omega_moteur1*(2*3.141592)*32/31.75*3 )){
      Setpoint_moteur1 = 0;
      consigne_position_moteur1 = 0;
      position_moteur1 = 0;
      consigne1 =0;
    }
  }
  
  temps_2 = micros();
  omega_moteur2 = ((float)((2.*3.141592/6533*((float)codeurDeltaPos_moteur2)))/(temps_2-tempsD_2)*1000000);  // en rad/s
  Input_moteur2 = omega_moteur2*255/Vmax;
  tempsD_2 = temps_2;
  
  
  if (consigne_Angle ==0){  
    position_moteur2 = position_moteur2 + omega_moteur2*(2*3.141592)*32/31.76;
    if  ( abs(consigne_position_moteur2) <= abs(position_moteur2+ omega_moteur2*(2*3.141592)*32/31.76*3 )){
      Setpoint_moteur2 = 0;
      consigne_position_moteur2 = 0;
      position_moteur2 = 0;
      consigne2 =0;
    }
  }
  
  temps_3 = micros();
  omega_moteur3 = ((float)((2.*3.141592/6533*((float)codeurDeltaPos_moteur3)))/(temps_3-tempsD_3)*1000000);  // en rad/s
  Input_moteur3 = omega_moteur3*255/Vmax;
  tempsD_3 = temps_3;

  if (consigne_Angle ==0){ 
    position_moteur3 = position_moteur3 + omega_moteur3*(2*3.141592)*32/30.84;
    if  ( abs(consigne_position_moteur3) <= abs(position_moteur3+ omega_moteur3*(2*3.141592)*32/30.84*3 )){
      Setpoint_moteur3 = 0;
      consigne_position_moteur3 = 0;
      position_moteur3 = 0;
      consigne3 =0;
    }
  }

  
  temps_4 = micros();
  omega_moteur4 = ((float)((2.*3.141592/6533*((float)codeurDeltaPos_moteur4)))/(temps_4-tempsD_4)*1000000);  // en rad/s
  Input_moteur4 = omega_moteur4*255/Vmax;
  tempsD_4 = temps_4;

  if (consigne_Angle ==0){
    position_moteur4 = position_moteur4 + omega_moteur4*(2*3.141592)*32/31.6;
    if  ( abs(consigne_position_moteur4) <= abs(position_moteur4+ omega_moteur4*(2*3.141592)*32/31.6*3 )){
      Setpoint_moteur4 = 0;
      consigne_position_moteur4 = 0;
      position_moteur4 = 0;
      consigne4 =0;
    }
  }

  if (consigne_Angle =! 0){
    vitesse_moteur = omega_moteur1*100/Vmax;
    Angle = Angle + 1.85;//(-3.773* vitesse_moteur + 3.974* vitesse_moteur + Angle_precedant)*1.05; 

    if (abs(consigne_angle) <= abs(Angle)){
      consigne_Angle = 0;
      consigne_angle = 0;
      Angle = -1.85;
      Angle_precedant = 0;
      }
  }

  
}
 
void ecritureData(void) {
  
  tempsCourant = millis();
  
  if (tempsCourant-tempsDernierEnvoi > TSDATA) {
    isrt();
    cacul_PID();
    envoie_commande();    
    tension_condensateur = analogRead(A0);
    tension_condensateur = tension_condensateur/1023*5;
    if (tension_condensateur < 0.7){tension_condensateur = 0;}
    Serial.println(tension_condensateur);
    tempsDernierEnvoi = tempsCourant;
  }
}

void cacul_PID()
{
  
  /****** Moteur 1 ******/
  if (Setpoint_moteur1 == 0. ){
    ErreurPrecedante_moteur1 =0;
    Output_moteur1_A = 0;
    Output_moteur1 =0;    
  }
  
  if (Setpoint_moteur1 != 0 ){

  Erreur_moteur1 = Setpoint_moteur1 - Input_moteur1; 

  Outputf_moteur1 =0.533*(Output_moteur1_A) + 0.47*(ErreurPrecedante_moteur1);
 
  Output_moteur1 = Output_moteur1_A + Erreur_moteur1*Kp + ErreurPrecedante_moteur1*(Ki*TSDATA/1000-Kp); 

  Output_moteur1 = Output_moteur1 - Outputf_moteur1;  
 
  if (Output_moteur1 > 255){
    Output_moteur1 = 255;
  }
  if (Output_moteur1 < -255){
    Output_moteur1 = -255;
  }
  Output_moteur1_A = Output_moteur1;
  } 
  /**********************/

  /****** Moteur 2 ******/
  if (Setpoint_moteur2 == 0. ){
    ErreurPrecedante_moteur2 =0;
    Output_moteur2_A = 0;
    Output_moteur2 =0;    
  }
  
  if (Setpoint_moteur2 != 0 ){

  Erreur_moteur2 = Setpoint_moteur2 - Input_moteur2; 

  Outputf_moteur2 =0.533*(Output_moteur2_A) + 0.47*(ErreurPrecedante_moteur2);
 
  Output_moteur2 = Output_moteur2_A + Erreur_moteur2*Kp + ErreurPrecedante_moteur2*(Ki*TSDATA/1000-Kp); 

  Output_moteur2 = Output_moteur2 - Outputf_moteur2;  
 
  if (Output_moteur2 > 255){
    Output_moteur2 = 255;
  }
  if (Output_moteur2 < -255){
    Output_moteur2 = -255;
  }
  Output_moteur2_A = Output_moteur2;
  }
  /**********************/
  
  /****** Moteur 3 ******/
   if (Setpoint_moteur3 == 0. ){
    ErreurPrecedante_moteur3 =0;
    Output_moteur3_A = 0;
    Output_moteur3 =0;    
  }
  
  if (Setpoint_moteur3 != 0 ){

  Erreur_moteur3 = Setpoint_moteur3 - Input_moteur3; 

  Outputf_moteur3 =0.533*(Output_moteur3_A) + 0.47*(ErreurPrecedante_moteur3);
 
  Output_moteur3 = Output_moteur3_A + Erreur_moteur3*Kp + ErreurPrecedante_moteur3*(Ki*TSDATA/1000-Kp); 

  Output_moteur3 = Output_moteur3 - Outputf_moteur3;  
 
  if (Output_moteur3 > 255){
    Output_moteur3 = 255;
  }
  if (Output_moteur3 < -255){
    Output_moteur3 = -255;
  }
  Output_moteur3_A = Output_moteur3;
  }

  
  /**********************/

  /****** Moteur 4 ******/
  if (Setpoint_moteur4 == 0. ){
    ErreurPrecedante_moteur4 =0;
    Output_moteur4_A = 0;
    Output_moteur4 =0;    
  }
  
  if (Setpoint_moteur4 != 0 ){

  Erreur_moteur4 = Setpoint_moteur4 - Input_moteur4; 

  Outputf_moteur4 =0.533*(Output_moteur4_A) + 0.47*(ErreurPrecedante_moteur4);
 
  Output_moteur4 = Output_moteur4_A + Erreur_moteur4*Kp + ErreurPrecedante_moteur4*(Ki*TSDATA/1000-Kp); 

  Output_moteur4 = Output_moteur4 - Outputf_moteur4;  
  if (Output_moteur4 > 255){
    Output_moteur4 = 255;
  }
  if (Output_moteur4 < -255){
    Output_moteur4 = -255;
  }
  Output_moteur4_A = Output_moteur4;
  }
  /**********************/
}
void envoie_commande()
{ 
  /****** Moteur 1 ******/
  if (Output_moteur1 < 0){
    digitalWrite(commandePin1_moteur1, HIGH);
    digitalWrite(commandePin2_moteur1, LOW);
    analogWrite(commandePWM_moteur1, abs(Output_moteur1));
  }
  if (Output_moteur1 > 0){
    digitalWrite(commandePin1_moteur1, LOW);
    digitalWrite(commandePin2_moteur1, HIGH);
    analogWrite(commandePWM_moteur1, abs(Output_moteur1));
  }
  if (Output_moteur1 == 0.){
    digitalWrite(commandePin1_moteur1, HIGH);
    digitalWrite(commandePin2_moteur1, HIGH);
    analogWrite(commandePWM_moteur1, 0);
  }
  /**********************/
  
  /****** Moteur 2 ******/
  if (Output_moteur2 < 0){
    digitalWrite(commandePin1_moteur2, HIGH);
    digitalWrite(commandePin2_moteur2, LOW);
    analogWrite(commandePWM_moteur2, abs(Output_moteur2));
  }
  if (Output_moteur2 > 0){
    digitalWrite(commandePin1_moteur2, LOW);
    digitalWrite(commandePin2_moteur2, HIGH);
    analogWrite(commandePWM_moteur2, abs(Output_moteur2));
  }
  if (Output_moteur2 == 0.){
    digitalWrite(commandePin1_moteur2, HIGH);
    digitalWrite(commandePin2_moteur2, HIGH);
    analogWrite(commandePWM_moteur2, 0);
  }
  /**********************/
  
  /****** Moteur 3 ******/
  if (Output_moteur3 < 0){
    digitalWrite(commandePin1_moteur3, LOW);
    digitalWrite(commandePin2_moteur3, HIGH);
    analogWrite(commandePWM_moteur3, abs(Output_moteur3));
  }
  if (Output_moteur3 > 0){
    digitalWrite(commandePin1_moteur3, HIGH);
    digitalWrite(commandePin2_moteur3, LOW);
    analogWrite(commandePWM_moteur3, abs(Output_moteur3));
  }
  if (Output_moteur3 == 0.){
    digitalWrite(commandePin1_moteur3, HIGH);
    digitalWrite(commandePin2_moteur3, HIGH);
    analogWrite(commandePWM_moteur3, 0);
  }
  /**********************/
  
  /****** Moteur 4 ******/
  if (Output_moteur4 < 0){
    digitalWrite(commandePin1_moteur4, LOW);
    digitalWrite(commandePin2_moteur4, HIGH);
    analogWrite(commandePWM_moteur4, abs(Output_moteur4));
  }
  if (Output_moteur4 > 0){
    digitalWrite(commandePin1_moteur4, HIGH);
    digitalWrite(commandePin2_moteur4, LOW);
    analogWrite(commandePWM_moteur4, abs(Output_moteur4));
  }
  if (Output_moteur4 == 0.){
    digitalWrite(commandePin1_moteur4, HIGH);
    digitalWrite(commandePin2_moteur4, HIGH);
    analogWrite(commandePWM_moteur4, 0);
  }
  /**********************/
}

void GestionInterruption_moteur1()
{
  if (digitalReadFast(encodeurPinA_moteur1) == digitalReadFast(encodeurPinB_moteur1)) {
    ticksCodeur_moteur1++;
  }
  else {
    ticksCodeur_moteur1--;
  }
}
 
void GestionInterruption_moteur2()
{
  if (digitalReadFast(encodeurPinA_moteur2) == digitalReadFast(encodeurPinB_moteur2)) {
    ticksCodeur_moteur2++;
  }
  else {
    ticksCodeur_moteur2--;
  }
}

void GestionInterruption_moteur3()
{
  if (digitalReadFast(encodeurPinA_moteur3) == digitalReadFast(encodeurPinB_moteur3)) {
    ticksCodeur_moteur3--;
  }
  else {
    ticksCodeur_moteur3++;
  }
}

void GestionInterruption_moteur4()
{
  if (digitalReadFast(encodeurPinA_moteur4) == digitalReadFast(encodeurPinB_moteur4)) {
    ticksCodeur_moteur4--;
  }
  else {
    ticksCodeur_moteur4++;
  }
}
