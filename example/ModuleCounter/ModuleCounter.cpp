#include "ModuleCounter.h"

ModuleCounter::ModuleCounter() : PitikappModuleInstance(&m_data)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout,
                     std::bind(&ModuleCounter::update, this));
    m_updateTimer.start(1000);
    m_counterValue = 0;

    m_data.setParameter(PitikappGaugeParameter_e::MinValue, 0);
    m_data.setParameter(PitikappGaugeParameter_e::MaxValue, 60);
    m_data.setParameter(PitikappGaugeParameter_e::HideUnit, true);
    m_data.setParameter(PitikappGaugeParameter_e::TextFormat, PitikappGaugeDisplayTextFormat_e::eFormat_Absolute);
}

void ModuleCounter::update()
{
    m_counterValue++;
    if (m_counterValue == 60)
    {
        m_counterValue = 0;
        sendNotification("Hello!", 5000);
    }
    m_data.setValue(m_counterValue);
    m_data.setTextValue(QString::number(m_counterValue));
}
