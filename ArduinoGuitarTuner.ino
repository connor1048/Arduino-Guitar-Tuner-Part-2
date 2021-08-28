
/*
 File/Sketch Name: ArduinoGuitarTuner

 Created: 26, AUGUST 2021
 
 Author: Connor Martin
 
 Description:  The goal of this program is to tune a guitar. 
               This code is to be paired with the schemtaic that can be found on my website: 
               This code takes an input signal from am electric or acoustic guitar string via the arduino sound sensor and processes the 
               the frequency via foureir analysis to determine the frequency of the signal (guitar string). Once the frequency of the signal from the guitar string is determined, 
               the LEDs will direct whether to tune the guitar string up or down to acheive accurate standard tuning for the guitar. 

*/
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "arduinoFFT.h"


LiquidCrystal lcd(44, 45, 46, 47, 48, 49);

const byte keypad_rows = 4; //four rows
const byte keypad_cols = 4; //three columns
char keypad_keys[keypad_rows][keypad_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

int way = 0; //help with string selection routine 

byte keypad_rowPins[keypad_rows] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte keypad_colPins[keypad_cols] = {26, 27, 28, 29}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keypad_keys), keypad_rowPins, keypad_colPins, keypad_rows, keypad_cols );

char keypad_current_key;
char key;

 
#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values
int i = 0;

