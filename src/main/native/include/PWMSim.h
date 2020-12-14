#pragma once


#include <networktables/NetworkTableInstance.h>


class PWMNTLookupItem {
  public:
    int number ;
    std::shared_ptr<nt::NetworkTable> rootTable ;
} ;




class PWMSim {
 public:

     std::shared_ptr<nt::NetworkTable> rootTable ;
     std::vector<PWMNTLookupItem> items ;

  void Initialize();
};