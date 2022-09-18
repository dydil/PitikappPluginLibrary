#include "ModuleCounter.h"

ModuleCounter::ModuleCounter() : PitikappModuleInstance(&m_data)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout,
                     std::bind(&ModuleCounter::update, this));
    m_updateTimer.start(1000);
    m_counterValue = 0;

    // --------------------------------------------------------------------------------------------
    // Set custom default values
    // See the definition of PitikappGaugeParameter_e for the default values of all the parameters.
    // By setting the min to 0 and max to 60, there is no need to calculate a progress.
    // It is done manually when setting a value between 0 and 60.
    // --------------------------------------------------------------------------------------------
    m_data.setParameter(PitikappGaugeParameter_e::MinValue, 0);
    m_data.setParameter(PitikappGaugeParameter_e::MaxValue, 60);
    m_data.setParameter(PitikappGaugeParameter_e::UnitDisplayMode, PitikappGaugeUnitDisplayMode_e::None);
    m_data.setParameter(PitikappGaugeParameter_e::TextFormat, PitikappGaugeDisplayTextFormat_e::eFormat_Absolute);
}

void ModuleCounter::update()
{
    m_counterValue++;
    if (m_counterValue == 60)
    {
        m_counterValue = 0;

        // ------------------------------------------------------------------
        // When the counter reaches the end, show a message for 5 seconds.
        // ------------------------------------------------------------------
        sendNotification("Hello!", 5000);
    }

    // ----------------------------------------------------------------------
    // This value is used to calculate which percentage of the gauge to fill.
    // ----------------------------------------------------------------------
    m_data.setValue(m_counterValue);

    // ----------------------------------------------------------------------
    // This value is the one displayed on the gauge.
    // Here it is the same as the counter value but it can be any value.
    // ----------------------------------------------------------------------
    m_data.setTextValue(QString::number(m_counterValue));

    // ----------------------------------------------------------------------
    // To display a unit, use m_data.setUnit(XXX);
    // No unit is used here.
    // ----------------------------------------------------------------------
}
