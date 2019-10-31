import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import com.pitikapp.plugin.widgets 1.0

Item
{
    readonly property int  __itemHeight: Math.min(Screen.height, Screen.width) / 40

    id: root

    function startEdition()
    {
        spinbox_CounterMaxValue.value = moduleLocalParameters.CounterMaxValue;
        widget_Gauge.startEdition();
    }

    function endEdition()
    {
        widget_Gauge.endEdition();
    }

    ColumnLayout
    {
        anchors.fill: parent

        ColumnLayout
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: __itemHeight * 2

            Text
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: __itemHeight

                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Counter maximum value")
                color: "white"
            }

            SpinBox
            {
                id: spinbox_CounterMaxValue

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: __itemHeight
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: root.width / 2

                onValueChanged:
                {
                    if (visible)
                    {
                        moduleLocalParameters.CounterMaxValue = value;
                    }
                }
            }
        }

        PitikappGaugeWidget
        {
            id: widget_Gauge

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
