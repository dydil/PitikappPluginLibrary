import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import Qt.labs.platform
import QtQuick.Window

// Import this to use basic widgets that have the same look and feel everywhere (checkbox, combobox, ...)
import com.pitikapp.widgets

ColumnLayout
{
             property bool __ready     : false
    readonly property int  __itemHeight: Math.min(Screen.height, Screen.width) / 40

    id: root

    function startEdition()
    {
        checkbox_ShowNames.checked = moduleClientParameters.ShowNames;
        combobox_Alignment.currentIndex = moduleClientParameters.Alignment;

        __ready = true;
    }

    function endEdition()
    {
        __ready = false;
    }

    ColumnLayout
    {
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width * 0.95
        Layout.alignment: Qt.AlignHCenter

        spacing: 5

        ColumnLayout
        {
            spacing: 0
            Repeater
            {
                model: moduleClientParameters.ProcessList

                Item
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumHeight: __itemHeight
                    Layout.minimumHeight: __itemHeight

                    // To highlight a line
                    MouseArea
                    {
                        id: mousearea_ProcessHover

                        anchors.fill: parent

                        hoverEnabled: true

                        Rectangle
                        {
                            anchors.fill: parent

                            opacity: parent.containsMouse? 0.5 : 0
                        }

                        RowLayout
                        {
                            anchors.fill: parent

                            // Process icon
                            Image
                            {
                                Layout.maximumHeight: __itemHeight
                                Layout.minimumHeight: __itemHeight
                                Layout.maximumWidth: __itemHeight
                                Layout.minimumWidth: __itemHeight

                                source: modelData.Icon
                            }

                            // Process name
                            TextField
                            {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                text: modelData.Name
                                verticalAlignment: Text.AlignVCenter
                                color: mousearea_ProcessHover.containsMouse? "black" : "white"
                                selectByMouse: true
                                background: Rectangle
                                {
                                    color: "transparent"
                                }

                                onEditingFinished:
                                {
                                    root.forceActiveFocus();
                                    moduleHelpers.ProcessSelection.processNameChanged(modelData.ID, text)
                                }
                            }

                            // Delete button
                            Image
                            {
                                Layout.maximumHeight: __itemHeight
                                Layout.minimumHeight: __itemHeight
                                Layout.maximumWidth: __itemHeight
                                Layout.minimumWidth: __itemHeight

                                source: "qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/icon-delete.png"
                                fillMode: Image.PreserveAspectFit
                                horizontalAlignment: Image.AlignHCenter
                                verticalAlignment: Image.AlignVCenter

                                MouseArea
                                {
                                    anchors.fill: parent

                                    propagateComposedEvents: true
                                    hoverEnabled: true

                                    onClicked:
                                    {
                                        moduleHelpers.ProcessSelection.processRemoved(modelData.ID)
                                    }

                                    ToolTip.text: qsTr("Remove")
                                    ToolTip.visible: containsMouse
                                }
                            }
                        }
                    }
                }
            }
        }

        Button
        {
            Layout.alignment: Qt.AlignHCenter

            text: qsTr("Add Process")

            onClicked:
            {
                filedialog_ProcessSelect.open();
            }
        }

        // Show Names
        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: __itemHeight
            Layout.minimumHeight: __itemHeight

            RowLayout
            {
                anchors.fill: parent

                spacing: 10

                Item
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2
                    Layout.maximumWidth: (parent.width - parent.spacing) / 2

                    Text
                    {
                        anchors.fill: parent
                        text: qsTr("Show names")
                        color: "white"
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 13
                    }
                }

                Item
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2
                    Layout.maximumWidth: (parent.width - parent.spacing) / 2

                    PitikappCheckBox
                    {
                        id: checkbox_ShowNames

                        height: parent.height
                        width: height

                        onCheckedChanged:
                        {
                            moduleClientParameters.ShowNames = checked;
                        }
                    }
                }
            }
        }

        // Alignment
        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: __itemHeight
            Layout.minimumHeight: __itemHeight

            RowLayout
            {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height

                spacing: 10

                Text
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2
                    Layout.maximumWidth: Layout.minimumWidth

                    text: qsTr("Alignment")
                    color: "white"
                    font.weight: Font.Light
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

                PitikappCombobox
                {
                    id: combobox_Alignment

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: (parent.width - parent.spacing) / 2
                    Layout.maximumWidth: Layout.minimumWidth

                    model: [qsTr("None"), qsTr("Top"), qsTr("Center"), qsTr("Bottom")]

                    onCurrentIndexChanged:
                    {
                        if (__ready)
                        {
                            moduleClientParameters.Alignment = currentIndex;
                        }
                    }
                }
            }
        }
    }

    FileDialog
    {
        id: filedialog_ProcessSelect

        onAccepted:
        {
            moduleHelpers.ProcessSelection.selectProcess(Qt.resolvedUrl(file));
        }
    }
}
