#include <Stepper.h>

const int stepFuelerRevolution = 70;  // change this to fit the number of steps per revolution

Stepper stepper_Temp(stepFuelerRevolution, 8, 7, 6, 5);
Stepper stepper_Fuel(stepFuelerRevolution, 12, 11, 10, 9);

String inputString = "";
bool stringComplete = false;

int cacheRotTemp = 0;
int cacheRotFuel = 0;
int RotTemp = 0;
int RotFuel = 0;

const int oilPressure = A0;
const int waterTemp = A1;
const int brakePressure = A2;
const int washerFluid = A3;
const int fuelWarning = A4;
const int batteryVoltage = 2;
const int engineBrake = 3;
const int parkingBrake = 4;

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


  stepper_Temp.setSpeed(100);
  stepper_Fuel.setSpeed(100);

  pinMode(oilPressure, OUTPUT);
  pinMode(waterTemp, OUTPUT);
  pinMode(brakePressure, OUTPUT);
  pinMode(washerFluid, OUTPUT);
  pinMode(fuelWarning, OUTPUT);
  pinMode(batteryVoltage, OUTPUT);
  pinMode(engineBrake, OUTPUT);
  pinMode(parkingBrake, OUTPUT);

  for (int i = 0; i < stepFuelerRevolution; i++) {
    stepper_Temp.step(1);
    stepper_Fuel.step(-1);
  }

  delay(500);

  for (int i = 0; i < stepFuelerRevolution; i++) {
    stepper_Temp.step(-1);
    stepper_Fuel.step(1);
  }

  digitalWrite(oilPressure, HIGH);
  delay(500);
  digitalWrite(oilPressure, LOW);
  digitalWrite(waterTemp, HIGH);
  delay(500);
  digitalWrite(waterTemp, LOW);
  digitalWrite(brakePressure, HIGH);
  delay(500);
  digitalWrite(brakePressure, LOW);
  digitalWrite(washerFluid, HIGH);
  delay(500);
  digitalWrite(washerFluid, LOW);
  digitalWrite(fuelWarning, HIGH);
  delay(500);
  digitalWrite(fuelWarning, LOW);
  digitalWrite(batteryVoltage, HIGH);
  delay(500);
  digitalWrite(batteryVoltage, LOW);
  digitalWrite(parkingBrake, HIGH);
  delay(500);
  digitalWrite(parkingBrake, LOW);
  digitalWrite(engineBrake, HIGH);
  delay(500);
  digitalWrite(engineBrake, LOW);
  delay(500);
  digitalWrite(oilPressure, HIGH);
  digitalWrite(waterTemp, HIGH);
  digitalWrite(brakePressure, HIGH);
  digitalWrite(washerFluid, HIGH);
  digitalWrite(fuelWarning, HIGH);
  digitalWrite(batteryVoltage, HIGH);
  digitalWrite(engineBrake, HIGH);
  digitalWrite(parkingBrake, HIGH);
  delay(1000);
  digitalWrite(oilPressure, LOW);
  digitalWrite(waterTemp, LOW);
  digitalWrite(brakePressure, LOW);
  digitalWrite(washerFluid, LOW);
  digitalWrite(fuelWarning, LOW);
  digitalWrite(batteryVoltage, LOW);
  digitalWrite(engineBrake, LOW);
  digitalWrite(parkingBrake, LOW);
  delay(500);

  Serial.begin(115200);

}


void loop()
{

  if (stringComplete) {

    RotTemp = decodeHex(inputString.substring(0, 2));
    RotFuel = decodeHex(inputString.substring(2, 4));

    stepper_Temp.step(RotTemp - cacheRotTemp);
    stepper_Fuel.step(-1 * (RotFuel - cacheRotFuel));

    cacheRotTemp = RotTemp;
    cacheRotFuel = RotFuel;

    if (inputString.charAt(5) == '1') {
      digitalWrite(oilPressure, HIGH);
    } else {
      digitalWrite(oilPressure, LOW);
    }

    if (inputString.charAt(7) == '1') {
      digitalWrite(waterTemp, HIGH);
    } else {
      digitalWrite(waterTemp, LOW);
    }

    if (inputString.charAt(9) == '1') {
      digitalWrite(brakePressure, HIGH);
    } else {
      digitalWrite(brakePressure, LOW);
    }

    if (inputString.charAt(11) == '1') {
      digitalWrite(washerFluid, HIGH);
    } else {
      digitalWrite(washerFluid, LOW);
    }

    if (inputString.charAt(13) == '1') {
      digitalWrite(fuelWarning, HIGH);
    } else {
      digitalWrite(fuelWarning, LOW);
    }

    if (inputString.charAt(15) == '1') {
      digitalWrite(batteryVoltage, HIGH);
    } else {
      digitalWrite(batteryVoltage, LOW);
    }

    if (inputString.charAt(17) == '1') {
      digitalWrite(engineBrake, HIGH);
    } else {
      digitalWrite(engineBrake, LOW);
    }

    if (inputString.charAt(19) == '1') {
      digitalWrite(parkingBrake, HIGH);
    } else {
      digitalWrite(parkingBrake, LOW);
    }

    inputString = "";
    stringComplete = false;
  }

}
