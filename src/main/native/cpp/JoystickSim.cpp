#include "JoystickSim.h"

#include <wpi/raw_ostream.h>
#include <hal/simulation/DriverStationData.h>
#include <hal/Ports.h>




void JoystickCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {

    JoystickNTLookupItem* item = (JoystickNTLookupItem*) param ;

    nt::NetworkTableEntry theEntry = item->rootTable->GetEntry(name) ;
    if ( value->type == HAL_Type::HAL_DOUBLE) {
        theEntry.SetDouble( value->data.v_double) ;
    } else if ( value->type == HAL_Type::HAL_INT) {
        theEntry.SetDouble( (double) value->data.v_int) ;
    } else if ( value->type == HAL_Type::HAL_BOOLEAN) {
        theEntry.SetBoolean( value->data.v_boolean ) ;
    }

}



void JoystickSim::Initialize() {

    rootTable = nt::NetworkTableInstance::GetDefault().GetTable("sim")->GetSubTable("Joysticks");
    for ( int i = 0; i < Num_Joysticks; i++) {
        items[i].number= i ;
        std::string tableName("Joystick" + std::to_string(i)) ;
        items[i].rootTable = rootTable->GetSubTable(tableName) ;

        HALSIM_SetJoystickAxisCount(0, Max_Axes) ;
        for( int axis = 0; axis < Max_Axes; axis++ ) {
            std::string axisName("Axis" + std::to_string(axis)) ;
            nt::NetworkTableEntry axisEntry = items[i].rootTable->GetEntry(axisName) ;
            axisEntry.ForceSetDouble(0.0) ;
            axisEntry.AddListener(
                [=](const nt::EntryNotification& ev) -> void {
                    HALSIM_SetJoystickAxis(i, axis, ev.value->GetDouble()) ;
                },
                NT_NotifyKind::NT_NOTIFY_UPDATE);
        }

        HALSIM_SetJoystickButtonCount(0, Max_Buttons) ;
        for( int button = 0; button < Max_Buttons; button++ ) {
            std::string buttonName("Button" + std::to_string(button)) ;
            nt::NetworkTableEntry buttonEntry = items[i].rootTable->GetEntry(buttonName) ;
            buttonEntry.AddListener(
                [=](const nt::EntryNotification& ev) -> void {
                    HALSIM_SetJoystickButton(i, button, ev.value->GetDouble()) ;
                },
                NT_NotifyKind::NT_NOTIFY_UPDATE);
        }

    }

    wpi::outs() << "halsim_nt_bridge Joystick sim initialization complete\n" ;


}