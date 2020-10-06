/***********************************************
 * 
 *  Definitions of Cathode pins
 * 
 ***********************************************/

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define G 8
#define DP 9

/***********************************************
 * 
 *  Definitions of Andoe pins
 * 
 ***********************************************/

#define ANODE1 10
#define ANODE2 11
#define ANODE3 12
#define ANODE4 13
#define ANODE5 A0
#define ANODE6 A1

/***********************************************
 * 
 *  Definition Timing 
 *  NOLI SI TANGERE
 ***********************************************/

#define VIEW_TIME 2

/***********************************************
 * 
 *  OTHER DEFINITiONS 
 * 
 ***********************************************/

#define masterOn A2

/***********************************************
 * 
 *  Definition BYTES for Numbers 
 * 
 ***********************************************/

byte one =   B00001100;
byte two =   B01011011; 
byte three = B01011110;
byte four =  B01101100;
byte five =  B01110110;
byte six =   B01110111;
byte seven = B00011100;
byte eight = B01111111;
byte nine =  B01111110;
byte zero =  B00111111;
byte DPs =   B10000000;

/***********************************************
 * 
 *  Definition Global Frequency Variables
 * 
 ***********************************************/

long oldFreq;
long freq;  
long ackFreq;
int radix = 0x02;

String str;
char cstr[10];

byte anodes = B00000000; // if led display should be of or on
//bool DPs= false;
/***********************************************
 * 
 *  Definition segs numbers
 * 
 ***********************************************/

int hunderedthousands;// = int((freq/100000)%10);
int tenthousends;// = int((freq/10000)%10);
int thousands;// = int((freq/1000)%10);  
int hundreds;// = int((freq/100)%10);
int tens;// = int((freq/10)%10);
int ones;// = int((freq/1)%10);

/***********************************************
 * 
 *  SETUP
 * 
 ***********************************************/
void setup() {
  
/***********************************************
 * 
 *   INITIALIZING 
 * 
 ***********************************************/

 Serial.begin(57600);
 /***********************************************
 * 
 *  Display 
 * 
 ***********************************************/
 
  
/***********************************************
 * 
 *  CATHODE PINS 
 * 
 ***********************************************/
 
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  
  /***********************************************
   * 
   *  ANODE PINS 
   * 
   ***********************************************/
   
  pinMode(ANODE1, OUTPUT);
  pinMode(ANODE2, OUTPUT);
  pinMode(ANODE3, OUTPUT);
  pinMode(ANODE4, OUTPUT);
  pinMode(ANODE5, OUTPUT);
  pinMode(ANODE6, OUTPUT);
  
  
  /***********************************************
   * 
   *  OTHER PINS 
   * 
   ***********************************************/
  pinMode(masterOn, INPUT);

  /***********************************************
   * 
   *  INITIATION CERMONY FROM MASTER 
   * 
   ***********************************************/
   
  freq = 123456;
  int val = analogRead(masterOn);
  
  while(val < 127){
  val = analogRead(masterOn);
  
  //Serial.println("cant find master");
  }
  delay(500);
  Serial.println("MASDIS");
  while(!Serial.available()){
     //Serial.println(val);
  }
  str = Serial.readStringUntil('\r\n');
  if(str.substring(0,3)== "DIS"){
    str = str.substring(3,10);
    str.toCharArray(cstr,10);
    //Serial.println("DISPLAY");
    if(atol(cstr)){
      freq = atol(cstr);
      //Serial.println(freq);  
      calcSevenSeg(freq);
      sevenSeg(freq);
    }
  }  
}

  /***********************************************
   * 
   *  SERIEL
   * 
   ***********************************************/

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available()) {
    str = Serial.readStringUntil('\r\n');
    if(str.substring(0,3)== "DIS"){
      str = str.substring(3,10);
      str.toCharArray(cstr,10);
      //Serial.println("DISPLAY");
      
      if(atol(cstr)){
        freq = atol(cstr);
        Serial.println(freq);  
      }
    }
    if(str.substring(0,3)== "RIX"){
      str = str.substring(3,10);
      str.toCharArray(cstr,10);
      //Serial.println("DISPLAY");
      if(atol(cstr)){
        radix = atol(cstr);
        //Serial.println(freq);  
        //calcSevenSeg(freq);
        //sevenSeg(freq);
      }
    }
    if(oldFreq != freq){
      calcSevenSeg(freq);
      oldFreq = freq;
    }
  }
  sevenSeg(freq);
}
/***********************************************
 * 
 *  Defines what display to light up 
 * 
 ***********************************************/

