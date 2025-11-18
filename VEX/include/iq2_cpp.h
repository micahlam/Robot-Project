// Minimal, class-style stub for VEXcode IQ2 C++
// NOT the official SDK. Enough for local compile/IntelliSense.

#ifndef IQ2_CPP_H_
#define IQ2_CPP_H_

#include <chrono>
#include <thread>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <cmath>

namespace vex {

/*---------------- Units / basic enums ----------------*/
enum class velocityUnits { pct, rpm };
enum class rotationUnits { deg, rev };
enum class directionType { fwd = 1, rev = -1 };
enum class brakeType     { coast, brake, hold };
enum class distanceUnits { mm, cm, in_ };
enum class timeUnits     { msec, sec };

/* plain identifiers used in class code */
inline constexpr velocityUnits pct  = velocityUnits::pct;
inline constexpr rotationUnits  deg  = rotationUnits::deg;
inline constexpr distanceUnits   mm  = distanceUnits::mm;
inline constexpr timeUnits      msec = timeUnits::msec;
inline constexpr directionType   fwd = directionType::fwd;
inline constexpr brakeType     brake = brakeType::brake;

/*---------------- Ports ----------------*/
constexpr int PORT1=1, PORT2=2, PORT3=3, PORT4=4, PORT5=5;
constexpr int PORT6=6, PORT7=7, PORT8=8, PORT9=9, PORT10=10;

/*---------------- Colors ----------------*/
class color {
  int _v;
public:
  explicit color(int v = 0) : _v(v) {}
  static const color black;
  static const color white;
};
inline const color color::black = color(1);
inline const color color::white = color(0);

/*---------------- Button type (GLOBAL) ----------------*/
enum class buttonType { A, B };

/*---------------- Brain + Screen ----------------*/
class brain {
public:
  class screenType {
    int _row{1}, _col{1};
  public:
    void clearScreen() {}
    void setCursor(int r, int c) { _row = r; _col = c; }
    void print(const char* fmt, ...) {
      (void)_row; (void)_col; // no-op positioning
      va_list args; va_start(args, fmt);
      std::vprintf(fmt, args); std::printf("\n");
      va_end(args);
    }
    void printAt(int /*x*/, int /*y*/, const char* fmt, ...) {
      va_list args; va_start(args, fmt);
      std::vprintf(fmt, args); std::printf("\n");
      va_end(args);
    }
    // filled rectangle
    void drawRectangle(int /*x*/, int /*y*/, int /*w*/, int /*h*/, const color& /*fill*/) {}
    // outline variant (6 args if you ever need it)
    void drawRectangle(int /*x*/, int /*y*/, int /*w*/, int /*h*/, const color& /*outline*/, bool /*outlineOnly*/) {}
  } Screen;

  // simple check; always false in stub (replace if you simulate)
  bool buttonCheck(buttonType /*b*/) { return false; }
};

/*---------------- Motor ----------------*/
class motor {
  int    _port{0};
  bool   _rev{false};
  double _posDeg{0.0};
  double _velPct{0.0};
public:
  motor(int port = 0, bool reversed = false) : _port(port), _rev(reversed) {}
  void resetRotation() { _posDeg = 0.0; }

  void spin(directionType /*dir*/) {}
  void spin(directionType /*dir*/, double speed, velocityUnits /*u*/) { _velPct = speed; }

  void spinToPosition(double pos, rotationUnits /*u*/) { _posDeg = pos; }
  void spinToPosition(double pos, rotationUnits /*u*/, double /*vel*/, velocityUnits /*vu*/, bool /*wait*/ = true) { _posDeg = pos; }

  void spinFor(double amount, rotationUnits /*u*/) { _posDeg += amount; }

  void stop(brakeType /*b*/ = brakeType::brake) { _velPct = 0.0; }
};

/*---------------- Optical ----------------*/
class optical {
  int _port{0};
  bool _lightOn{false};
  double _hue{120.0};
public:
  optical(int port = 0) : _port(port) {}
  void setLight(bool on) { _lightOn = on; }
  void setLightPower(double /*power*/, velocityUnits /*u*/ = velocityUnits::pct) { _lightOn = true; }
  double hue() const { return _hue; }          // stubbed; edit if you simulate
  double brightness() const { return 50.0; }
};

/*---------------- Distance ----------------*/
class distance {
  int _port{0};
public:
  distance(int port = 0) : _port(port) {}
  int objectDistance(distanceUnits /*u*/) const { return 100; } // mm in stub
};

/*---------------- Timing ----------------*/
inline void wait(int t, timeUnits u) {
  using namespace std::chrono;
  if (u == timeUnits::msec) std::this_thread::sleep_for(milliseconds(t));
  else                      std::this_thread::sleep_for(seconds(t));
}

namespace this_thread {
  inline void sleep_for(int ms) { vex::wait(ms, timeUnits::msec); }
}

} // namespace vex

#endif // IQ2_CPP_H_
