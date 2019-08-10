#pragma once

#include "pluginlib_global.h"

#include <memory>

#include "PitikappModuleInstanceData.h"

class PitikappModuleInstancePrivate;

/** ************************************************************************************************
* @class        PitikappModuleInstance
*
* @brief        Class to represent an active Pitikapp module.
*
* @details      A module instance is an object that will provide data to display in a client.\n
*               Create a subclass of PitikappModuleInstance for each module. Override processData
*               to process data that was sent by a client (e.g. the user pressed a button).\n\n
*
*               A PitikappModuleInstance requires a PitikappModuleInstanceData.\n\n
*
*               sendNotification() can be used to show any text at the bottom of the connected client
*               for the given duration in milliseconds.
***************************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappModuleInstance
{
    friend class PitikappPrivateData;

    public:
        PitikappModuleInstance(PitikappModuleInstanceData *moduleInstanceData);
        virtual ~PitikappModuleInstance();

        PitikappModuleInstanceData *instanceData();
        virtual void processData(const QVariantMap &data);
        void sendNotification(const QString &text, int durationMs);

    private:
        std::unique_ptr<PitikappModuleInstancePrivate> data;
};
