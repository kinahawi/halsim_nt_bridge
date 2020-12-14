#pragma once

#include <networktables/NetworkTableInstance.h>

// wpk - this is a really simple implementation created to support a single Logitech F310
// attached. It will have to be enhanced to handlemore general cases and multiple controllers.

static const int Num_Joysticks = 2 ;
static const int Max_Axes = 8 ;

// wpk num buttons are limited to single digit numbers because of the terrible hack 
// I'm using to get the button number. Fix in the future
static const int Max_Buttons = 9 ;
static const int Max_POVs = 4 ;

class JoystickNTLookupItem {
  public:
    int number ;
    std::shared_ptr<nt::NetworkTable> rootTable ;
} ;


class JoystickSim {
 public:

     std::shared_ptr<nt::NetworkTable> rootTable ;
     JoystickNTLookupItem items[Num_Joysticks];

  void Initialize();
};
