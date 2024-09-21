#pragma once

#include "pluginlib_global.h"

#include <memory>
#include <functional>

#include <QString>
#include <QVariant>

class PitikappModuleInstanceDataPrivate;

/** ************************************************************************************************
* @class        PitikappModuleInstanceData
*
* @brief        Class that contains all data related to a PitikappModuleInstance.
*
* @details      The data is divided in different categories:
*                - Client parameters: those values as sent as parameters to the client application
*                  displaying this module instance. For example: color, size, title, etc.
*                - Client data: those values as sent to the client application displaying this
*                  module instance to show the state of the module. For example: temperature, usage,
*                  etc.
*                - Local parameters: those parameters are only saved locally and can be used freely
*                  by the plugin or module. They are not sent to a client.\n\n
*
*               A PitikappModuleInstanceData takes an ID as parameter is the constructor. This ID
*               must match one of those registered in the plugin with registerModuleDataClass.
*
*               A subclass of PitikappModuleInstanceData must be created for each type of data
*               that will be used by the modules of a plugin. Predefined data types can also be
*               used, such as PitikappModuleInstanceGaugeData.
*
*               When data is reloaded at application startup, onReload() is called.
*               Override it for custom actions when data is reloaded (e.g. initialize resources).
***************************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappModuleInstanceData
{
    friend class PitikappPrivateData;

    public:
        using DataChangeCallbackFunction = std::function<void(const QString &, const QVariant&)>;

        PitikappModuleInstanceData(const QString &dataClassId);
        virtual ~PitikappModuleInstanceData();

        virtual void onReload();

        const QString &classId() const;

        void setLocalParameter(const QString &key, const QVariant &value);
        void setClientParameter(const QString &key, const QVariant &value);
        void setClientData(const QString &key, const QVariant &value);
        void addHelper(const QString &helperId, QObject *helperObject);

        void setLocalParameterChangeCallback(DataChangeCallbackFunction callback);
        void setClientParameterChangeCallback(DataChangeCallbackFunction callback);
        void setClientDataChangeCallback(DataChangeCallbackFunction callback);

        QVariant getLocalParameter(const QString &key) const;
        QVariant getClientParameter(const QString &key) const;
        QVariant getClientData(const QString &key) const;

        void setDisplayTitleEditable(bool editable);
        bool isDisplayTitleEditable();

        void setLocalTitle(const QString &title);
        QString getLocalTitle() const;

    private:
        std::unique_ptr<PitikappModuleInstanceDataPrivate> data;
};