void setup() 
{
    Serial.begin(115200); //Baud rate for the Serial Monitor
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
    pinMode(30, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(37, OUTPUT);
    pinMode(38, OUTPUT);
    
}
 
void loop() 

{   
  //The following lines make sure the LEDs turn off once they indicate tuning 
  digitalWrite(30, LOW);
  digitalWrite(31, LOW);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);
  digitalWrite(38, LOW);
  
  if(way == 0){
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Choose String");
       key = keypad.waitForKey();
       way = 1;
       lcd.clear();
       lcd.setCursor(0,0);
       
       if(key == '1'){
       lcd.print("Tune Low E");
       }
       if(key == '2'){
       lcd.print("Tune A");
       }
       if(key == '3'){
       lcd.print("Tune D");
       }
       if(key == 'A'){
       lcd.print("Tune G");
       }
       if(key == '4'){
       lcd.print("Tune B");
       }
       if(key == '5'){
       lcd.print("Tune High e");
       }
     
   }
 
    // takes SAMPLES anount of samples
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
     
        vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
        vImag[i] = 0; //Makes imaginary term 0 always

        //remaining wait time between samples if necessary
        while(micros() < (microSeconds + samplingPeriod))
        {
          //do nothing
        }
    }
    
    //Perform FFT on samples 
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    //Find peak frequency
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);


   /*The following set of if statments takes the peak we have recorded from our sound sensor and 
     analyzes it to determine whether or not the chosen string needs to be tuned and how the string 
     needs to be tuned. */


   //LOW E STRING
   if(key == '1'){
    //lcd.setCursor(0,0);
    //lcd.print("Tune Low E");

    
   for(int i = 1; i <= 3; i++){
      if(peak >= 69/i && peak<= 97/i)
   {
      peak = peak*i;
    }}
    
   for(int i = 1; i <= 12; i++){
      if(peak >= 69*i && peak<= 97*i)
   {
      peak = peak/i;
    }}
    
   

    if(peak >= 69 && peak <= 73){ 
      digitalWrite(30, HIGH);
    }  
    else if(peak >= 73 && peak <= 77){ 
      digitalWrite(31, HIGH);
    }
    else if(peak >= 77 && peak <= 80){ 
      digitalWrite(32, HIGH);
    }
    else if(peak >= 80 && peak <= 82){
      digitalWrite(33, HIGH);
    }
    else if(peak >= 82 && peak <= 84){
      digitalWrite(34, HIGH);
    }
    else if(peak >= 84 && peak <= 86){
      digitalWrite(35, HIGH);
    }
    else if(peak >= 86 && peak <= 89){
      digitalWrite(36, HIGH);
    }
    else if(peak >= 89 && peak <=93 ){
      digitalWrite(37, HIGH);
    }
    else if(peak >= 93 && peak <= 97){
      digitalWrite(38, HIGH);
    }
    }

   // A STRING
   if(key == '2'){
   // lcd.setCursor(0,0);
   // lcd.print("Tune A");

       for(int i = 1; i <= 3; i++){
      if(peak >= 100/i && peak<= 124.5/i)
   {
      peak = peak*i;
    }}
    
   for(int i = 1; i <= 10; i++){
      if(peak >= 100*i && peak<= 124.5*i)
   {
      peak = peak/i;
    }}

    if(peak >= 100 && peak <= 104){ 
       digitalWrite(30, HIGH);
    }  
    else if(peak >= 104 && peak <= 107){ 
       digitalWrite(31, HIGH);
    }
    else if(peak >= 107 && peak <= 109.5){ 
       digitalWrite(32, HIGH);
    }
    else if(peak >= 109.5 && peak < 111.5){
       digitalWrite(33, HIGH);
    }
    else if(peak >= 111.5 && peak <= 113){
       digitalWrite(34, HIGH);
    }
    else if(peak > 113 && peak <= 115){
       digitalWrite(35, HIGH);
    }
    else if(peak >= 115 && peak <= 117.5){
       digitalWrite(36, HIGH);
    }
    else if(peak >= 117.5 && peak <= 120.5){
       digitalWrite(37, HIGH);
    }
    else if(peak >= 120.5 && peak <= 124.5){
       digitalWrite(38, HIGH);
    }
    }

     // D STRING
    if(key == '3'){
     // lcd.setCursor(0,0);
     // lcd.print("Tune D");

    for(int i = 1; i <= 3; i++){
      if(peak >= 129.5/i && peak<= 165/i)
      {
         peak = peak*i;
     }}
    
     for(int i = 1; i <= 8; i++){
      if(peak >= 129.5*i && peak<= 165*i)
      {
      peak = peak/i;
      }}
    
    if(peak >= 129.5 && peak <= 133.5){ 
       digitalWrite(30, HIGH);
    }  
    else if(peak >= 133.5 && peak <= 137.5){ 
       digitalWrite(31, HIGH);
    }
    else if(peak >= 141.5 && peak <= 144.5){ 
       digitalWrite(32, HIGH);
    }
    else if(peak >= 144.5 && peak <= 147.5){
       digitalWrite(33, HIGH);
    }
    else if(peak >= 147.5 && peak <= 150){
       digitalWrite(34, HIGH);
    }
    else if(peak >= 150 && peak <= 153){
       digitalWrite(35, HIGH);
    }
    else if(peak >= 153 && peak <= 157){
       digitalWrite(36, HIGH);
    }
    else if(peak >= 157 && peak <= 161){
       digitalWrite(37, HIGH);
    }
    else if(peak >= 161 && peak <= 165){
       digitalWrite(38, HIGH);
    }
    }

    // G STRING
    if(key == 'A'){
   //   lcd.setCursor(0,0);
    //  lcd.print("Tune G");

      for(int i = 1; i <= 4; i++){
      if(peak >= 182.5/i && peak<= 215/i)
      {
         peak = peak*i;
     }}
    
     for(int i = 1; i <= 7; i++){
      if(peak >= 182.5*i && peak<= 215*i)
      {
      peak = peak/i;
      }}

    if(peak >= 182.5 && peak <= 186.5){ 
       digitalWrite(30, HIGH);
    }  
    else if(peak >= 186.5 && peak <= 190.5){ 
       digitalWrite(31, HIGH);
    }
    else if(peak >= 190.5 && peak <= 194.5){ 
       digitalWrite(32, HIGH);
    }
    else if(peak >= 195.5 && peak <= 198.5){
       digitalWrite(33, HIGH);
    }
    else if(peak >= 198.5 && peak <= 201){
       digitalWrite(34, HIGH);
    }
    else if(peak >= 201 && peak <= 204){
       digitalWrite(35, HIGH);
    }
    else if(peak >= 203 && peak <= 207){
       digitalWrite(36, HIGH);
    }
    else if(peak >= 207 && peak <= 211){
       digitalWrite(37, HIGH);
    }
    else if(peak >= 211 && peak <= 215){
       digitalWrite(38, HIGH);
    }
    }

    // B STRING
       if(key == '4'){
      //  lcd.setCursor(0,0);
      //  lcd.print("Tune B");

    for(int i = 1; i <= 5; i++){
      if(peak >= 230/i && peak<= 276/i)
      {
         peak = peak*i;
     }}
    
     for(int i = 1; i <= 6; i++){
      if(peak >= 230*i && peak<= 276*i)
      {
      peak = peak/i;
      }}

    if(peak >= 230 && peak <= 237){ 
       digitalWrite(30, HIGH);
    }  
    else if(peak >= 237 && peak <= 243){ 
       digitalWrite(31, HIGH);
    }
    else if(peak >= 243 && peak <= 247){ 
       digitalWrite(32, HIGH);
    }
    else if(peak >= 247 && peak <= 251){
       digitalWrite(33, HIGH);
    }
    else if(peak >= 251 && peak <= 255){
       digitalWrite(34, HIGH);
    }
    else if(peak >= 255 && peak <= 259){
       digitalWrite(35, HIGH);
    }
    else if(peak >= 259 && peak <= 263){
       digitalWrite(36, HIGH);
    }
    else if(peak >= 263 && peak <= 269){
       digitalWrite(30, HIGH);
    }
    else if(peak >= 269 && peak <= 276){
       digitalWrite(38, HIGH); 
    }
    }

     //HIGH e STRING
       if(key == '5'){
       // lcd.setCursor(0,0);
       //  lcd.print("Tune High e");

    for(int i = 1; i <= 6; i++){
      if(peak >= 311/i && peak<= 365/i)
      {
         peak = peak*i;
     }}
    
     for(int i = 1; i <= 6; i++){
      if(peak >= 311*i && peak<= 365*i)
      {
      peak = peak/i;
      }}

    if(peak >= 311 && peak <= 318){ 
       digitalWrite(30, HIGH);
    }  
    else if(peak >= 318 && peak <= 324){ 
       digitalWrite(31, HIGH);
    }
    else if(peak >= 324 && peak <= 330){ 
       digitalWrite(32, HIGH);
    }
    else if(peak >= 330 && peak <= 336){
       digitalWrite(33, HIGH);
    }
    else if(peak >= 336 && peak <= 339){
       digitalWrite(34, HIGH);
    }
    else if(peak >= 339 && peak <= 345){
       digitalWrite(35, HIGH);
    }
    else if(peak >= 345 && peak <= 351){
       digitalWrite(36, HIGH);
    }
    else if(peak >= 351 && peak <= 358){
       digitalWrite(37, HIGH);
    }
    else if(peak >= 358 && peak <= 365){
       digitalWrite(38, HIGH);
    }
    }

    delay(30);
    if(keypad.getKey()){
      way = 0;
    }
}
