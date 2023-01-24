enum signalStates {INERT, GO, RESOLVE};
byte signalState = INERT;
//water, fire, plant, rock, lightning (0-4)
//water cannot go next to fire or lightning
//fire cannot go next to water or plant
//plant cannot go next to fir or rock
//rock cannot go next to plant or lightning
//lightning cannot go next to rock or water
byte gameState = 0;

void setup() {
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
  }
  setValueSentOnAllFaces(signalState);
  if (signalState == INERT) {
    setValueSentOnAllFaces(gameState + 3);
  }
}

void inertLoop() {
  //set myself to GO and randomize color if double clicked
  if (buttonDoubleClicked()) {
    signalState = GO;
    changeGameState();
  }
  //set color to white by default
  setColor(WHITE);
  //listen for neighbors in GO and for colors they dont like
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == GO) {//a neighbor saying GO!
        signalState = GO;
        changeGameState();
      } else if (getLastValueReceivedOnFace(f) == gameState + 4) {
        showColorOnFace(f);
      } else if (getLastValueReceivedOnFace(f) == gameState + 2) {
        showColorOnFace(f);
      } else if ((getLastValueReceivedOnFace(f) == 3) && (gameState == 4)) {
        showColorOnFace(f);
      } else if ((getLastValueReceivedOnFace(f) == 7) && (gameState == 0)) {
        showColorOnFace(f);
      } 
    } else {
      //if no neighbor it sense no disagreement
      showColorOnFace(f);
    }
  }
}

void goLoop() {
  signalState = RESOLVE;//I default to this at the start of the loop. Only if I see a problem does this not happen

  //look for neighbors who have not heard the GO news
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if ((!getLastValueReceivedOnFace(f) == GO) && (!getLastValueReceivedOnFace(f) == RESOLVE)) {//This neighbor doesn't know it's GO time. Stay in GO
        signalState = GO;
      }
    }
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
}

void changeGameState() {
  //set to random color
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
