#pragma once

#include <hal/Ports.h>
#include <networktables/NetworkTableInstance.h>

static const int Num_Solenoids = 8 ;

class SolenoidNTLookupItem {
  public:
    int number ;
    int channel ;
    std::shared_ptr<nt::NetworkTable> rootTable ;
} ;


class SolenoidSim {
 public:

//wpk consider making items a vector that can be resized based on count from Ports.h
     std::shared_ptr<nt::NetworkTable> rootTable ;
     std::vector<SolenoidNTLookupItem> items;

  void Initialize();
};
