/*----------------------------------------------------------------------------
  MTE100/MTE121 — Autonomous Shelf-Stocking (class-style)
  Team: Micah, Lakindu, Aidan, Gerry
  Ports: Left=10, Right=9, Optical=8, Distance=6, Lift=4, Claw=5
----------------------------------------------------------------------------*/
#include "vex.h"
using namespace vex;

/* ----------------- Devices ----------------- */
brain   Brain;
motor   leftMotor (PORT10, false);
motor   rightMotor(PORT9 , true );   // reverse so + = forward
motor   liftMotor (PORT4 , false);
motor   clawMotor (PORT5 , false);
optical optical1  (PORT8);
distance dist1    (PORT6);

/* ----------------- Simple constants ----------------- */
// Tape + markers (set your hues)
double HUE_GREEN   = 120.0;   // line to follow
double HUE_TOL     = 25.0;

double HUE_DISP    =   0.0;   // dispenser marker (red)
double HUE_SHELF   = 220.0;   // shelf marker (blue)
double HUE_EXIT    =  45.0;   // exit marker (yellow)
double HUE_M_TOL   = 20.0;

// Drive
int DRIVE_BASE = 35;          // %
int DRIVE_TURN = 20;          // %
int DRIVE_MAX  = 60;          // %

// Z rows (fill after calibration)
int rowTicks[3] = { 80, 210, 340 };     // bottom, mid, top

// Distance thresholds (mm) at fixed standoff
int DIST_OCC_THR = 80;        // < thr = occupied

// Grid status
const int ROWS = 3, COLS = 4;
bool filled[ROWS][COLS] = { false };
int totalPlaced = 0;
int tr = 0;                   // current row  (0 = bottom)
int tc = 0;                   // current col  (0 = left)

/* ----------------- Helpers (kept simple) ----------------- */
void drive(int l, int r) {
  if (l > DRIVE_MAX) l = DRIVE_MAX; if (l < -DRIVE_MAX) l = -DRIVE_MAX;
  if (r > DRIVE_MAX) r = DRIVE_MAX; if (r < -DRIVE_MAX) r = -DRIVE_MAX;
  leftMotor.spin(fwd,  l, pct);
  rightMotor.spin(fwd, r, pct);
}
void stopDrive() { leftMotor.stop(brake); rightMotor.stop(brake); }

void clawOpen()  { clawMotor.spinToPosition(  0, deg, 40, pct, false); }
void clawClose() { clawMotor.spinToPosition( 60, deg, 40, pct, false); }

void moveZ(int ticks) { liftMotor.spinToPosition(ticks, deg, 50, pct, false); }
void zTravel() { moveZ(rowTicks[0]); }

/* ----------------- Micah ----------------- */
// Return marker type as an int: -1 none, 0 dispenser, 1 shelf, 2 exit
int getMarker() {
  double h = optical1.hue();
  if (fabs(h - HUE_DISP ) <= HUE_M_TOL) return 0;
  if (fabs(h - HUE_SHELF) <= HUE_M_TOL) return 1;
  if (fabs(h - HUE_EXIT ) <= HUE_M_TOL) return 2;
  return -1;
}

// Median of N distance samples, then classify empty/occupied
int scanCell(int r, int c, int nSamples) {
  if (nSamples < 1) nSamples = 1;
  if (nSamples > 15) nSamples = 15;
  int a[15]; int i, j, t;

  for (i=0; i<nSamples; i++) {
    a[i] = dist1.objectDistance(mm);
    wait(10, msec);
  }
  // simple selection sort
  for (i=0; i<nSamples-1; i++) {
    for (j=i+1; j<nSamples; j++) {
      if (a[j] < a[i]) { t=a[i]; a[i]=a[j]; a[j]=t; }
    }
  }
  int med = a[nSamples/2];
  if (med < DIST_OCC_THR) return 1; // occupied
  return 0;                         // empty
}

/* ----------------- Lakindu ----------------- */
// Single-sensor line follower using hue bands (no PID)
void lineFollow(int vBase, int vTurn) {
  double h = optical1.hue();
  if (h < HUE_GREEN - HUE_TOL) {
    // too blue side → steer right
    drive(vBase + vTurn, vBase - vTurn);
  } else if (h > HUE_GREEN + HUE_TOL) {
    // too yellow side → steer left
    drive(vBase - vTurn, vBase + vTurn);
  } else {
    // centered
    drive(vBase, vBase);
  }
}

