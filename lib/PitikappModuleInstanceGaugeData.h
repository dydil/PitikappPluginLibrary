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
* @enum         PitikappGaugeValueColorType_e
*
* @brief        The color type of the value text.
*******************************************************************************************/
enum class PitikappGaugeValueColorType_e
{
    Default,
    Custom
};
Q_DECLARE_METATYPE(PitikappGaugeValueColorType_e)

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
* @enum         PitikappGaugeUnitDisplayMode_e
*
* @brief        What to display for the unit.
*******************************************************************************************/
enum class PitikappGaugeUnitDisplayMode_e
{
    Default, /// As provided by the module.
    Custom,  /// Overwritten by the user.
    None,    /// Do not show any unit.
};
Q_DECLARE_METATYPE(PitikappGaugeUnitDisplayMode_e)

/** ***************************************************************************************
* @enum         PitikappGaugeUnitVerticalAnchor_e
*
* @brief        How to vertically anchor the unit.
*******************************************************************************************/
enum class PitikappGaugeUnitVerticalAnchor_e
{
    Top,
    Center,
    Bottom
};
Q_DECLARE_METATYPE(PitikappGaugeUnitVerticalAnchor_e)

/** ***************************************************************************************
* @enum         PitikappGaugeUnitHorizontalAnchor_e
*
* @brief        How to horizontally anchor the unit.
*******************************************************************************************/
enum class PitikappGaugeUnitHorizontalAnchor_e
{
    Left,
    Center,
    Right
};
Q_DECLARE_METATYPE(PitikappGaugeUnitHorizontalAnchor_e)

/** ***************************************************************************************
* @enum         PitikappGaugeParameter_e
*
* @brief        All client parameters of a Pitikapp gauge.
*******************************************************************************************/
enum class PitikappGaugeParameter_e
{
    // Gauge
    GaugeType,         // Type: PitikappGaugeType_e                 - Default: eGaugeType_Circle
    BarDirection,      // Type: PitikappBarGaugeDirection_e         - Default: eBarGaugeDirection_Top
    DefaultColor,      // Type: QColor                              - Default: black
    BackgroundType,    // Type: PitikappGaugeBackgroundType_e       - Default: Default
    BackgroundColor,   // Type: QColor                              - Default: white
    BarWidth,          // Type: int (%)                             - Default: 30

    // Value
    TextFormat,        // Type: PitikappGaugeDisplayTextFormat_e    - Default: eFormat_Percent
    TextColorType,     // Type: PitikappGaugeValueColorType_e       - Default: Default
    TextColor,         // Type: QColor                              - Default: black
    ValuePosition,     // Type: PitikappGaugeValuePosition_e        - Default: eValuePos_Center
    ValueHeight,       // Type: int                                 - Default: 20
    ValueWidth,        // Type: int                                 - Default: 20
    MinValue,          // Type: double                              - Default: 0
    MaxValue,          // Type: double                              - Default: 100

    // Unit
    UnitDisplayMode,   // Type: PitikappGaugeUnitDisplayMode_e      - Default: Default
    CustomUnit,        // Type: QString                             - Default: ""
    UnitHeight,        // Type: int (%)                             - Default: 30
    VerticalAnchor,    // Type: PitikappGaugeUnitVerticalAnchor_e   - Default: Top
    HorizontalAnchor,  // Type: PitikappGaugeUnitHorizontalAnchor_e - Default: Right

    // Thresholds
    Warning1Enabled,   // Type: bool                                - Default: false
    Warning1Threshold, // Type: int                                 - Default: 60
    Warning1Color,     // Type: QColor                              - Default: orange
    Warning2Enabled,   // Type: bool                                - Default: false
    Warning2Threshold, // Type: int                                 - Default: 80
    Warning2Color,     // Type: QColor                              - Default: red
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
        void setRangeSliderModifiable(bool modifiable);
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
        void setValue(double value);
        void setValuePercent(int value);
        void setTextValue(const QString &textValue);
        void setUnit(const QString &unit);

        void setParameter(PitikappGaugeParameter_e param, const QVariant &value);
};
