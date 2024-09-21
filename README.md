# Pitikapp Plugin Library
This library can be used to create a plugin for Pitikapp.
For more information about Pitikapp, visit [pitikapp.com].

[pitikapp.com]: <https://www.pitikapp.com>

# Who is this guide for?

Anyone with basic programming knowledge in C++ and Qt/QML can create a plugin for Pitikapp.


# Definitions

Pitikapp can load user created libraries.

Each library is called a `plugin`.
All loaded plugins are listed either in the information window or at the left of the screen:

![N|Solid](https://pitikapp.com/images/plugins/pluginList-Information.png)

![N|Solid](https://pitikapp.com/images/plugins/pluginList-AddModule.png)

Each plugin is made of one or more `modules`.
The modules of a plugin are listed either in the information window or at the left of the screen.

![N|Solid](https://pitikapp.com/images/plugins/moduleList-Information.png)

![N|Solid](https://pitikapp.com/images/plugins/moduleList-AddModule.png)

# Requirements

A plugin must be compiled using Qt 5.15.2 MSVC 2019 64 bits.

This version of the library is intented to be used with Pitikapp 1.5.x.

# Global architecture

A plugin is represented by the abstract class `PitikappPlugin`. Each plugin class must have a unique id (e.g. "com.pitikapp.plugins.example").

The meta-data of a module is represented by the class `PitikappModuleInfo`. A list of `PitikappModuleInfo` can be attached to a `PitikappPlugin` object. This list is used to show all the modules available for a plugin.

`PitikappModuleInstance` represents the actual module. It is created upon request from the user.

`PitikappModuleInstanceData` stores all the data necessary for the use of a `PitikappModuleInstance`. This includes the parameters that the user can modify as well as the data that must be displayed in a client.

Each `PitikappModuleInstance` object requires an instance of `PitikappModuleInstanceData`.

Each module can use:
* A widget for the user to modify the parameters (colors, sizes, ...).
* A widget to show data on the client application.
* A list of files that can be used in the client (e.g. images).

All this information is stored in a `PitikappModuleDataInfo` object. A plugin can register several `PitikappModuleDataInfo` types as long as each has a unique ID. All resources (QML files, images, ...) of a `PitikappModuleDataInfo` must be embedded in a Qt resource file.

Each `PitikappModuleInstanceData` takes as constructor the ID of a `PitikappModuleDataInfo` that is registered by the plugin.

A module can also use one of the predefined `PitikappModuleInstanceData` classes. For now there is only one available: `PitikappModuleInstanceGaugeData`.
By using `PitikappModuleInstanceGaugeData` or other predefined classes, a plugin doesn't need to register its own `PitikappModuleDataInfo` type since everything is included.


# How it works

At startup, Pitikapp looks for a shared library file called `plugin.dll` in each sub-directory of `./plugins`.

If the file exists and is a valid shared library it will try to load and call a function `getPitikappPluginInstance()` that returns a pointer to an object of type `PitikappPlugin`.

If the file cannot be loaded (for example incompatible architecture or missing dependencies) or if the `getPitikappPluginInstance` symbol is not found, the plugin is ignored and won't be listed anywhere.

# Example

The attached demo project shows the all the possibilities offered by the plugin library.


## 1 Creation of a Pitikapp plugin class

`PitikappPlugin` is an abstract class. `createModuleInstance` must be overriden in a subclass.
In the example, this class is called `PitikappExamplePlugin`.

The constructor of `PitikappPlugin` takes two parameters:
* The ID of the plugin (must be unique globally)
* The name of the plugin. This name must be short to fit in all the screens.
```cpp
PitikappExamplePlugin::PitikappExamplePlugin() : PitikappPlugin("com.pitikapp.plugins.example", "Example Plugin")
```
The constructor of the plugin object must initialize the meta-data for all the modules.

### 1.1 Plugin meta-data

```cpp
setValid(true);
setVisible(true);
setDescription("This is an example plugin.");
setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/PluginIcon.png"));
```

An invalid plugin will display a warning sign.

An invisible plugin will not be displayed in the module selection window but still displayed in the information window.

The description text is displayed when selecting the module in the information window.

The icon is used in the module selection window and the information window.

All those properties can be changed at any time and the displayed data will be updated in real time.

### 1.2 Module data classes

```cpp
registerModuleDataClass(ModuleProcessRunTimeData::ModuleDataInfo());
```

Register all the data classes that will be used by the modules of this plugin.

A data class needs to be registered only once in the entire application.
Most default Pitikapp plugins already use `PitikappModuleInstanceGaugeData` so it doesn't need to be registered, but it won't hurt if it is registered twice.

### 1.3 Declaration of module meta-data

In the example, two modules are static and one is dynamic. They are declared this way:

```cpp
PitikappModuleInfo m_processRunTimeModuleInfo;
PitikappModuleInfo m_counterModuleInfo;
std::map<QString, std::pair<PitikappModuleInfo, QString>> m_storageModuleInfo;

[...]

registerModuleClass(&m_processRunTimeModuleInfo);
registerModuleClass(&m_counterModuleInfo);

```

Note that meta-data must remain valid until the program is shut down. If it is deleted, the module won't be available anymore.

An invalid module will display a warning sign.

An invisible module will not be displayed in the module selection window but still displayed in the information window.

All those properties can be changed at any time and the displayed data will be updated in real time.

### 1.4 Creation of a module instance

```cpp
PitikappModuleInstance *createModuleInstance(const QString &moduleId) override final;
```

This is the function that will be called when the user picks a type in the module selection window.

The plugin must return a `PitikappModuleInstance` for the given ID. If the ID is invalid, a null pointer can be returned.

```cpp
PitikappModuleInstance *PitikappExamplePlugin::createModuleInstance(const QString &moduleId)
{
    if (MODULE_ID_PROCESS_RUN_TIME == moduleId)
    {
        return new ModuleProcessRunTime;
    }
    else if (MODULE_ID_COUNTER == moduleId)
    {
        return new ModuleCounter;
    }

    return nullptr;
}
```

### 1.4 Make the plugin available to the application

The application will look for a symbol `getPitikappPluginInstance` in the loaded library. It will be called once at startup.

The function must return a pointer to a `PitikappPlugin`.

Note: the application does not take ownership of the pointer. It must remain valid while the application is active.

```cpp
extern "C" PitikappPlugin *getPitikappPluginInstance()
{
    if (!Instance)
    {
        Instance = new PitikappExamplePlugin();
    }

    return Instance;
}
```

## 2 Creation of a Pitikapp module class

A module is composed of several classes:
* `PitikappModuleInstance`

   An object of this type will be created upon request from the user.
* `PitikappModuleInstanceData`

  Each `PitikappModuleInstance` object requires an instance of `PitikappModuleInstanceData`.

  This is the class that stores and display any kind of information and allows the user to modify the parameters. It can can be used by several modules.

  There are some pre-defined instance data classes. For example, to have a gauge using the same style as the default Pitikapp plugins, `PitikappModuleInstanceGaugeData` can be used.

* `PitikappModuleDataInfo`

  Each `PitikappModuleInstanceData` is linked to a `PitikappModuleDataInfo` class.
  It will be used by the application to show the correct widgets when necessary.


### 2.1 Simple module using a predefined data type


`ModuleCounter` is a simple module that shows a counter from 0 to 60 and sends a notification when reaching 60.
It uses `PitikappModuleInstanceGaugeData` as data type.

Default parameters:

```cpp
m_data.setParameter(PitikappGaugeParameter_e::MinValue, 0);
m_data.setParameter(PitikappGaugeParameter_e::MaxValue, 60);
m_data.setParameter(PitikappGaugeParameter_e::HideUnit, true);
m_data.setParameter(PitikappGaugeParameter_e::TextFormat, PitikappGaugeDisplayTextFormat_e::eFormat_Absolute);
```

The minimum value is 0 and the max 60. This allows using directly the value of the counter.

```cpp
m_data.setValue(m_counterValue);
```

The other option is to leave the default [0-100] and calculate the progress based on the value of the counter.

The value that is shown in the gauge is set by `setTextValue`.
```cpp
m_data.setTextValue(QString::number(m_counterValue));
```

When the counter reaches 60, a notification is sent. This shows the text "Hello!" for 5 seconds.
```cpp
sendNotification("Hello!", 5000);
```

### 2.2 Dynamic modules

`DiskUsage` is a simple module that shows the used disk space. The plugin shows one module for each disk.

See `PitikappExamplePlugin::updateStorageList` for more information.

### 2.3 Custom module data

`ModuleProcessRunTime` uses a custom data type: `ModuleProcessRunTimeData`.

It allows the user to select executables to watch. The client shows for how much time each process has been running and the user can press on an item to kill the process.

This section explains how to get there.

The module is not perfect, it is just mean to demonstrate the possibilities. For example if there are multiple instances of a program running, the first one encountered in the process list will be used.

#### 2.3.1 PitikappModuleDataInfo

`ModuleProcessRunTimeData::ModuleDataInfo` defines the files used by `ModuleProcessRunTimeData`.

```cpp
PitikappModuleDataInfo info(DATA_CLASS_ID);
```

The ID must be unique. The same ID must be passed to the constructor of `ModuleProcessRunTimeData`.

```cpp
info.setClientDataDisplayWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Data.qml"));
```

This is the file that will be used in the client application to display data to the user.

```cpp
info.setParameterEditionWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Parameters.qml"));
```

This is the file that will be used in the server application to allow the user to modify the parameters of the module instance.

```cpp
info.addClientFile(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/icon-confirm.png"));
info.addClientFile(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/icon-cancel.png"));
```

Those two images are used in the `Data.qml` widget. See below for how to use them.

Finally, the plugin must register this information.

```cpp
registerModuleDataClass(ModuleProcessRunTimeData::ModuleDataInfo());
```

#### 2.3.2 PitikappModuleInstanceData

`ModuleProcessRunTimeData` translates data in a format that is usable by the `Data` and `Parameters` files (refered to below as "data widget" and "parameter widget").

It attaches a value to a key. The following types of values exist:
* Client parameters

  Those values are sent to the client and affect the way the information is displayed. They are changed by the user from the parameter widget.

  Example: color of a text.

  A client parameter is defined using `PitikappModuleInstanceData::setClientParameter`.
* Local parameters

  Those parameters are not used in the client. They are saved locally only.

  Example: the token to login to a service (like Spotify).

  A local parameter is defined using `PitikappModuleInstanceData::setLocalParameter`.
* Client data

  These are the values that are displayed in the data widget. They are usually changed from the `PitikappModuleInstance` when some events are triggered (like a refresh timer).

  Example: the elapsed time of a process.

  A client data value is defined using `PitikappModuleInstanceData::setClientData`.

It is possible to set a callback that will be triggered whenever any of the above values is modified.

The callback must take as argument a `QString` (the item key) and a `QVariant` (the item value). It is registered using:
* `PitikappModuleInstanceData::setClientParameterChangeCallback`
* `PitikappModuleInstanceData::setLocalParameterChangeCallback`
* `PitikappModuleInstanceData::setClientDataChangeCallback`

#### 2.3.3 Data storage

All the values mentionned above are stored as a `QVariant` attached to a `QString` key. Custom values type must respect the rules of a `QVariant` when necessary:
* Declared with `Q_DECLARE_METATYPE`
* Registered with `qRegisterMetaType`
* Registered stream operators with `qRegisterMetaTypeStreamOperators`
* Registered comparison operators with `QMetaType::registerComparators`

The keys must be valid OS file names. It means no special characters. If a special character needs to be used, the key can for example be converted to base64 before use.
Data is stored in a binary format but can be read by any user. Critical information should be encrypted before. Note that a CRC is used, which prevents from manually modifying the data.

#### 2.3.4 Parameter widget

This widget lets the user configure how to display data. It is defined in the declaration of a `PitikappModuleDataInfo`.

```cpp
info.setParameterEditionWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Parameters.qml"));
```

The parameter widget has access to the following objects:
* moduleClientParameters

  This is a `QQmlPropertyMap`. Setting any value of this object will propagate it to the client.

  Note that `QQmlPropertyMap` doesn't require the key to already exist. Calling `moduleClientParameters.Key = value` is all that is needed.
* moduleLocalParameters

  This is also a `QQmlPropertyMap`. The key-value pairs assigned to this object are stored locally. They are never propagated to the client.
* moduleHelpers

  This object gives access to all the custom defined `helper` objects.

  A helper is a `QObject` that is registered using `PitikappModuleInstanceData::addHelper`.
  It is meant to allow calling complex operations that are not possible using only QML.
  In the example, `ProcessSelectionHelper` is a helper that reads a path to extract the icon of the selected file. The `selectProcess` function can be called from QML since it is declared `Q_INVOKABLE`.

Before showing the widget, the application calls the method `startEdition` (if it exists). When this function is called, it means all the objects mentionned above are ready to be used. The widget can be initialized with the correct values.
It is also possible to use the `onVisibleChanged` method.

Before hiding the widget, the application calls the method `endEdition`.

In the example, the widget uses those functions to set a flag to tell if the values are ready to be used.

**Important note**: some properties can change when the widget is hidden so it is important to ensure the widget is visible before changing a property.

In the example, the `onCurrentIndexChanged` method of `combobox_Alignment` is called with value 0 when the widget is being hidden. Without checking `if (__ready)` first, the alignment always returns to 0 (none) when hiding.

**Parameter widget layout**:

Parameter widgets are layed out automatically. For a better experience, the following guidelines should be used when designing a parameter widget:
- The root item of any parameter widget must be a `ColumnLayout` with no `Layout.fillWidth` or `Layout.fillHeight` values.
- Child elements should be grouped in layouts with no height hint and `Layout.fillWidth: true`. Some margins can be added where necessary.
- Each individual item should have a fixed height. It can be in pixels, or proportional to the screen size: `readonly property int itemHeight: Math.min(Screen.height, Screen.width) / 40`.

#### 2.3.5 Data widget

This widget shows information to the user. It is defined in the declaration of a `PitikappModuleDataInfo`.

```cpp
info.setClientDataDisplayWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Data.qml"));
```

The data widget has access to the following objects:
* moduleParameters

  This is a `QQmlPropertyMap` that contains all the values that were set either by assigning a value to a `moduleClientParameters` item in the parameters widget or by calling `PitikappModuleInstanceData::setClientParameter`.
* moduleData

  This is a `QQmlPropertyMap` that contains all the values that were set by calling `PitikappModuleInstanceData::setClientParameter`.

  It is possible to assign a value to `moduleData`. Doing so will result in the client sending the key-value pair to the server and call `PitikappModuleInstance::processData`. See the section below for more information.


Properties of the widget can be bound to any key of those two objects, as long as the key exists at the moment the binding is done.


The data widget also has access to a set of predefined colors that are used by the other widgets in the client. Those colors can be used in order to make a coherent UI where all the colors are similar.
The colors also automatically update when dark theme is enabled or disabled. The colors are:
  - `primaryBackground`: The primary background of the application.
  - `secondaryBackground`: The color of a secondary background in the application.
  - `applicationForeground`: The color of any widget to display above the background.
  - `widgetBackground`: The color of the background of the widgets.
  - `widgetInternalBackground`: The color to use in the background inside every widget.
  - `buttons`: The color of the buttons.
  - `notificationText`: The color of the notification text.
  - `primaryText`: The color of primary texts. To put emphasis on an important text compared to a secondary one.
  - `secondaryText`: The color of secondary texts.

To bind a property to a color: `color: Colors.widgetInternalBackground`.

If the colors are not enough, it is also possible to read the configuration to see the state of the client.

Example: `property color buttonColor: Configuration.DarkThemeActive? "#899BA6" : "#90A4AE"`.

#### 2.3.6 PitikappModuleInstance

This is the class returned by `PitikappPlugin::createModuleInstance`. The application takes ownership of all the module instances. If `createModuleInstance` returns a null pointer, it will not be usable.

##### Send data

Each module instance takes a `PitikappModuleInstanceData` object as constructor parameter.

When assigning a value to a client data key using `PitikappModuleInstanceData::setClientData`, the key-value pair will be sent to the client application and be usable in the data widget through the `moduleData` object (see 'data widget' section above).

In the example (`ModuleProcessRunTime`), a `QTimer` runs every second to update the run time of the configured processes.

```cpp
QVector<ProcessData_t> processList = m_data.getConfiguredProcesses();

for (const ProcessData_t &processData : processList)
{
    m_data.setProcessExecutionDuration(processData.Id, getProcessRunDuration(processData.Path));
}
```

`setProcessExecutionDuration` calls `PitikappModuleInstanceData::setClientData`.

##### Receive data

By assigning any key-value pair to `moduleData`.

```
MouseArea
{
    anchors.fill: parent

    onClicked:
    {
        moduleData.Terminate = item_KillProcess.processId;
        item_KillProcess.visible = false;
    }
}
```


Assigning this property will trigger a call to `PitikappModuleInstance::processData`. This function can be overriden by the custom module instance class.

It takes as parameter a `QVariantMap` that contains the key that was assigned in the data widget. In the example, the `Terminate` key contains the ID of the process to be terminated. If the process is found, it will be killed.

### 2.4 Custom module embedding a predefined data type
_This is available in Pitikapp 1.1.1_.

It is also possible to use a predefined data type  but to customize it with addition parameters.

`ModuleCustomCounter` is similar to `ModuleCounter` but it allows entering a custom value for the counter instead of the default value of 60.

To make a widget available to all other plugins, use `qmlRegisterType`. For example:

```cpp
qmlRegisterType(QUrl("qrc:/widgetPath"), "com.pitikapp.plugin.widgets", 1, 0, "PitikappGaugeWidget");
```

This will allow any widget to import `com.pitikapp.plugin.widgets` to declare an instance of `PitikappGaugeWidget`. The widget must be registered before being used anywhere, for example in the plugin constructor.

### 2.4.1 PitikappModuleInstanceData

`ModuleCustomCounterData` inherits `PitikappModuleInstanceGaugeData` in order to use the features of `PitikappModuleInstanceGaugeData`. It must however be distinct from `PitikappModuleInstanceGaugeData`, which means it must use its own ID. This ID is given to the constructor of `PitikappModuleInstanceGaugeData`.

### 2.4.2 PitikappModuleDataInfo

The `PitikappModuleDataInfo` attached to `ModuleCustomCounterData` must copy the values from the `PitikappModuleDataInfo` of `PitikappModuleInstanceGaugeData`.

```cpp
PitikappModuleDataInfo ModuleCustomCounterData::ModuleDataInfo()
{
    PitikappModuleDataInfo dataInfo(DATA_CLASS_ID);
    dataInfo.setParameterEditionWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-CustomCounter/Parameters.qml"));
    dataInfo.setClientDataDisplayWidgetPath(PitikappModuleInstanceGaugeData::ModuleDataInfo().getClientDataDisplayWidgetPath());

    return dataInfo;
}
```

### 2.4.3 Parameter widget

The parameter widget must embed the parameter widget of `PitikappModuleInstanceGaugeData`. See `com.pitikapp.plugins.example.resources/Module-CustomCounter/Parameters.qml`.

The widget must initialize the `PitikappGaugeWidget` in order to properly load the current gauge parameter values:

```
function startEdition()
{
    spinbox_CounterMaxValue.value = moduleLocalParameters.CounterMaxValue;
    widget_Gauge.startEdition();
}

function endEdition()
{
    widget_Gauge.endEdition();
}
```

Use a local parameter callback to detect when the max value changes. When it changes, update the gauge parameters:
```
m_data.setParameter(PitikappGaugeParameter_e::MaxValue, value);
```

### 2.4.4 Data widget

The data widget cannot be customized. The default one will be used.


## 3 - Miscellaneous information

### 3.1 Resources

All files required by a plugin should be included in a Qt resource file.

It is recommended to have a unique path for each plugin in the Qt resource system.
Here, all files are stored in a folder `com.pitikapp.plugins.example.resources` and added to the `resources.qrc` file.

About image format:
 - A plugin icon should be an image with a transparent background.
 - A module icon should be a black image with transparent background.

### 3.2 Logging

If debug mode is enabled in the application settings (client or server), it is possible to use `QDebug` to log information.

The C++ code can use `qDebug()`, `qInfo()`, etc. while the QML code can use `console.info()`, `console.debug()`, etc.

When debug mode is enabled, the server can request to download the client's logs by clicking on the device name in the top bar.

All the logs can be found in `%LOCALAPPDATA%\Pitikapp Remote Dashboard`.

### 3.3 Available QML modules

Not all QML modules are currently available, as it would take too much space on the disk.
Send us an email if you think a new module should be available.

### 3.4 Localisation

Pitikapp is currently available only in English, so the plugins cannot be translated for now.

### 3.5 Contact

To ask a question, report a bug, show your plugin, send a suggestion, contact us at contact@pitikapp.com!