// Shelf “dock” without bumper: stop after dwelling on shelf marker
bool dockAtShelf(int vCreep, int timeoutMs) {
  int elapsed = 0;
  int dwell = 0;
  const int NEED = 250; // ms on marker to accept
  while (elapsed < timeoutMs) {
    int m = getMarker();
    if (m == 1) { // shelf
      drive(vCreep, vCreep);
      wait(20, msec);
      dwell += 20;
      if (dwell >= NEED) { stopDrive(); return true; }
    } else {
      dwell = 0;
      lineFollow(vCreep, 15);
      wait(20, msec);
    }
    elapsed += 20;
  }
  stopDrive();
  return false;
}

/* ----------------- Aidan ----------------- */
void pickFromDispenser() {
  clawOpen();
  moveZ(rowTicks[0] - 20); wait(150, msec);
  clawClose();             wait(150, msec);
  moveZ(rowTicks[0] + 40); // travel height
}

void placeAtCell(int r, int c) {
  moveZ(rowTicks[r]); wait(150, msec);
  clawOpen();         wait(150, msec);
}

/* ----------------- Gerry ----------------- */
void recordEvent(int r, int c) {
  filled[r][c] = true;
  totalPlaced = totalPlaced + 1;
}

void showSummary() {
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print("Run Summary");
  Brain.Screen.setCursor(3,1);
  Brain.Screen.print("Total placed: %d", totalPlaced);

  int x0=10, y0=60, w=25, h=25;
  int r,c;
  for (r=ROWS-1; r>=0; r--) {
    for (c=0; c<COLS; c++) {
      int x = x0 + c*(w+5);
      int y = y0 + (ROWS-1-r)*(h+5);
      color fillColor = filled[r][c] ? color::black : color::white;
      Brain.Screen.drawRectangle(x,y,w,h, fillColor);
      Brain.Screen.drawRectangle(x,y,w,h, color::black);
    }
  }
  Brain.Screen.setCursor(ROWS*2+8,1);
  Brain.Screen.print("A=restart  B=idle");
}

void handleFault(int code) {
  stopDrive();
  zTravel();
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print("FAULT: %d", code);
  while(!Brain.buttonCheck(buttonType::A)) { wait(10, msec); }
  Brain.Screen.clearScreen();
}

/* ----------------- Startup ----------------- */
void setup() {
  optical1.setLightPower(100, pct);
  optical1.setLight(true);
  leftMotor.resetRotation();
  rightMotor.resetRotation();
  liftMotor.resetRotation();
  clawMotor.resetRotation();
  zTravel();
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(2,30,"HAHA VEX PROJECT");
}

/* ----------------- Main ----------------- */
int main() {
  setup();

  // press A to start
  while(!Brain.buttonCheck(buttonType::A)) { wait(10, msec); }

  while (true) {
    // Follow line until a marker is seen
    int marker = -1;
    while (marker == -1) {
      lineFollow(DRIVE_BASE, DRIVE_TURN);
      marker = getMarker();
      wait(10, msec);
    }

    // Act on marker
    if (marker == 0) {
      // dispenser
      pickFromDispenser();
    }
    else if (marker == 1) {
      // shelf
      bool ok = dockAtShelf(20, 4000);
      if (!ok) {
        handleFault(1);
      } else {
        int occ = scanCell(tr, tc, 7);
        if (occ == 0) {
          placeAtCell(tr, tc);
          recordEvent(tr, tc);
          tc = tc + 1;
          if (tc >= COLS) { tc = 0; tr = tr + 1; }
        } else {
          // skip occupied
          tc = tc + 1;
          if (tc >= COLS) { tc = 0; tr = tr + 1; }
        }
        if (tr >= ROWS) {
          showSummary();
          while(!Brain.buttonCheck(buttonType::A) &&
                !Brain.buttonCheck(buttonType::B)) { wait(10, msec); }
          if (Brain.buttonCheck(buttonType::A)) {
            int r,c;
            for (r=0;r<ROWS;r++) for (c=0;c<COLS;c++) filled[r][c]=false;
            totalPlaced=0; tr=0; tc=0;
          } else {
            stopDrive();
            break;
          }
        }
      }
    }
    else if (marker == 2) {
      // exit
      showSummary();
      break;
    }

    wait(10, msec);
  }

  return 0;
}
