// Expanded non-proprietary stub that mimics the public-facing API
// of the VEXcode IQ2 C++ header enough for local compilation
// and IntelliSense. This is NOT the official SDK and contains
// simplified implementations for desktop development only.

#ifndef IQ2_CPP_H_
#define IQ2_CPP_H_

#include <chrono>
#include <thread>
#include <cstdint>
#include <cmath>
#include <string>

namespace vex {

// Units and enums (simplified)
enum class velocityUnits { pct, rpm };
enum class rotationUnits { deg, rev };
enum class directionType { fwd = 1, rev = -1 };

// Backwards-compatible legacy types/values used by older VEX examples
enum direction_t { forward = 1, reverse = -1 };
enum unit_t { degrees = 0, percent = 1 };

// Port constants
constexpr int PORT1 = 1; constexpr int PORT2 = 2; constexpr int PORT3 = 3; constexpr int PORT4 = 4;
constexpr int PORT5 = 5; constexpr int PORT6 = 6; constexpr int PORT7 = 7; constexpr int PORT8 = 8;
constexpr int PORT9 = 9; constexpr int PORT10 = 10;

// Simple Brain + Screen stub
class brain {
public:
  class screenType {
  public:
    void printAt(int /*x*/, int /*y*/, const char * /*fmt*/) {}
  } Screen;

  brain() = default;
};

// Button/controller stubs
class button {
public:
  button() = default;
  bool pressing() const { return false; }
};

class controller {
public:
  controller() = default;
  // named buttons used in example projects
  button ButtonLUp;
  button ButtonLDown;
  button ButtonRUp;
  button ButtonRDown;
};

// Very small motor class sufficient for project usage
class motor {
  int _port;
  bool _reversed;
  double _position = 0.0; // stored in degrees for this stub
  double _velocity = 0.0; // percent
public:
  motor(int port = 0, bool reversed = false) : _port(port), _reversed(reversed) {}

  // Position/rotation helpers
  void setPosition(double pos, rotationUnits /*u*/) { _position = pos; }
  double position(rotationUnits /*u*/) const { return _position; }
  // Legacy overload using `unit_t` from older code
  void setPosition(double pos, unit_t /*u*/) { _position = pos; }
  double position(unit_t /*u*/) const { return _position; }

  // Basic spin API (overloads)
  void spin(directionType /*dir*/) {}
  void spin(directionType /*dir*/, double speed, velocityUnits /*u*/) { _velocity = speed; }
  // Legacy overloads accepting older `direction_t` and `unit_t` types
  void spin(direction_t dir) { spin(static_cast<directionType>(dir)); }
  void spin(direction_t dir, double speed, unit_t /*u*/) { spin(static_cast<directionType>(dir), speed, velocityUnits::pct); }

  void spinToPosition(double pos, rotationUnits /*u*/) { _position = pos; }
  void spinToPosition(double pos, rotationUnits /*u*/, bool /*wait*/) { _position = pos; }
  // Legacy overload
  void spinToPosition(double pos, unit_t /*u*/) { _position = pos; }

  void spinFor(double amount, rotationUnits /*u*/) { _position += amount; }
  // Legacy overload
  void spinFor(double amount, unit_t /*u*/) { _position += amount; }
  void stop() { _velocity = 0.0; }
};

// Lightweight thread helpers similar to vex::this_thread
namespace this_thread {
  inline void sleep_for(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }
}

} // namespace vex

#endif // IQ2_CPP_H_
