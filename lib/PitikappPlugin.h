#pragma once

#include "pluginlib_global.h"

#include <memory>

#include <QString>

#include "PitikappModuleInfo.h"
#include "PitikappModuleDataInfo.h"
#include "PitikappModuleInstance.h"

class PitikappPluginData;

/** ************************************************************************************************
* @class        PitikappPlugin
*
* @details      To create a new plugin, create a subclass of PitikappPlugin.\n
*               Only one instance of PitikappPlugin must exist for each new plugin.\n
*               Define getPitikappPluginInstance() to give access to this instance.\n\n
*
*               A plugin can provide several module. See PitikappModuleInfo for the definition of
*               of a module.\n\n
*
*               The following properties of a plugin can be read but not modified:
*                   - The ID attached to this plugin. It is used internally by the application.
*                   - The name of this plugin. It is displayed in the application in the module
*                     selection window.
*               The following values can be modified in real time.
*                   - icon: this is the icon displayed in the module selection window. The icon is
*                     a QUrl pointing to an item in a Qt resource file compiled in the plugin.
*                     Example: qrc:/com.plugin.id/icon.png
*                   - description: this is the information displayed in the plugin list in the
*                     information widget.
*                   - valid: when set to false (default value), a warning icon is displayed in the
*                     application to indicate there is something wrong with this plugin. Once the
*                     required data is initialized in the plugin, this value must be set to true.
*                   - visible: this value is used to show or mask the plugin in the module selection
*                     window. By default, a plugin is hidden.
*
*               To create a PitikappModuleInstance, override createModuleInstance().
***************************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappPlugin
{
    friend class PitikappPrivateData;

    public:
        PitikappPlugin(const QString &id, const QString &name);
        virtual ~PitikappPlugin();

        void registerModuleClass(PitikappModuleInfo *moduleInfo);
        void registerModuleDataClass(const PitikappModuleDataInfo &moduleDataInfo);
        virtual PitikappModuleInstance *createModuleInstance(const QString &moduleId) = 0;

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
        std::unique_ptr<PitikappPluginData> data;
};

/** ************************************************************************************************
* @brief    Get the instance of this plugin.
*
* @details  The returned object must always be the same. The application doesn't take ownership of
*           the pointer.
***************************************************************************************************/
extern "C" PitikappPlugin PITIKAPP_PLUGIN_EXPORT *getPitikappPluginInstance();