void pickDigit(int digit){
  digitalWrite(ANODE1,1);
  digitalWrite(ANODE2,1);
  digitalWrite(ANODE3,1);
  digitalWrite(ANODE4,1);
  digitalWrite(ANODE5,1);
  digitalWrite(ANODE6,1);
  
  switch(digit){
    case 1: digitalWrite(ANODE1,(anodes & 0x01)); break;
    case 2: digitalWrite(ANODE3,(anodes & 0x02)); break;
    case 3: digitalWrite(ANODE4,(anodes & 0x04)); break;
    case 4: digitalWrite(ANODE5,(anodes & 0x08)); break;
    case 5: digitalWrite(ANODE6,(anodes & 0x10)); break;
    //case 6: digitalWrite(ANODE7,0); break;
    default: digitalWrite(ANODE2,(anodes & 0x20)); break;
  }  
}
/***********************************************
 * 
 *  The calculations needed every time the 
 *  frequency changes
 * 
 ***********************************************/
void calcSevenSeg(long freq){

  /***********************************************
 * 
 *  Checks for what aode to turn on or off
 * 
 ***********************************************/
  if(freq <= 100000){
    anodes = B00010000;  
  }
  else{
    anodes = B00000000;  
  }

  /***********************************************
 * 
 *  Definition Divides long to an integer per 
 *  segment
 * 
 ***********************************************/
 
  hunderedthousands = int((freq/100000)%10);
  tenthousends = int((freq/10000)%10);
  thousands = int((freq/1000)%10);  
  hundreds = int((freq/100)%10);
  tens = int((freq/10)%10);
  ones = int((freq/1)%10);
  
}

/***********************************************
 * 
 *  Main print function
 * 
 ***********************************************/
 
void sevenSeg(long freq){
  
//int millions = int((freq/1000000)%10);
/*int hunderedthousands = int((freq/100000)%10);
int tenthousends = int((freq/10000)%10);
int thousands = int((freq/1000)%10);  
int hundreds = int((freq/100)%10);
int tens = int((freq/10)%10);
int ones = int((freq/1)%10);
  */ 
   pickDigit(0);
   choose_digit(tens,(0x02 & DPs));
   delay(VIEW_TIME);
   pickDigit(1);
   choose_digit(ones,(0x01 & DPs));
   delay(VIEW_TIME);
   pickDigit(2);
   choose_digit(hundreds,(0x04 & DPs));
   delay(VIEW_TIME);
    pickDigit(3);
   choose_digit(thousands,(0x08 & DPs));
   delay(VIEW_TIME);
   pickDigit(4);
   choose_digit(tenthousends,(0x10 & DPs));
   delay(VIEW_TIME);
   pickDigit(5);
   choose_digit(hunderedthousands,(0x20 & DPs));
   delay(VIEW_TIME);
   //pickDigit(6);
   //choose_digit(millions);
   //delay(3);
   
   //TODO DP -> Radix 
}

/***********************************************
 * 
 *  Definition What Digit Byte to pick
 * 
 ***********************************************/

void choose_digit(int num, bool DPs){
  switch(num){
    default:
      printDigit(B00000000,false); break;
    case 0:
      printDigit(zero,DPs);break;
    case 1:
      printDigit(one,DPs);break;     
    case 2:
      printDigit(two,DPs);break;
    case 3:
      printDigit(three,DPs);break;
    case 4:
      printDigit(four,DPs);break;
    case 5:
      printDigit(five,DPs);break;
    case 6:
      printDigit(six,DPs);break;
    case 7:
      printDigit(seven,DPs);break;
    case 8:
      printDigit(eight,DPs);break;
    case 9:
      printDigit(nine,DPs);break;
  }
}

/***********************************************
 * 
 *  Divides the number bytes to segments 
 * 
 ***********************************************/

void printDigit(byte digit,bool DPs){
  digitalWrite(A,(digit & 0x01));
  digitalWrite(B,(digit & 0x02));
  digitalWrite(C,(digit & 0x04));
  digitalWrite(D,(digit & 0x08));
  digitalWrite(E,(digit & 0x10));
  digitalWrite(F,(digit & 0x20));
  digitalWrite(G,(digit & 0x40));
  digitalWrite(DP,DPs);
}
