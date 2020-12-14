#include "DIOSim.h"

#include <wpi/raw_ostream.h>
#include <hal/simulation/DIOData.h>
#include <hal/Ports.h>
#include <networktables/NetworkTableInstance.h>


#include <string>


void DIOCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {

    DIONTLookupItem* item = (DIONTLookupItem*) param ;

    nt::NetworkTableEntry theEntry = item->rootTable->GetEntry(name) ;
    if ( value->type == HAL_Type::HAL_DOUBLE) {
        theEntry.SetDouble( value->data.v_double) ;
    } else if ( value->type == HAL_Type::HAL_INT) {
        theEntry.SetDouble( (double) value->data.v_int) ;
    } else if ( value->type == HAL_Type::HAL_BOOLEAN) {
        theEntry.SetBoolean( value->data.v_boolean ) ;
    }

}



void DIOInitializedCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {
    
    DIONTLookupItem* item = (DIONTLookupItem*) param ;

    std::shared_ptr<nt::NetworkTable> rootTable = nt::NetworkTableInstance::GetDefault().GetTable("sim")->GetSubTable("DIO");

    char num[20] ;
    snprintf( num, 15, "%02d", item->number) ;
    std::string tableName( "DIO"+ std::string(num)) ;
    item->rootTable = rootTable->GetSubTable(tableName) ;
    item->rootTable->GetEntry("PulseLength").ForceSetDouble(0.0);
    item->rootTable->GetEntry("Input").ForceSetBoolean(false);

    nt::NetworkTableEntry valueEntry = item->rootTable->GetEntry("Value") ;
    valueEntry.ForceSetBoolean(false);

    valueEntry.AddListener(
       [=] (const nt::EntryNotification& ev) -> void {
        if ( HALSIM_GetDIOIsInput( item->number) ) {
            HALSIM_SetDIOValue( item->number, ev.value->GetBoolean() ) ;
        }
    },
    NT_NotifyKind::NT_NOTIFY_UPDATE);

    HALSIM_RegisterDIOAllCallbacks( item->number, DIOCallbackFunction, item, true) ;

}







void DIOSim::Initialize() {

    wpi::outs() << "initialize DIOSim \n" ;

    this->items.resize(HAL_GetNumPCMModules() ) ;
    int num_channels = HAL_GetNumDigitalChannels();
    for ( int i = 0; i < num_channels; i++) {
        items[i].number = i ;
        HALSIM_RegisterDIOInitializedCallback( i, DIOInitializedCallbackFunction, &items[i], false) ;
    }

    wpi::outs() << "halsim_nt_bridge DIO sim initialization complete\n" ;

}