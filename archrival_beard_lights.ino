// Analog Pins
//int redLED = 0;
//int greenLED = 1;
int redLED = 5;
int greenLED = 3;

// LED Values
int maxBrightness = 150;
int greenLEDVal = 0;
int redLEDVal = 0;

// Behavior Parameters
int transitionDelay = 15000;
int transitionLengthLong = 30000;
int transitionLengthShort = 13000;
int greenTransitionTime = 0;
int redTransitionTime = 0;

// Pattern Flags
bool lightAll = true;
bool fadeAll = false;
bool alternatingFromOff = false;

// Transition End Triggers
bool isGreenDone = false;
bool isRedDone = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
}

void loop() {
  // Pattern execution loop.
  if (lightAll == true) {
    LightAll();
  } else if (fadeAll == true) {
    FadeAll();
  } else if (alternatingFromOff == true) {
    AlternatingFadesFromOff();
    AlternatingFadesFromGreen();
    AlternatingFadesFromOff();
    AlternatingFadesFromGreen();
    alternatingFromOff = false;
    delay(400);
    lightAll = true;
  }
}

/**
 * Pattern Behaviors
 */
void LightAll() {
  // Fade red and green in.
  if (isGreenDone == false) {
    isGreenDone = LightingGreen(transitionLengthShort);
  }

  if (isRedDone == false) {
    isRedDone = LightingRed(transitionLengthShort);
  }

  if (isGreenDone == true && isRedDone == true) {
    delay(transitionDelay);
    isGreenDone = false;
    isRedDone = false;
    lightAll = false;
    fadeAll = true;
  }
}

void FadeAll() {
  if (isGreenDone == false) {
    isGreenDone = FadingGreen(transitionLengthShort);
  }

  if (isRedDone == false) {
    isRedDone = FadingRed(transitionLengthShort);
  }

  if (isGreenDone == true && isRedDone == true) {
    isGreenDone = false;
    isRedDone = false;
    fadeAll = false;
    alternatingFromOff = true;
  }
}

/**
 * Fades Green and Red LEDs back and forth.
 */
void AlternatingFadesFromOff() {
  bool firstDone = false;
  bool secondDone = false;
  bool thirdDone = false;

  // Light Green.
  while (firstDone == false) {
    if (isGreenDone == false) {
      isGreenDone = LightingGreen(transitionLengthLong);
    } else {
      firstDone = true;
      isGreenDone = false;
    }
  }

  // Fade Green, light Red.
  while (secondDone == false) {
    if (isGreenDone == false) {
      isGreenDone = FadingGreen(transitionLengthLong);
    }

    if (isRedDone == false) {
      isRedDone = LightingRed(transitionLengthLong);
    }

    if (isGreenDone == true && isRedDone == true) {
      secondDone = true;
      isGreenDone = false;
      isRedDone = false;
    }
  }

  // Fade Red, light Green.
  while (thirdDone == false) {
    if (isGreenDone == false) {
      isGreenDone = LightingGreen(transitionLengthLong);
    }

    if (isRedDone == false) {
      isRedDone = FadingRed(transitionLengthLong);
    }

    if (isGreenDone == true && isRedDone == true) {
      thirdDone = true;
      isGreenDone = false;
      isRedDone = false;
    }
  }
}

/**
 * A continuation of the fade that ends in both LEDs off.
 */
void AlternatingFadesFromGreen() {
  bool firstDone = false;
  bool secondDone = false;
  bool thirdDone = false;

  // Fade Green, light Red.
  while (firstDone == false) {
    if (isGreenDone == false) {
      isGreenDone = FadingGreen(transitionLengthLong);
    }
  
    if (isRedDone == false) {
      isRedDone = LightingRed(transitionLengthLong);
    }
  
    if (isGreenDone == true && isRedDone == true) {
      firstDone = true;
      isGreenDone = false;
      isRedDone = false;
    }
  }

  // Fade Red, light Green.
  while (secondDone == false) {
    if (isGreenDone == false) {
      isGreenDone = LightingGreen(transitionLengthLong);
    }
  
    if (isRedDone == false) {
      isRedDone = FadingRed(transitionLengthLong);
    }
  
    if (isGreenDone == true && isRedDone == true) {
      secondDone = true;
      isGreenDone = false;
      isRedDone = false;
    }
  }

  // Fade Green.
  while (thirdDone == false) {
    if (isGreenDone == false) {
      isGreenDone = FadingGreen(transitionLengthLong);
    } else {
      thirdDone = true;
      isGreenDone = false;
    }
  }
}

/***********************************
 *        LIGHT/FADE MODES         *
 ***********************************/

/**
 * Fade Green Mode
 */
bool FadingGreen(int fadeDuration) {
  greenTransitionTime++;
  
  bool isDone = false;
  int greenLEDVal = maxBrightness - (255 * sin(2 * (PI / fadeDuration) * greenTransitionTime));

  if (greenLEDVal <= 0) {
    greenLEDVal = 0;
    isDone = true;
    greenTransitionTime = 0;
  }

  analogWrite(greenLED, greenLEDVal);

  return isDone;
}

/**
 * Fade Red Mode
 */
bool FadingRed(int fadeDuration) {
  redTransitionTime++;
  
  bool isDone = false;
  int redLEDVal = maxBrightness - (255 * sin(2 * (PI / fadeDuration) * redTransitionTime));

  if (redLEDVal <= 0) {
    redLEDVal = 0;
    isDone = true;
    redTransitionTime = 0;
  }

  analogWrite(redLED, redLEDVal);

  return isDone;
}

/**
 * Light Green Mode
 */
bool LightingGreen(int lightingDuration) {
  greenTransitionTime++;
  
  bool isDone = false;
  int greenLEDVal = 255 - (255 * cos(2 * (PI / lightingDuration) * greenTransitionTime));

  if (greenLEDVal >= maxBrightness) {
    greenLEDVal = maxBrightness;
    isDone = true;
    greenTransitionTime = 0;
  }

  analogWrite(greenLED, greenLEDVal);

  return isDone;
}

/**
 * Light Red Mode
 */
bool LightingRed(int lightingDuration) {
  redTransitionTime++;
  
  bool isDone = false;
  int redLEDVal = 255 - (255 * cos(2 * (PI / lightingDuration) * redTransitionTime));

  if (redLEDVal >= maxBrightness) {
    redLEDVal = maxBrightness;
    isDone = true;
    redTransitionTime = 0;
  }

  analogWrite(redLED, redLEDVal);

  return isDone;
}

