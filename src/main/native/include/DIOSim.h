#pragma once

#include <hal/Ports.h>

#include <networktables/NetworkTableInstance.h>
//#include <wpi/mutex.h>

class DIONTLookupItem {
  public:
    int number ;
    std::shared_ptr<nt::NetworkTable> rootTable ;
} ;


class DIOSim {
 public:

     std::shared_ptr<nt::NetworkTable> rootTable ;
     std::vector<DIONTLookupItem> items ;

  void Initialize();
};
