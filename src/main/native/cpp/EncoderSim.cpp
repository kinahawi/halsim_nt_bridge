#include "EncoderSim.h"

#include <wpi/raw_ostream.h>
#include <hal/simulation/EncoderData.h>
#include <hal/Ports.h>






void EncoderResetCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {
    // Reset has to be handled differently. The WPI lib code brings reset high for a short time and then
    // low again before the simulation sees it. To deal with that,we'll on ly set it high here and let the 
    // sim bring it low again.

    EncoderNTLookupItem* item = (EncoderNTLookupItem*) param ;
    item->startingCounts = (int) item->countEntry.GetDouble(0.0) ;
}


void EncoderCallbackFunction( const char* name, void* param, const struct HAL_Value* value ) {

    EncoderNTLookupItem* item = (EncoderNTLookupItem*) param ;
    nt::NetworkTableEntry theEntry = item->rootTable->GetEntry(name) ;

    if ( value->type == HAL_Type::HAL_DOUBLE) {
        theEntry.SetDouble( value->data.v_double) ;
    } else if ( value->type == HAL_Type::HAL_INT) {
        theEntry.SetDouble( (double) value->data.v_int) ;
    } else if ( value->type == HAL_Type::HAL_BOOLEAN) {
        theEntry.SetBoolean( value->data.v_boolean ) ;
    }

}



void EncoderSim::Initialize() {

    this->items.resize(HAL_GetNumEncoders()) ;

    rootTable = nt::NetworkTableInstance::GetDefault().GetTable("sim")->GetSubTable("Encoder");
    for ( int i = 0; i < HAL_GetNumEncoders(); i++) {
        items[i].number= i ;
        std::string tableName("Encoder" + std::to_string(i)) ;
        items[i].rootTable = rootTable->GetSubTable(tableName) ;

        nt::NetworkTableEntry countEntry = items[i].rootTable->GetEntry("Count") ;
        items[i].countEntry = countEntry ;
        items[i].startingCounts = (int) countEntry.GetDouble(0.0) ;
        countEntry.ForceSetDouble(0.0) ;
        countEntry.AddListener(
            [this, i](const nt::EntryNotification& ev) -> void {
                int countValue = (int) ev.value->GetDouble() - this->items[i].startingCounts ;
                HALSIM_SetEncoderCount(i, countValue) ;
            },
            NT_NotifyKind::NT_NOTIFY_UPDATE);

        nt::NetworkTableEntry rateEntry = items[i].rootTable->GetEntry("Rate") ;
        rateEntry.ForceSetDouble(0.0) ;
        rateEntry.AddListener(
            [=](const nt::EntryNotification& ev) -> void {
                HALSIM_SetEncoderRate(i, (int32_t)ev.value->GetDouble()) ;
            },
            NT_NotifyKind::NT_NOTIFY_UPDATE);

        nt::NetworkTableEntry directionEntry = items[i].rootTable->GetEntry("Direction") ;
        directionEntry.ForceSetBoolean(false) ;
        directionEntry.AddListener(
            [=](const nt::EntryNotification& ev) -> void {
                HALSIM_SetEncoderDirection(i, (int32_t)ev.value->GetBoolean()) ;
            },
            NT_NotifyKind::NT_NOTIFY_UPDATE);


        HALSIM_RegisterEncoderResetCallback( i, EncoderResetCallbackFunction, &items[i], true) ;
        HALSIM_RegisterEncoderMaxPeriodCallback( i, EncoderCallbackFunction, &items[i], true) ;
        HALSIM_RegisterEncoderReverseDirectionCallback( i, EncoderCallbackFunction, &items[i], true) ;
        HALSIM_RegisterEncoderSamplesToAverageCallback( i, EncoderCallbackFunction, &items[i], true) ;
        HALSIM_RegisterEncoderResetCallback( i, EncoderResetCallbackFunction, &items[i], true) ;
        HALSIM_RegisterEncoderDistancePerPulseCallback( i, EncoderCallbackFunction, &items[i], true) ;

    }

//    wpi::outs() << "halsim_nt_bridge Encoder sim initialization complete\n" ;


}