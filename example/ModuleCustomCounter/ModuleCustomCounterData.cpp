#include "ModuleCustomCounterData.h"

#include <QByteArray>
#include <QUrl>
#include <QDebug>

namespace
{
    // Class ID
    const QString DATA_CLASS_ID = QStringLiteral("com.pitikapp.plugins.example.module_custom_counter.data");

    // Local parameters
    const QString ParamKeyCounterMaxValue = QStringLiteral("CounterMaxValue");
}

ModuleCustomCounterData::ModuleCustomCounterData() : PitikappModuleInstanceGaugeData(DATA_CLASS_ID)
{
    setLocalParameter(ParamKeyCounterMaxValue, 60);

    setLocalParameterChangeCallback(std::bind(&ModuleCustomCounterData::onLocalParameterChanged, this, std::placeholders::_1, std::placeholders::_2));
}

PitikappModuleDataInfo ModuleCustomCounterData::ModuleDataInfo()
{    
    PitikappModuleDataInfo dataInfo(DATA_CLASS_ID);
    dataInfo.setParameterEditionWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-CustomCounter/Parameters.qml"));
    dataInfo.setClientDataDisplayWidgetPath(PitikappModuleInstanceGaugeData::ModuleDataInfo().getClientDataDisplayWidgetPath());

    return dataInfo;
}

int ModuleCustomCounterData::getCounterMaxValue() const
{
    return getLocalParameter(ParamKeyCounterMaxValue).toInt();
}

void ModuleCustomCounterData::onLocalParameterChanged(const QString &key, const QVariant &value)
{
    if (key == ParamKeyCounterMaxValue)
    {
        emit counterMaxValueChanged(value.toInt());
    }
}
