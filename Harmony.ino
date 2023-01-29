enum signalStates {INERT, GO, RESOLVE, GOCHALLENGE};
byte signalState = INERT;
//water, fire, plant, rock, lightning (0-4)
//water cannot go next to fire or lightning
//fire cannot go next to water or plant
//plant cannot go next to fir or rock
//rock cannot go next to plant or lightning
//lightning cannot go next to rock or water
byte gameState = 0;
byte cycleAngry = 0;
byte cycleHappy = 0;
bool challengeMode = false;

void setup() {
  randomize();
  changeGameState();
}

void loop() {
  switch (signalState) {
    case INERT:
      inertLoop();
      break;
    case GO:
      goLoop();
      break;
    case RESOLVE:
      resolveLoop();
      break;
    case GOCHALLENGE:
      goChallengeLoop();
      break;
  }
  setValueSentOnAllFaces(signalState);
  if (signalState == INERT) {
    setValueSentOnAllFaces(gameState + 10);
  }
}

void inertLoop() {
  if (buttonMultiClicked()) {
    byte clicks = buttonClickCount();
    if (clicks == 3) {
      signalState = GOCHALLENGE;
    }
  }
  //set myself to GO and randomize color if double clicked
  if (buttonDoubleClicked()) {
    signalState = GO;
    changeGameState();
  }
  cycleAngry = sin8_C(millis()/2 % 256);
  cycleHappy = sin8_C(millis()/6 % 256);
  //set color to white by default
  dispBlank();
  //listen for neighbors in GO and for colors they dont like
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == GO) {//a neighbor saying GO!
        signalState = GO;
        changeGameState();
        break;
      } if (getLastValueReceivedOnFace(f) == GOCHALLENGE) {//a neighbor saying GOCHALLENGE!
        signalState = GOCHALLENGE;
        break;
      } else if (getLastValueReceivedOnFace(f) == gameState + 11) {
        dispAngry(f);
      } else if (getLastValueReceivedOnFace(f) == gameState + 9) {
        dispAngry(f);
      } else if ((getLastValueReceivedOnFace(f) == 10) && (gameState == 4)) {
        dispAngry(f);
      } else if ((getLastValueReceivedOnFace(f) == 14) && (gameState == 0)) {
        dispAngry(f);
      } else if ((getLastValueReceivedOnFace(f) == gameState + 10) && (challengeMode)) {
        dispAngry(f);
      }  
    } else {
      //if no neighbor it senses no disagreement
      showColorOnFace(f);
    }
  }
}

void dispBlank() {
  if (gameState == 0) {
    setColor(makeColorRGB(cycleHappy,cycleHappy,255));
  } if (gameState == 1) {
    setColor(makeColorRGB(255,cycleHappy,cycleHappy));
  } if (gameState == 2) {
    setColor(makeColorRGB(cycleHappy,255,cycleHappy));
  } if (gameState == 3) {
    setColor(makeColorRGB(255,170 + cycleHappy/3,cycleHappy));
  } if (gameState == 4) {
    setColor(makeColorRGB(255,255,cycleHappy));
  }
}

void dispAngry(byte f) {
  if (gameState == 0) {
    setColorOnFace(dim(BLUE,cycleAngry),f);
  } if (gameState == 1) {
    setColorOnFace(dim(RED,cycleAngry),f);
  } if (gameState == 2) {
    setColorOnFace(dim(GREEN,cycleAngry),f);
  } if (gameState == 3) {
    setColorOnFace(dim(ORANGE,cycleAngry),f);
  } if (gameState == 4) {
    setColorOnFace(dim(YELLOW,cycleAngry),f);
  }
}

void goLoop() {
  signalState = RESOLVE;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not heard the GO news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == INERT) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = GO;
      }
    }
  }
  setColor(dim(WHITE,150));
}

void goChallengeLoop() {
  signalState = RESOLVE;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not heard the GOCHALLENGE news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == INERT) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = GOCHALLENGE;
      }
    }
  }
  setColor(dim(MAGENTA,200));
  if (signalState == RESOLVE) {
    challengeMode = !challengeMode;
  }
}

void resolveLoop() {
  signalState = INERT;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not moved to RESOLVE
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == GO) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
        signalState = RESOLVE;
      }
    }
  }
  setColor(WHITE);
}

void changeGameState() {
  //set to random color
  //randomize();
  gameState = random(4);
}

void showColorOnFace(byte f) {
  //show your element on light f
  if (gameState == 0) {
    setColorOnFace(BLUE, f);
  } else if (gameState == 1) {
    setColorOnFace(RED, f);
  } else if (gameState == 2) {
    setColorOnFace(GREEN, f);
  } else if (gameState == 3) {
    setColorOnFace(ORANGE, f);
  } else if (gameState == 4) {
    setColorOnFace(YELLOW, f);
  }
}
