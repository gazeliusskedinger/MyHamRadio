#include <SoftwareSerial.h>
#include <Wire.h>

#define MasterON 5

/***********************************************
 * 
 *  Global Variables for the receive String
 * 
 ***********************************************/
bool updateDis = 0;
String str;
char cstr[10];
bool AB = 0;

/***********************************************
 * 
 *  Frequency Variables, set by the Masters rotary 
 *  encoder  
 * 
 ***********************************************/

long radix = 100;
int  radixInt = 1;
int  oldRadixInt = radixInt;

volatile long int freq = 3560000; 
volatile long int oldFreq = freq;

//volatile long int frequencyA =   3650000ULL;
//volatile long int frequencyB =   3560000ULL;
//volatile long int oldFreqA =     frequencyA;
volatile long int bandEnd =     300000;
volatile long int bandStart =    30000;

/***********************************************
 * 
 *  Rotary encoder declarations
 * 
 ***********************************************/

static const int pushPin = 4;
static const int rotAPin = 3;
static const int rotBPin = 2;

/***********************************************
 * 
 *  Rotary encoder variables
 * 
 ***********************************************/

volatile int rotState = 0;
volatile int rotBValue = 1;
volatile int rotAValue = 1; 

/***********************************************
 * 
 *  Headers Master/Display/Controller
 * 
 ***********************************************/

 String dis = "DIS";
 String fre = "FRE";
 String mas = "MAS";
 String rix = "RIX"; 

/***********************************************
 * 
 *  Other declarations
 * 
 ***********************************************/


SoftwareSerial mySerial(6, 7);
/************************************************ 
 *  
 *  the setup routine runs once when you press 
 *  reset:
 *  
 ************************************************/
 
void setup(){

/***********************************************
 * 
 *  ROTARY ENCODER PIN SETUP
 * 
 ***********************************************/
  pinMode(pushPin,INPUT);
  pinMode(rotAPin,INPUT);
  pinMode(rotBPin,INPUT);

  digitalWrite(pushPin,HIGH);
  digitalWrite(rotAPin,HIGH);
  digitalWrite(rotBPin,HIGH);

  attachInterrupt(digitalPinToInterrupt(rotAPin),ISRrotAChange,CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotBPin),ISRrotBChange,CHANGE);

/***********************************************
 * 
 *  Headers Master/Display/Controller
 * 
 ***********************************************/
  Serial.begin(57600);
  mySerial.begin(57600);
  pinMode(MasterON,OUTPUT);
  digitalWrite(MasterON,HIGH);
  
  
  //delay(2000);
  while(!mySerial.available()){
   //Serial.println(val);
  }
}

/***********************************************
 * 
 *  LOOP
 * 
 ***********************************************/

void loop() {
//Serial.println(freq);
/***********************************************
 * 
 *  Headers Master/Display/Controller
 * 
 ***********************************************/

  if(freq != oldFreq){ 
    //Serial.println("Test");
    //if(updateDis == 1){
    //SendFrequency(bandInit);
    updateDisplay(freq);
    oldFreq = freq;   
    //Serial.println("check");
    //Serial.println(freq);
  }
  if(oldRadixInt != radixInt){
    updateRadix();
  }  
  if (mySerial.available()) {
    str = mySerial.readStringUntil('\r\n');


    
    //Serial.println(str.substring(0,3));
    
    // Specific Display Driver Code
      /*
      if(str.substring(0,3)== "DIS"){
        str = str.substring(3,10);
        str.toCharArray(cstr,10);
        Serial.println("DISPLAY");
        
        if(atol(cstr)){
          freq = atol(cstr);
          Serial.println(freq);  
        }
      }
      */
      // Controller specific code
      /*
      if(str.substring(0,3)== "FRE"){
        str = str.substring(3,10);
        str.toCharArray(cstr,10);
        Serial.println("SI5351");
        
        if(atol(cstr)){
          freq = atol(cstr);
          Serial.println(freq);  
        }
      }
      */
      // Master specific
      
      
/***********************************************
 * 
 *  Headers Master/Display/Controller
 * 
 ***********************************************/

      if(str.substring(0,3) == "MAS"){
        String message;
        //Serial.println("MASTER");
        if(str.substring(3,6) == "DIS"){
          message = dis+int(freq/100);
          mySerial.println(message);
        }
        if(str.substring(3,6) == "FRE"){
          message = fre+int(freq/100);
          mySerial.println(message);
        }
        
        /*if(atol(cstr)){
          freq = atol(cstr);
          Serial.println(freq);
          */  
        }
      }
  delay(1);        // delay in between reads for stability
}

