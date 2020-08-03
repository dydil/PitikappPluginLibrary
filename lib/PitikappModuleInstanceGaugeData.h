#pragma once

#include "pluginlib_global.h"

#include "PitikappModuleInstanceData.h"
#include "PitikappModuleDataInfo.h"

/** ***************************************************************************************
* @enum         GaugeDisplayTextFormat_e
*
* @brief        Gauge text value format to show.
*******************************************************************************************/
enum class PitikappGaugeDisplayTextFormat_e
{
    eFormat_Percent,   /// Value as percent between min and max.
    eFormat_Absolute,  /// Absolute value.
    eFormat_NoText,    /// Do not display any value.
};
Q_DECLARE_METATYPE(PitikappGaugeDisplayTextFormat_e)

/** ***************************************************************************************
* @enum         GaugeType_e
*
* @brief        Type of gauge to display.
*******************************************************************************************/
enum class PitikappGaugeType_e
{
    eGaugeType_Bar,    /// Show a horizontal or vertical bar.
    eGaugeType_Circle, /// Show a circle.
    eGaugeType_None,   /// Show only the text.
};
Q_DECLARE_METATYPE(PitikappGaugeType_e)

/** ***************************************************************************************
* @enum         PitikappBarGaugeOrientation_e
*
* @brief        Direction of a bar gauge.
*******************************************************************************************/
enum class PitikappBarGaugeDirection_e
{
    eBarGaugeDirection_Top,
    eBarGaugeDirection_Right,
    eBarGaugeDirection_Bottom,
    eBarGaugeDirection_Left,
};
Q_DECLARE_METATYPE(PitikappBarGaugeDirection_e)

/** ***************************************************************************************
* @enum         GaugeValuePosition_e
*
* @brief        The position of the value text.
*******************************************************************************************/
enum class PitikappGaugeValuePosition_e
{
    eValuePos_Top,
    eValuePos_Center,
    eValuePos_Bottom,
    eValuePos_Left,
    eValuePos_Right,
};
Q_DECLARE_METATYPE(PitikappGaugeValuePosition_e)

/** ***************************************************************************************
* @enum         PitikappGaugeBackgroundType_e
*
* @brief        The type of background (the empty part of the gauge).
*******************************************************************************************/
enum class PitikappGaugeBackgroundType_e
{
    None,        /// No background. Only the filled part of the gauge is visible.
    Default,     /// Automatic color depending on the theme (clear or dark).
    CustomColor, /// User selected color.
};
Q_DECLARE_METATYPE(PitikappGaugeBackgroundType_e)

/** ***************************************************************************************
* @enum         PitikappGaugeParameter_e
*
* @brief        All client parameters of a Pitikapp gauge.
*******************************************************************************************/
enum class PitikappGaugeParameter_e
{
    GaugeType,         // Type: PitikappGaugeType_e              - Default: eGaugeType_Circle
    BarDirection,      // Type: PitikappBarGaugeDirection_e      - Default: eBarGaugeDirection_Top
    HideUnit,          // Type: bool                             - Default: false
    TextFormat,        // Type: PitikappGaugeDisplayTextFormat_e - Default: eFormat_Percent
    DefaultColor,      // Type: QColor                           - Default: black
    BackgroundType,    // Type: PitikappGaugeBackgroundType_e    - Default: Default
    BackgroundColor,   // Type: QColor                           - Default: white
    BarWidth,          // Type: int (%)                          - Default: 30
    ValuePosition,     // Type: PitikappGaugeValuePosition_e     - Default: eValuePos_Center
    ValueHeight,       // Type: int                              - Default: 20
    ValueWidth,        // Type: int                              - Default: 20
    MinValue,          // Type: int                              - Default: 0
    MaxValue,          // Type: int                              - Default: 100
    Warning1Enabled,   // Type: bool                             - Default: false
    Warning1Threshold, // Type: int                              - Default: 60
    Warning1Color,     // Type: QColor                           - Default: orange
    Warning2Enabled,   // Type: bool                             - Default: false
    Warning2Threshold, // Type: int                              - Default: 60
    Warning2Color,     // Type: QColor                           - Default: red
};

class PITIKAPP_PLUGIN_EXPORT PitikappModuleInstanceGaugeData : public PitikappModuleInstanceData
{
    public:
        PitikappModuleInstanceGaugeData();
        explicit PitikappModuleInstanceGaugeData(const QString &id);
        ~PitikappModuleInstanceGaugeData() override = default;

        static PitikappModuleDataInfo ModuleDataInfo();

        // ---------------------
        // Local parameters
        // ---------------------
        void setRangeModifiable(bool modifiable);
        void setRangeMin(int min);
        void setRangeMax(int max);

        // ---------------------
        // Client parameters
        // ---------------------
        // To avoid needing to register enums stream operators, use two functions.
        // One that takes any parameter but an enumeration, and one that takes an enumeration
        // and converts it to an int.
        template <typename T, typename U = std::enable_if_t<!std::is_enum<T>::value>>
        void setParameter(PitikappGaugeParameter_e param, const T &value)
        {
            setParameter(param, QVariant::fromValue(value));
        }
        template <typename T, typename U = std::enable_if_t<std::is_enum<T>::value>>
        void setParameter(PitikappGaugeParameter_e param, T value)
        {
            setParameter(param, QVariant::fromValue(static_cast<int>(value)));
        }
        QVariant getParameter(PitikappGaugeParameter_e param, const QVariant &defaultValue = QVariant{});

        // ---------------------
        // Client data
        // ---------------------
        void setValue(int value);
        void setTextValue(const QString &textValue);
        void setUnit(const QString &unit);

    private:
        void setParameter(PitikappGaugeParameter_e param, const QVariant &value);
};
