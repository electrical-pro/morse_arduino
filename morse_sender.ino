//Morse code generator. Serhii Bazavluk 2020
//YouTube: https://www.youtube.com/electricalpro
//Demonstration: https://www.youtube.com/watch?v=Mw76lK94tRw
//Supports letters digits and some symbols
//You can set speed via serial command SetUnit:50
//You can update message via serial as well

String morseArray[63] = {"-.-.--", "", "", "...-..-", "", ".-...", ".----.", "-.--.", "-.--.-", "", ".-.-.", "--..--", "-....-", ".-.-.-", "-..-.", "-----", ".----", "..---", "...--",
"....-", ".....", "-....", "--...", "---..", "----.", "---...", "-.-.-.", "", "-...-", "", "..--..", ".--.-.", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", 
"-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "", "", "", "", "..--.-"}; // letters, numbers, symbols


int ledPin = 2; // pin that used to send morse code.
bool isInverted = false; // build-in LED is inverted on esp8266 so it needs to be 'true' if you just blinking using build-in LED
int oneUnitDelay = 80; // it is dot time in ms. 250 is good to decode with smartphone camera (try 80 - 300, lower = faster speed)
String DefaultTextToSend = "Hello world, I'm Morse code beacon"; // text to send in a loop

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // pin as output
}

void loop() {
  String textToSend = DefaultTextToSend;
  textToSend.toUpperCase(); // all to uppercase
  for(int i = 0; i <= textToSend.length(); i++)
  {
    Serial.print(textToSend[i]); // print symbols we currently sending
    if(textToSend[i]>=33 && textToSend[i]<=95) // check for targeted ASCII range
    {
        String morseToSend = morseArray[textToSend[i]-33]; // use morse array to covert
        for(int x = 0; x <= morseToSend.length(); x++)
        {
          sendDashOrDot(morseToSend[x]);
        }
          delay(oneUnitDelay*3); //between letters delay.
    }
    if (textToSend[i]==32) // if space 
    {
      delay(oneUnitDelay*4); //  delay for space (adding oneUnitDelay*4 delay to get oneUnitDelay*7 in total)
    }
  serialHandle(); // handle serial, remove this if not needed
  }
delay(oneUnitDelay*28); // between messages delay
Serial.println(); // print on a new line
}

void sendDashOrDot(byte sendChar){  
  if (sendChar==0x2E) // if dot
  {
      digitalWrite (ledPin, !isInverted);
      delay(oneUnitDelay);
      digitalWrite (ledPin, isInverted);
      delay(oneUnitDelay);
  }
  if (sendChar==0x2D) // if dash
  {
      digitalWrite (ledPin, !isInverted);
      delay(oneUnitDelay*3);
      digitalWrite (ledPin, isInverted);
      delay(oneUnitDelay);
  }
}

void serialHandle(){ // remove this function if it is not needed
  // if you type text in serial console it will be transmitted, or you can update speed by sending SetUnit:50
  if (Serial.available() > 0) //check if new date have arraved
  {
    Serial.println(" ");
    Serial.println("=======================================================================");
    String SerData = Serial.readStringUntil('\n');
    if (SerData.substring(0, 8) == "SetUnit:") // if command
    {
        String subStr = SerData.substring(8, 12); // update speed, send ex. SetUnit:50
        oneUnitDelay = subStr.toInt();
        Serial.println("New one unit delay is " + String(oneUnitDelay) + " ms");
    }
    else // if not command then it must be a new message
    {
        DefaultTextToSend = SerData; // update text
        Serial.println("Got new message:");
        Serial.println(DefaultTextToSend);
        Serial.println("*** New message will be transmitted after old message is finished ***"); 
    }
    Serial.println("=======================================================================");
  }
}
              
