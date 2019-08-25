import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import Qt.labs.platform 1.0
import QtQuick.Window 2.3


Item
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
        anchors.fill: parent

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
        Row
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: __itemHeight
            Layout.minimumHeight: __itemHeight

            spacing: 0

            Text
            {
                height: parent.height
                width: parent.width / 2

                text: qsTr("Show names")
                color: "white"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            RowLayout
            {
                height: parent.height
                width: parent.width / 2

                CheckBox
                {
                    id: checkbox_ShowNames

                    onCheckedChanged:
                    {
                        moduleClientParameters.ShowNames = checked;
                    }
                }

                Item
                {
                    Layout.fillWidth: true
                }
            }
        }

        // Alignment
        Row
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: __itemHeight
            Layout.minimumHeight: __itemHeight

            spacing: 0

            Text
            {
                height: parent.height
                width: parent.width / 2

                text: qsTr("Alignment")
                color: "white"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            RowLayout
            {
                height: parent.height
                width: parent.width / 2

                Layout.leftMargin: width * 0.1

                Item
                {
                    Layout.fillWidth: true
                    Layout.maximumWidth: __itemHeight / 8
                }

                ComboBox
                {
                    id: combobox_Alignment

                    model: [qsTr("None"), qsTr("Top"), qsTr("Center"), qsTr("Bottom")]

                    onCurrentIndexChanged:
                    {
                        if (__ready)
                        {
                            moduleClientParameters.Alignment = currentIndex;
                        }
                    }
                }

                Item
                {
                    Layout.fillWidth: true
                }
            }
        }

        // Align to the top
        Item { Layout.fillHeight: true}
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
