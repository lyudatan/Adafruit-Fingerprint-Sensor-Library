#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


// pin definition for the Uno
#define cs   9
#define dc   8
#define rst  7

int getFingerprintIDez();

// pin #2 is IN from sensor
// pin #3 is OUT from arduino
SoftwareSerial mySerial(2, 3);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);



void setup() {
Serial.begin(9600);
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(1);

  TFTscreen.text("fingertest", 0, 0);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    TFTscreen.text("Found fingerprint sensor!", 0, 15);
    
  } else {
    TFTscreen.text("Did not find fingerprint sensor :(", 0, 15);
    
    while (1);
  }
  TFTscreen.text("Waiting for valid finger...", 0, 25);
  
  
}

void loop() {

  
getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  
  
  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
    TFTscreen.text("Image taken", 0, 40);
      break;
    case FINGERPRINT_NOFINGER:
    TFTscreen.text("No finger detected", 0, 40);   
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    TFTscreen.text("Communication error", 0, 40);
      return p;
    case FINGERPRINT_IMAGEFAIL:
    TFTscreen.text("Imaging error", 0, 40);
      return p;
    default:
    TFTscreen.text("Unknown error", 0, 40);
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
    TFTscreen.text("Image converted", 0, 55);
     
      break;
    case FINGERPRINT_IMAGEMESS:
    TFTscreen.text("Image too messy", 0, 55);
   
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    TFTscreen.text("Communication error", 0, 55);
      
      return p;
    case FINGERPRINT_FEATUREFAIL:
    TFTscreen.text("Could not find fingerprint features", 0, 55);
      
      return p;
    case FINGERPRINT_INVALIDIMAGE:
    TFTscreen.text("Could not find fingerprint features", 0, 55);
     
      return p;
    default:
    TFTscreen.text("Unknown error", 0, 55);
  
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    TFTscreen.text("Found a print match!", 0, 70);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    TFTscreen.text("Communication error", 0, 70);

    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    TFTscreen.text("Did not find a match", 0, 70);

    return p;
  } else {
    TFTscreen.text("Unknown error", 0, 70);

    return p;
  }   
  
  // found a match!
  // clear the screen with a black background
  TFTscreen.stroke(0,255,0);
  TFTscreen.setTextSize(3);
  TFTscreen.background(0, 0, 0);
  TFTscreen.text("UNLOCKED", 10, 50); 


 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  // clear the screen with a black background
  TFTscreen.stroke(0,255,0);
  TFTscreen.setTextSize(3);
  TFTscreen.background(0, 0, 0);
  TFTscreen.text("UNLOCKED", 10, 50);  
  return finger.fingerID; 
}

