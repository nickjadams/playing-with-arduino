/*
  SparkFun Electronics 2010
  Playing with IR remote control
  
  IR Receiver TSOP382: Supply voltage of 2.5V to 5.5V
  With the curved front facing you, pin 1 is on the left.
  Attach
    Pin 1: To pin 2 on Arduino
    Pin 2: GND
    Pin 3: 5V
  
  This is based on pmalmsten's code found on the Arduino forum from 2007:
  http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1176098434/0

  This code works with super cheapo remotes. If you want to look at the individual timing
  of the bits, use this code:
  http://www.arduino.cc/playground/Code/InfraredReceivers
  
  This code clips a lot of the incoming IR blips, but what is left is identifiable as key codes.

*/

int irPin = 2; //Sensor pin 1 wired to Arduino's pin 2
int statLED = 13; //Toggle the status LED every time Power is pressed
int start_bit = 2652; //Start bit threshold (Microseconds)
int bin_1 = 1000; //Binary 1 threshold (Microseconds)
int bin_0 = 400; //Binary 0 threshold (Microseconds)

void setup() {
  pinMode(statLED, OUTPUT);
  digitalWrite(statLED, LOW);

  pinMode(irPin, INPUT);

  Serial.begin(9600);
  Serial.println("Waiting: ");
}

void loop() {
  int key = getIRKey();		    //Fetch the key
  
  if(key != 0) //Ignore keys that are zero
  {
    Serial.print("Key Recieved: ");
    switch(key)
    {
      case 128: Serial.print("CH 1"); break;
      case 129: Serial.print("CH 2"); break;
      case 130: Serial.print("CH 3"); break;
      case 131: Serial.print("CH 4"); break;
      case 132: Serial.print("CH 5"); break;
      case 133: Serial.print("CH 6"); break;
      case 134: Serial.print("CH 7"); break;
      case 135: Serial.print("CH 8"); break;
      case 136: Serial.print("CH 9"); break;
      case 137: Serial.print("CH 0"); break;
      case 191: Serial.print("TXT"); break;
      case 236: Serial.print("ESC"); break;
      case 157: Serial.print("Guide"); break;
      case 186: Serial.print("SLow"); break;
      case 252: Serial.print("Menu"); break;
      case 171: Serial.print("EPG"); break;
      case 245: Serial.print("left"); break;
      case 244: Serial.print("right"); break;
      case 142: Serial.print("Fav"); break;
      case 184: Serial.print("i"); break;
      case 182: Serial.print("Help"); break;
      case 460: Serial.print("Red"); break;
      case 461: Serial.print("Green"); break;
      case 462: Serial.print("Yellow"); break;
      case 463: Serial.print("Blue"); break;
      case 456: Serial.print("Pause"); break;
      case 458: Serial.print("Stop"); break;
      case 144: Serial.print("CH Up"); break;
      case 145: Serial.print("CH Down"); break;
      case 146: Serial.print("VOL Up"); break;
      case 147: Serial.print("VOL Down"); break;
      case 148: Serial.print("Mute"); break;
      case 165: Serial.print("AV/TV"); break;
      case 149: 
        Serial.print("Power");
        if(digitalRead(statLED) != 1) //This toggles the statLED every time power button is hit
          digitalWrite(statLED, HIGH);
        else
          digitalWrite(statLED, LOW);
        break;

      default: Serial.print(key);
    }

    Serial.println();
    delay(100);
  }
}

int getIRKey() {
  int j=12;
  int data[j + 1];
  int i;

  while(pulseIn(irPin, LOW) < start_bit); //Wait for a start bit
  
  for(i = 0 ; i < j ; i++)
    data[i] = pulseIn(irPin, LOW); //Start measuring bits, I only want low pulses
  
  for(i = 0 ; i < j ; i++) //Parse them
  {	    
    if(data[i] > bin_1) //is it a 1?
      data[i] = 1;
    else if(data[i] > bin_0) //is it a 0?
      data[i] = 0;
    else
      return -1; //Flag the data as invalid; I don't know what it is! Return -1 on invalid data
  }

  int result = 0;
  for(i = 0 ; i < j ; i++) //Convert data bits to integer
    if(data[i] == 1) result |= (1<<i);

  return result; //Return key number
} 