void ISRrotBChange(){
  
  if(digitalRead(rotBPin)){
    rotBValue = 1;
    UpdateRot();
  } else{
    rotBValue = 0;
    UpdateRot();
  }
}

void ISRrotAChange(){
  
  if(digitalRead(rotAPin)){
    rotAValue = 1;
    //Serial.println("Achange");
    UpdateRot();
  } else{
    rotAValue = 0;
    UpdateRot();
  }
}


void UpdateRot(){
  switch(rotState){
    case 0:
      if(!rotBValue){
        
        rotState = 1; // CW 1
      }
      if(!rotAValue){
        //Serial.println("Bchange");
        rotState = 11; // CCW 1
      } 
    break;

    case 1:
      if(!rotBValue){
        if(!rotAValue){
          // either increment radioindex or freq
          if(digitalRead(pushPin) == LOW){
            updateDis = 1;
            if(radix == 1000000){
              radix = 100000;
              radixInt = 16;
            }
            else if(radix == 100000){
              radix = 10000;
              radixInt = 8;
            }
            else if(radix == 10000){
              radix = 1000;
              radixInt = 4;
            }
            else if(radix == 1000){
              radix = 100;
              radixInt = 2;
            }
            else if(radix == 100){
              radix = 10;
              radixInt = 1;
            }
            else{
              radix = 1000000;
              radixInt = 32;
            }
          }
          /*else if(digitalRead(btn1) == LOW && digitalRead(btn2) == HIGH && digitalRead(btn3) == HIGH && digitalRead(btn4) == HIGH){
            updateDisplay(frequencyA);
            brightness = brightness + 8;
            brightness = chkAnalog(brightness);
          }
          else if(digitalRead(btn1) == HIGH && digitalRead(btn2) == LOW && digitalRead(btn3) == HIGH && digitalRead(btn4) == HIGH){
            updateDisplay(frequencyA);
            contrast = contrast + 8;
            contrast = chkAnalog(contrast);
          }*/
          else{
            //Serial.println("Achange");
            freq = (freq + radix);
              if(freq > bandEnd){
                freq = bandEnd;
              }  
            /*if(!AB){
              frequencyA = (frequencyA + radix);
              if(frequencyA > bandEnd){
                frequencyA = bandEnd;
              }  
            }
            else{
              frequencyB = (frequencyB + radix);
              if(frequencyB > bandEnd){
                frequencyB = bandEnd;
              }  
            }*/
          }
          rotState = 2; //CW 2
        }
      }
      else if(rotAValue){ // glitch
        rotState = 0;
      }
      break;
    
    case 2: // CW Wait for bHigh
      if(rotBValue){
        rotState = 3; // CW 3
      }
      break;
    
    case 3:
      if(rotAValue){
        rotState = 0;
      }
      break;
      
    case 11:
      if(!rotAValue){
        if(!rotBValue){
          if(digitalRead(pushPin)== LOW){
            updateDis = 1;
            if(radix == 1000000){
              radix = 10;
              radixInt = 1;
            }
            else if(radix == 10){
              radix = 100;
              radixInt = 2;
            }
            else if(radix == 100){
              radix = 1000;
              radixInt = 4;
            }
            else if(radix == 1000){
              radix = 10000;
              radixInt = 8;
            }
            else if(radix == 10000){
              radix = 100000;
              radixInt = 16;
            }
            else{
              radix = 1000000;
              radixInt = 32;
            }
          }
          /*else if(digitalRead(btn1) == LOW && digitalRead(btn2) == HIGH && digitalRead(btn3) == HIGH && digitalRead(btn4) == HIGH){
            updateDisplay(frequencyA);
            brightness = brightness - 8;
            brightness = chkAnalog(brightness);
          }
          else if(digitalRead(btn1) == HIGH && digitalRead(btn2) == LOW && digitalRead(btn3) == HIGH && digitalRead(btn4) == HIGH){
            updateDisplay(frequencyA);
            contrast = contrast - 8;
            contrast = chkAnalog(contrast);
            
          }*/
          else{
            freq = (freq - radix);
              if(freq < bandStart){
                freq = bandStart;
              } 
           /* if(!AB){
              frequencyA = (frequencyA - radix);
              if(frequencyA < bandStart){
                frequencyA = bandStart;
              }  
            }
            else{
              frequencyB = (frequencyB - radix);
              if(frequencyB < bandStart){
                frequencyB = bandStart;
              }   
            }*/
          }
          rotState = 12; //CCW 2
        }
      }
      else if(rotBValue){
        rotState = 0;
      }
    break;
    case 12:
    if(rotAValue){
      rotState = 13;
    }
    break;
    case 13:
      if (rotBValue){
        rotState = 0;
      }
    break;
  }
}
/*
void UpdateRot(){
  switch(rotState){
    case 0:
      if(!rotBValue){
        rotState = 1; // CW 1
      }
      if(!rotAValue){
        rotState = 11; // CCW 1
      } 
    break;

    case 1:
      if(!rotBValue){
        if(!rotAValue){
          // either increment radioindex or freq
          if(center_button){
            updateDis = 1;
             radix = radix / 10;
            if(radix == 1){
              radix = 1000000;
            }
            center_button=0;
          }
          else{
            
              frequencyA = (frequencyA + radix);
              if(frequencyA > bandEnd){
                frequencyA = bandEnd;
              }  
          }
          rotState = 2; //CW 2
        }
      }
      else if(rotAValue){ // glitch
        rotState = 0;
      }
      break;
    
    case 2: // CW Wait for bHigh
      if(rotBValue){
        rotState = 3; // CW 3
      }
      break;
    
    case 3:
      if(rotAValue){
        rotState = 0;
      }
      break;
      
    case 11:
      if(!rotAValue){
        if(!rotBValue){
          if(center_button){
            updateDis = 1;
            radix = radix * 10;
            if(radix == 1000000){
              radix = 1;
            }
            center_button=0;
          }
          else{
           
              frequencyA = (frequencyA - radix);
              if(frequencyA < bandStart){
                frequencyA = bandStart;
              }  
          //}
          rotState = 12; //CCW 2
        }
        
      }
      else if(rotBValue){
        rotState = 0;
      }
    break;
    case 12:
    if(rotAValue){
      rotState = 13;
    }
    break;
    case 13:
      if (rotBValue){
        rotState = 0;
      }
    break;
  }
}
*/
void updateDisplay(long int frequency){
  long int sendFreq = long(frequency/100);
  String message = dis + sendFreq;
  mySerial.println(message);
  updateDis = 0;
}
void updateRadix(){
  String message = rix + radixInt;
  mySerial.println(message);
  updateDis = 0;
} 

/*
void SendFrequency(long int freq){
  //VFO
  si5351.set_freq(freq*400ULL, SI5351_CLK0);
  //EvenDivisor(freq);
  //delay(50);
  //si5351.set_phase(SI5351_CLK1, 0);
  //si5351.set_phase(SI5351_CLK2, Even_Divisor);
  //si5351.set_freq_manual(freq * SI5351_FREQ_MULT, Even_Divisor * freq * SI5351_FREQ_MULT, SI5351_CLK1);
  //si5351.set_freq_manual(freq * SI5351_FREQ_MULT, Even_Divisor * freq * SI5351_FREQ_MULT, SI5351_CLK2);
  //delay(50);
  //si5351.set_phase(SI5351_CLK1, 0);
  //si5351.set_phase(SI5351_CLK2, Even_Divisor);
}*/
