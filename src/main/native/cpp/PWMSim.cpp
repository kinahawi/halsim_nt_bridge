#include "PWMSim.h"

#include <wpi/raw_ostream.h>
#include <hal/simulation/PWMData.h>
#include <hal/Ports.h>



void PWMCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {

    PWMNTLookupItem* item = (PWMNTLookupItem*) param ;

    nt::NetworkTableEntry theEntry = item->rootTable->GetEntry(name) ;
    if ( value->type == HAL_Type::HAL_DOUBLE) {
        theEntry.SetDouble( value->data.v_double) ;
    } else if ( value->type == HAL_Type::HAL_INT) {
        theEntry.SetDouble( (double) value->data.v_int) ;
    }

}



void PWMSim::Initialize() {

    rootTable = nt::NetworkTableInstance::GetDefault().GetTable("sim")->GetSubTable("PWM");
    this->items.resize( HAL_GetNumPWMChannels()) ;

    for( int i = 0; i < HAL_GetNumPWMChannels(); i++) {

        std::string tableName("PWM" + std::to_string(i)) ;

        items[i].number = i ;
        items[i].rootTable = rootTable->GetSubTable(tableName) ;
        
        HALSIM_RegisterPWMSpeedCallback( i, PWMCallbackFunction, &items[i], true) ;
        HALSIM_RegisterPWMRawValueCallback( i, PWMCallbackFunction, &items[i], true) ;
        HALSIM_RegisterPWMPositionCallback( i, PWMCallbackFunction, &items[i], true) ;
        HALSIM_RegisterPWMPeriodScaleCallback( i, PWMCallbackFunction, &items[i], true) ;
    }

}