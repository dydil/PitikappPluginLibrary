#pragma once

#include "pluginlib_global.h"

#include <memory>

#include <QUrl>

class PitikappModuleInfoData;

/** ************************************************************************************************
* @class        PitikappModuleInfo
*
* @brief        Class to store meta-data of a module. This class is only informative, it does not
*               do anything. See PitikappModuleInstance for information about how to create
*               an active module.
*
* @details      A PitikappPlugin can provide several modules. For each module, create an instance
*               of PitikappModuleInfo and fill it with the appropriate values.\n
*               The following values can be read but not modified:
*                   - The ID attached to this module. It is used internally by the application.
*                   - The name of this module. It is displayed in the application in the module
*                     selection window.
*               The following values can be modified in real time.
*                   - icon: this is the icon displayed in the module selection window. The icon is
*                     a QUrl pointing to an item in a Qt resource file compiled in the plugin.
*                     Example: qrc:/com.plugin.id/com.module.id/icon.png
*                   - description: this is the information displayed in the plugin list in the
*                     information widget.
*                   - valid: when set to false (default value), a warning icon is displayed in the
*                     application to indicate there is something wrong with this module. Once the
*                     required data is initialized in the plugin, this value must be set to true.
*                   - visible: this value is used to show or mask the module in the module selection
*                     window. By default, a module is hidden.
***************************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappModuleInfo final
{
    friend class PitikappPrivateData;

    public:
        PitikappModuleInfo(const QString &moduleId, const QString &moduleName);
        ~PitikappModuleInfo();

        PitikappModuleInfo(PitikappModuleInfo &&other);
        PitikappModuleInfo& operator=(PitikappModuleInfo &&other);

        const QString &id() const;
        const QString &name() const;
        const QUrl &icon() const;
        const QString &description() const;
        bool valid() const;
        bool visible() const;

        void setIcon(const QUrl &icon);
        void setDescription(const QString &description);
        void setValid(bool valid);
        void setVisible(bool visible);

    private:
        std::unique_ptr<PitikappModuleInfoData> data;
};
