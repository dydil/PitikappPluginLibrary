import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Item {
    // The height of each line is based on the screen size.
    // Math.min(Window.height, Window.width) is used to make sure an item has the same size whether the device is horizontal or vertical.
    // Also, if only the height is used it could look too small when the device is horizontal.
    readonly property int  __itemHeight: Math.min(Window.height, Window.width) / 10

    id: root

    // ------------------------------------------------------------------
    // Alignment values.
    // The enum must match ProcessAlignment in the data class definition.
    // ------------------------------------------------------------------
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

        // -------------------------------------------------------------------------------------------
        // Using one of the predefined colors, the background color is automatically updated when dark
        // theme is enabled or disabled. It also ensure all modules have a similar appearance.
        // -------------------------------------------------------------------------------------------
        color: Colors.widgetBackground
    }

    clip: true

    ColumnLayout
    {
        anchors.fill: parent

        spacing: 0

        // ------------------------------------------------------------------------
        // Spacer to align according to the "Alignment" parameter.
        // ------------------------------------------------------------------------
        Item
        {
            Layout.fillHeight: true

            visible: (moduleParameters.Alignment === Data.AlignBottom) || (moduleParameters.Alignment === Data.AlignCenter)
        }

        // ------------------------------------------------------------------------
        // Each item of the ProcessList parameter is inserted into a column layout.
        // ------------------------------------------------------------------------
        Repeater
        {
            // ------------------------------------------------------------------------
            // The model is provided by PitikappModuleInstanceData::setClientParameter.
            // ------------------------------------------------------------------------
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
                    // To let a margin on the left.
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

                        // --------------------------------------------------------------------
                        // The source is a base64 encoded image.
                        // See convertImageToDisplayableBase64() in ProcessSelectionHelper.cpp.
                        // For now it is not possible to transfer images so this is the standard
                        // way. It is also possible to use URLs.
                        // --------------------------------------------------------------------
                        source: modelData.Icon
                    }

                    // Name
                    Text
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        // ---------------------------------------------------------------------------
                        // Parameters are defined with PitikappModuleInstanceData::setClientParameter.
                        // They can directly be bound to widget properties.
                        // ---------------------------------------------------------------------------
                        visible: moduleParameters.ShowNames

                        // ----------------------------------------------------------------------------
                        // Using one of the predefined colors, the text color is automatically updated
                        // depending on the device configuration. It also ensures all the modules have
                        // a similar appearance.
                        // ----------------------------------------------------------------------------
                        color: Colors.primaryText

                        text: modelData.Name
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        elide: Text.ElideRight
                        font.pixelSize: __itemHeight / 2
                    }

                    // Time
                    Text
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        // ------------------------------------------------------------------------------------------------
                        // This value is defined using PitikappModuleInstanceData::setClientData.
                        // Note that a QVariantMap doesn't notify when one of its values changes, so the whole map
                        // is updated every time.
                        // The map keys can be accessed like a regular map and the value can be bound to widget properties.
                        // ------------------------------------------------------------------------------------------------
                        text: moduleData.ProcessRunTime[modelData.ID]

                        color: Colors.primaryText
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: __itemHeight / 2
                    }

                    // Spacer
                    // To let a margin on the right.
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

        // ------------------------------------------------------------------------
        // Spacer to align according to the "Alignment" parameter.
        // ------------------------------------------------------------------------
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

            // --------------------------------------------------------------------------------------------------------------
            // We want the button color to be consistent when the dark theme is used.
            // Instead of having two icons, use a ColorOverlay applied to the button  and using one of the predefined colors.
            // Colors.buttons is automatically updated when dark theme is enabled or disabled.
            // --------------------------------------------------------------------------------------------------------------
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
                        // ---------------------------------------------------------------------------------
                        // Setting the value of a moduleData key instead of reading it will trigger a call
                        // to PitikappModuleInstance::processData which can be overriden in a custom module.
                        // ---------------------------------------------------------------------------------
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
