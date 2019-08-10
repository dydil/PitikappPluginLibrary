#pragma once

#include "pluginlib_global.h"

#include <memory>

#include <QString>
#include <QUrl>

class PitikappModuleDataInfoPrivate;

/** ************************************************************************************************
* @class        PitikappModuleDataInfo
*
* @brief        Class containing metadata related to a PitikappModuleInstanceData.
*
* @details      Any PitikappModuleInstanceData object must use a known ID that has been registered
*               in a PitikappModuleDataInfo using PitikappPlugin::registerModuleDataClass.\n
*               The information must be setup when constructing the plugin. Any change after this
*               point will be ignored.\n
*               The following data is required:
*                - A path to a QML file that will be used in the server to edit the parameters of a
*                  module.
*                - A path to a QML file that will be used in the client to display any data provided
*                  by the server.
*                - Paths to any file that could be required by the client, such as images.
*               Client files are sent to the client the first time it is connected to the server.\n\n
*
*               It is preferable to use a file from a Qt resource compiled in the plugin.
*               For example: qrc:/com.plugin.id/com.module.id/ParameterFile.qml
***************************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappModuleDataInfo final
{
    friend class PitikappPrivateData;

    public:
        PitikappModuleDataInfo(const QString &id);
        PitikappModuleDataInfo(const PitikappModuleDataInfo &other);
        ~PitikappModuleDataInfo();

        void setParameterEditionWidgetPath(const QUrl &path);
        void setClientDataDisplayWidgetPath(const QUrl &path);
        void addClientFile(const QUrl &path);

        const QString &getId() const;
        const QUrl &getParameterEditionWidgetPath() const;
        const QUrl &getClientDataDisplayWidgetPath() const;
        const QVector<QUrl> &getClientFiles() const;

    private:
        std::unique_ptr<PitikappModuleDataInfoPrivate> data;
};
