/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
**  This extension reimplements enough of the FRC_Network layer to enable the
**    simulator to communicate with a driver station.  That includes a
**    simple udp layer for communication.
**  The protocol does not appear to be well documented; this implementation
**    is based in part on the Toast ds_comms.cpp by Jaci and in part
**    by the protocol specification given by QDriverStation.
**--------------------------------------------------------------------------*/

#include <sys/types.h>

#include <atomic>
#include <cstring>

#include <DSCommPacket.h>
//#include <hal/Extensions.h>
#include <wpi/EventLoopRunner.h>
#include <wpi/StringRef.h>
#include <wpi/raw_ostream.h>

// #include <wpi/raw_uv_ostream.h>
// //#include <wpi/uv/Tcp.h>
#include <wpi/uv/Timer.h>
//#include <wpi/uv/Udp.h>
//#include <wpi/uv/util.h>

#include <HALSimDsNt.h>
#include <PWMSim.h>
#include <EncoderSim.h>
#include <JoystickSim.h>
#include <SolenoidSim.h>
#include <DIOSim.h>




#if defined(Win32) || defined(_WIN32)
#pragma comment(lib, "Ws2_32.lib")
#endif

using namespace wpi::uv;

//static std::unique_ptr<Buffer> singleByte;
//static std::atomic<bool> gDSConnected = false;




static std::unique_ptr<wpi::EventLoopRunner> eventLoopRunner;

static void SetupEventLoop(wpi::uv::Loop& loop) {

  // auto simLoopTimer = Timer::Create(loop);
  // simLoopTimer->timeout.connect([] {
  //   wpi::outs() << "timer expired.\n" ;
  // }) ;
  // simLoopTimer->Start(Timer::Time{1000}, Timer::Time{1000});


}







static HALSimDSNT dsnt;
static PWMSim pwmSim ;
static EncoderSim encoderSim ;
static JoystickSim joystickSim ;
static SolenoidSim solenoidSim ;
static DIOSim dioSim ;


extern "C" {
#if defined(WIN32) || defined(_WIN32)
__declspec(dllexport)
#endif
    int HALSIM_InitExtension(void) {
  wpi::outs() << "halsim_nt_bridge Initializing.\n" ;

  dsnt.Initialize();
  pwmSim.Initialize() ;
  encoderSim.Initialize() ;
  joystickSim.Initialize() ;
  solenoidSim.Initialize() ;
  dioSim.Initialize() ;

  // wpk - may not need this at all since everything seems to be working with callbacks
  eventLoopRunner = std::make_unique<wpi::EventLoopRunner>();
  eventLoopRunner->ExecAsync(SetupEventLoop);


  wpi::outs() << "halsim_nt_bridge Initialized!\n" ;
  return 0;
}
}  // extern "C"