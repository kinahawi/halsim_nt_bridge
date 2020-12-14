#pragma once

#include <hal/Ports.h>

#include <networktables/NetworkTableInstance.h>
//#include <wpi/mutex.h>

class EncoderNTLookupItem {
  public:
    int number ;
    int startingCounts ;
    std::shared_ptr<nt::NetworkTable> rootTable ;
    nt::NetworkTableEntry countEntry ;
} ;


class EncoderSim {
 public:

     std::shared_ptr<nt::NetworkTable> rootTable ;
     std::vector<EncoderNTLookupItem> items ;
     //EncoderNTLookupItem items[Num_Encoders];

  void Initialize();
};
