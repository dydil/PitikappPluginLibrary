import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Item {
    readonly property int  __itemHeight: Math.min(Screen.height, Screen.width) / 40

    id: root

    // Alignment values
    enum ListAlignment {
        NoAlignment,
        AlignTop,
        AlignCenter,
        AlignBottom
    }

    // Background
    Rectangle
    {
        anchors.fill: parent

        color: Colors.widgetBackground
    }

    clip: true

    ColumnLayout
    {
        anchors.fill: parent

        spacing: 0

        Item
        {
            Layout.fillHeight: true

            visible: (moduleParameters.Alignment === Data.AlignBottom) || (moduleParameters.Alignment === Data.AlignCenter)
        }

        Repeater
        {
            model: moduleParameters.ProcessList

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: __itemHeight
                Layout.minimumHeight: __itemHeight

                RowLayout
                {
                    anchors.fill: parent

                    // Spacer
                    Item
                    {
                        Layout.fillWidth: true
                        Layout.minimumWidth: __itemHeight / 2
                        Layout.maximumWidth: __itemHeight / 2
                    }

                    // Icon
                    Image
                    {
                        Layout.maximumHeight: __itemHeight
                        Layout.minimumHeight: __itemHeight
                        Layout.maximumWidth: __itemHeight
                        Layout.minimumWidth: __itemHeight

                        source: modelData.Icon
                    }

                    // Name
                    Text
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        visible: moduleParameters.ShowNames

                        text: modelData.Name
                        color: Colors.primaryText
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        elide: Text.ElideRight
                    }

                    // Time
                    Text
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        text: moduleData.ProcessRunTime[modelData.ID]

                        color: Colors.primaryText
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                    // Spacer
                    Item
                    {
                        Layout.fillWidth: true
                        Layout.minimumWidth: __itemHeight / 2
                        Layout.maximumWidth: __itemHeight / 2
                    }
                }

                MouseArea
                {
                    anchors.fill: parent

                    onClicked:
                    {
                        item_KillProcess.askKillProcess(modelData.Name, modelData.ID);
                    }
                }
            }
        }

        Item
        {
            Layout.fillHeight: true

            visible: (moduleParameters.Alignment === Data.AlignTop) || (moduleParameters.Alignment === Data.AlignCenter)
        }
    }

    // Item to show when clicking on a process
    Rectangle
    {
        property string processName: ""
        property string processId: ""

        id: item_KillProcess

        function askKillProcess(name, id)
        {
            processName = name;
            processId = id;

            visible = true;
        }

        anchors.fill: parent

        visible: false
        color: Colors.widgetBackground

        Text
        {
            anchors.top: parent.top
            width: parent.width
            height: parent.height / 2

            text: qsTr("Terminate %1?").arg(item_KillProcess.processName);
            color: Colors.primaryText
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        // Cancel
        Item
        {
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            height: parent.height / 2
            width: parent.width / 2

            Image
            {
                anchors.centerIn: parent

                height: parent.height / 2
                width: parent.width / 2

                source: "icon-cancel.png"
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter

                MouseArea
                {
                    anchors.fill: parent

                    onClicked:
                    {
                        item_KillProcess.visible = false;
                    }
                }
            }

            ColorOverlay
            {
                anchors.fill: parent.children[0]
                source: parent.children[0]
                color: Colors.buttons
            }
        }

        // Confirm
        Item
        {
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            height: parent.height / 2
            width: parent.width / 2

            Image
            {
                anchors.centerIn: parent

                height: parent.height / 2
                width: parent.width / 2

                source: "icon-confirm.png"
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter

                MouseArea
                {
                    anchors.fill: parent

                    onClicked:
                    {
                        moduleData.Terminate = item_KillProcess.processId;
                        item_KillProcess.visible = false;
                    }
                }
            }

            ColorOverlay
            {
                anchors.fill: parent.children[0]
                source: parent.children[0]
                color: Colors.buttons
            }
        }
    }
}
