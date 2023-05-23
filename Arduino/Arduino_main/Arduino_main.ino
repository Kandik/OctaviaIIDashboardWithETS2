#include <Stepper.h>

const int stepsPerRevolution = 147;

Stepper stepper_rpm(stepsPerRevolution, 5, 6, 7, 8);
Stepper stepper_tacho(stepsPerRevolution, 9, 10, 11, 12);

String inputString = "";
bool stringComplete = false;

int cacheRotRPM = 0;
int cacheRotSP = 0;
int RotRPM = 0;
int RotSP = 0;

const int cargoDamaged = A0;
const int rightBlinker = A1;
const int fogLights = A2;
const int Lights = A3;
const int beamLights = A4;
const int leftBlinker = A5;
const int checkEngine = 2;
const int adBlue = 3;
const int cruiseControl = 4;


int decodeHex(String s) {
  int x = 0;

  x += decodeHexCharacter(s.charAt(0)) * 16;
  x += decodeHexCharacter(s.charAt(1));

  return x;
}

int decodeHexCharacter(char c) {
  int x = 0;

  if (c == '1') {
    x = 1;
  }
  else if (c == '2') {
    x = 2;
  }
  else if (c == '3') {
    x = 3;
  }
  else if (c == '4') {
    x = 4;
  }
  else if (c == '5') {
    x = 5;
  }
  else if (c == '6') {
    x = 6;
  }
  else if (c == '7') {
    x = 7;
  }
  else if (c == '8') {
    x = 8;
  }
  else if (c == '9') {
    x = 9;
  }
  else if (c == 'a') {
    x = 10;
  }
  else if (c == 'b') {
    x = 11;
  }
  else if (c == 'c') {
    x = 12;
  }
  else if (c == 'd') {
    x = 13;
  }
  else if (c == 'e') {
    x = 14;
  }
  else if (c == 'f') {
    x = 15;
  }

  return x;
}



void serialEvent() {
  while (Serial.available()) {

    char inChar = (char)Serial.read();

    if (inChar == 'Z') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
}

void setup() {


  stepper_rpm.setSpeed(100);
  stepper_tacho.setSpeed(100);

  pinMode(cargoDamaged, OUTPUT);
  pinMode(rightBlinker, OUTPUT);
  pinMode(fogLights, OUTPUT);
  pinMode(Lights, OUTPUT);
  pinMode(beamLights, OUTPUT);
  pinMode(leftBlinker, OUTPUT);
  pinMode(checkEngine, OUTPUT);
  pinMode(adBlue, OUTPUT);
  pinMode(cruiseControl, OUTPUT);

  for (int i = 0; i < stepsPerRevolution; i++) {
    stepper_rpm.step(1);
    stepper_tacho.step(1);
  }

  delay(500);

  for (int i = 0; i < stepsPerRevolution; i++) {
    stepper_rpm.step(-1);
    stepper_tacho.step(-1);
  }

  digitalWrite(rightBlinker, HIGH);
  delay(500);
  digitalWrite(rightBlinker, LOW);
  digitalWrite(fogLights, HIGH);
  delay(500);
  digitalWrite(fogLights, LOW);
  digitalWrite(Lights, HIGH);
  delay(500);
  digitalWrite(Lights, LOW);
  digitalWrite(beamLights, HIGH);
  delay(500);
  digitalWrite(beamLights, LOW);
  digitalWrite(leftBlinker, HIGH);
  delay(500);
  digitalWrite(leftBlinker, LOW);
  digitalWrite(checkEngine, HIGH);
  delay(500);
  digitalWrite(checkEngine, LOW);
  digitalWrite(cargoDamaged, HIGH);
  delay(500);
  digitalWrite(cargoDamaged, LOW);
  digitalWrite(adBlue, HIGH);
  delay(500);
  digitalWrite(adBlue, LOW);
  digitalWrite(cruiseControl, HIGH);
  delay(500);
  digitalWrite(adBlue, HIGH);
  digitalWrite(cargoDamaged, HIGH);
  digitalWrite(checkEngine, HIGH);
  digitalWrite(leftBlinker, HIGH);
  digitalWrite(beamLights, HIGH);
  digitalWrite(Lights, HIGH);
  digitalWrite(fogLights, HIGH);
  digitalWrite(rightBlinker, HIGH);
  delay(1000);
  digitalWrite(adBlue, LOW);
  digitalWrite(cruiseControl, LOW);
  digitalWrite(cargoDamaged, LOW);
  digitalWrite(checkEngine, LOW);
  digitalWrite(leftBlinker, LOW);
  digitalWrite(beamLights, LOW);
  digitalWrite(Lights, LOW);
  digitalWrite(fogLights, LOW);
  digitalWrite(rightBlinker, LOW);
  delay(500);

  Serial.begin(115200);

}


void loop()
{

  if (stringComplete) {

    RotRPM = decodeHex(inputString.substring(0, 2));
    RotSP = decodeHex(inputString.substring(2, 4));

    stepper_rpm.step(RotRPM - cacheRotRPM);
    stepper_tacho.step(RotSP - cacheRotSP);

    cacheRotRPM = RotRPM;
    cacheRotSP = RotSP;

    if (inputString.charAt(5) == '1') {
      digitalWrite(cargoDamaged, HIGH);
    } else {
      digitalWrite(cargoDamaged, LOW);
    }

    if (inputString.charAt(7) == '1') {
      digitalWrite(rightBlinker, HIGH);
    } else {
      digitalWrite(rightBlinker, LOW);
    }

    if (inputString.charAt(9) == '1') {
      digitalWrite(fogLights, HIGH);
    } else {
      digitalWrite(fogLights, LOW);
    }

    if (inputString.charAt(11) == '1') {
      digitalWrite(Lights, HIGH);
    } else {
      digitalWrite(Lights, LOW);
    }

    if (inputString.charAt(13) == '1') {
      digitalWrite(beamLights, HIGH);
    } else {
      digitalWrite(beamLights, LOW);
    }

    if (inputString.charAt(15) == '1') {
      digitalWrite(leftBlinker, HIGH);
    } else {
      digitalWrite(leftBlinker, LOW);
    }

    if (inputString.charAt(17) == '1') {
      digitalWrite(checkEngine, HIGH);
    } else {
      digitalWrite(checkEngine, LOW);
    }

    if (inputString.charAt(19) == '1') {
      digitalWrite(adBlue, HIGH);
    } else {
      digitalWrite(adBlue, LOW);
    }

    if (inputString.charAt(21) == '1') {
      digitalWrite(cruiseControl, HIGH);
    } else {
      digitalWrite(cruiseControl, LOW);
    }

    inputString = "";
    stringComplete = false;
  }

}
