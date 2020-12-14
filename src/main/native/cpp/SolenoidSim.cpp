#include "SolenoidSim.h"

#include <wpi/raw_ostream.h>
#include <hal/simulation/PCMData.h>
#include <hal/Ports.h>





void SolenoidCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {

    SolenoidNTLookupItem* item = (SolenoidNTLookupItem*) param ;

    nt::NetworkTableEntry theEntry = item->rootTable->GetEntry(name) ;
    if ( value->type == HAL_Type::HAL_DOUBLE) {
        theEntry.SetDouble( value->data.v_double) ;
    } else if ( value->type == HAL_Type::HAL_INT) {
        theEntry.SetDouble( (double) value->data.v_int) ;
    } else if ( value->type == HAL_Type::HAL_BOOLEAN) {
        theEntry.SetBoolean( value->data.v_boolean ) ;
    }

}



void SolenoidInitializedCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {
    
    SolenoidNTLookupItem* item = (SolenoidNTLookupItem*) param ;

    std::shared_ptr<nt::NetworkTable> rootTable = nt::NetworkTableInstance::GetDefault().GetTable("sim")->GetSubTable("Solenoid");
    std::string tableName( "Module"+std::to_string(item->number)+"/Channel"+std::to_string(item->channel)) ;
    item->rootTable = rootTable->GetSubTable(tableName) ;

    nt::NetworkTableEntry outputEntry = item->rootTable->GetEntry("Output") ;
    outputEntry.ForceSetBoolean(false) ;

    HALSIM_RegisterPCMSolenoidOutputCallback( item->number, item->channel, SolenoidCallbackFunction, item, true) ;

}





void SolenoidSim::Initialize() {

    wpi::outs() << "initialize solenoid module \n" ;

    this->items.resize(HAL_GetNumPCMModules() * HAL_GetNumSolenoidChannels()) ;

    int num_channels = HAL_GetNumSolenoidChannels();
    int num_modules = HAL_GetNumPCMModules() ;
    for ( int i = 0; i < num_modules; i++) {
        for ( int j = 0; j < num_channels; j++) {
            items[i*num_channels+j].number = i ;
            items[i*num_channels+j].channel = j ;
            HALSIM_RegisterPCMSolenoidInitializedCallback( i, j, SolenoidInitializedCallbackFunction, &items[i*num_channels+j], false) ;
        }
    }

    wpi::outs() << "halsim_nt_bridge Solenoid sim initialization complete\n" ;


}