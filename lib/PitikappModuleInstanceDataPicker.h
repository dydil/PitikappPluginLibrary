#pragma once

#include "pluginlib_global.h"

#include "PitikappModuleInstanceData.h"
#include "PitikappModuleInstanceGaugeData.h"
#include "PitikappModuleDataInfo.h"

class PitikappModuleInstanceDataPickerPrivate;

/** ***************************************************************************************
* @class        PitikappModuleInstanceDataPicker
*
* @details      Sometimes, a module needs to use different types of data depending on the
*               user's choice. For example a module can show a gauge in some cases, and
*               a clock in others. This forces to re-implement existing data classes.
*
*               Instead, a PitikappModuleInstanceDataPicker can register a set of data
*               classes and alternate between each.
*
*               Use setActiveInstanceData() to change the data type.
*               Any change to the properties of the current data object is forwarded to
*               the application like if it was the one directly in use.
*               This function needs a path to a widget to edit the parameters, and the name
*               of the data widget to use.
*               In your custom parameter widget, declare a PitikappModulePickerParameterWidget
*               (from com.pitikapp.plugin.widgets 1.0). It will automatically show the
*               correct widget to edit the object set with setActiveInstanceData().
*               In your data widget, use the client parameter 'PitikappPickerCurrentType'
*               to show the correct representation of the data.
*               setActiveInstanceData() requires:
*                - A new instance data object
*                - A path to the parameter widget to use to edit this type of data.
*                  For default types, use the values returned by the PitikappModuleDataInfo
*                  this type.
*                - The value that the client parameter 'PitikappPickerCurrentType' will take
*                  when this type of data is to be displayed.
*
*               setActiveInstanceData() provides overload for default data types.
*
*               loadTo() will copy all information stored in the picker to the given object.
*               This can be used at startup, to reload saved data.
*
*               Each instance data of picker type is different, which means it must use
*               a custom PitikappModuleDataInfo:
*                - Parameter widget: custom widget that uses a PitikappModulePickerParameterWidget.
*                - Data widget: a widget that loads the file returned by
*                - Client files: in addition to all custom files, a data file for each data type
*                  must be provided. The name of this file must be the ID of the matching data type.
*
*               Adding data directly to the picker can conflict with the keys used by
*               the active data object. To prevent this, use a custom prefix for each key.
*               This also applies to helper keys.
*******************************************************************************************/
class PITIKAPP_PLUGIN_EXPORT PitikappModuleInstanceDataPicker : public PitikappModuleInstanceData
{
    public:
        explicit PitikappModuleInstanceDataPicker(const QString &id);
        virtual ~PitikappModuleInstanceDataPicker();

        void loadTo(PitikappModuleInstanceData *data);

        void setActiveInstanceData(PitikappModuleInstanceData *data, const QUrl &parameterWidget);
        void setActiveInstanceData(PitikappModuleInstanceGaugeData *data);

    private:
        std::unique_ptr<PitikappModuleInstanceDataPickerPrivate> data;
};

