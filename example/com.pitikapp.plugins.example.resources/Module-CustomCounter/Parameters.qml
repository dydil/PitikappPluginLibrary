import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Window

import com.pitikapp.widgets // For spinbox
import com.pitikapp.plugin.widgets // For gauge parameters

ColumnLayout
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
        id: layout_Main

        anchors.leftMargin: parent.width * 0.025
        anchors.rightMargin: anchors.leftMargin
        anchors.left: parent.left
        anchors.right: parent.right

        spacing: 10

        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: __itemHeight
            Layout.maximumHeight: __itemHeight

            RowLayout
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                height: parent.height
                width: parent.width / 2

                uniformCellSizes: true

                spacing: 10

                Text
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2
                    Layout.maximumWidth: Layout.minimumWidth

                    text: qsTr("Counter maximum value")
                    color: "white"
                    font.weight: Font.Light
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

                PitikappSpinBox
                {
                    id: spinbox_CounterMaxValue

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2

                    from: 1
                    to: 100

                    editable: true

                    onAccepted:
                    {
                        root.forceActiveFocus();
                    }

                    onValueChanged:
                    {
                        if (visible)
                        {
                            moduleLocalParameters.CounterMaxValue = value;
                        }
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
