#include "ModuleCustomCounter.h"

ModuleCustomCounter::ModuleCustomCounter() : PitikappModuleInstance(&m_data)
{
    connect(&m_updateTimer, &QTimer::timeout, this, &ModuleCustomCounter::update);
    m_updateTimer.start(1000);
    m_counterValue = 0;

    connect(&m_data, &ModuleCustomCounterData::counterMaxValueChanged, this, &ModuleCustomCounter::onCounterMaxValueChanged);

    // Set custom default values
    m_data.setParameter(PitikappGaugeParameter_e::MinValue, 0);
    m_data.setParameter(PitikappGaugeParameter_e::MaxValue, m_data.getCounterMaxValue());
    m_data.setParameter(PitikappGaugeParameter_e::HideUnit, true);
    m_data.setParameter(PitikappGaugeParameter_e::TextFormat, PitikappGaugeDisplayTextFormat_e::eFormat_Absolute);
}

void ModuleCustomCounter::onCounterMaxValueChanged(int value)
{
    m_data.setParameter(PitikappGaugeParameter_e::MaxValue, value);

    if (m_counterValue > m_data.getCounterMaxValue())
    {
        m_counterValue = 0;
        sendNotification("Hello!", 5000);
    }
}

void ModuleCustomCounter::update()
{
    m_counterValue++;
    if (m_counterValue > m_data.getCounterMaxValue())
    {
        m_counterValue = 0;
        sendNotification("Hello!", 5000);
    }

    m_data.setValue(m_counterValue);
    m_data.setTextValue(QString::number(m_counterValue));
}
