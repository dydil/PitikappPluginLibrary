# Pitikapp Plugin Library
This library can be used to create a plugin for Pitikapp.
For more information about Pitikapp, visit [pitikapp.com]

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

A plugin must be compiled using Qt 5.13.0 MSVC 2017 64 bits.

# Global architecture

A plugin is represented by the abstract class `PitikappPlugin`. Each plugin class must have a unique id (e.g. "com.pitikapp.plugin.plugin1").

The meta-data of a module is represented by the class `PitikappModuleInfo`. A list of `PitikappModuleInfo` can be attached to the `PitikappPlugin` instance. This list is used to show all the modules available for a plugin.

`PitikappModuleInstance` represents the actual module. It is created upon request from the user.

`PitikappModuleInstanceData` stores all the data necessary for the use of a `PitikappModuleInstance`. This includes the parameters that the user can modify as well as the data that must be displayed in a client.
Each `PitikappModuleInstance` object requires an instance of `PitikappModuleInstanceData`.

Each module instance can use:
    - A widget for the user to modify the parameters (colors, sizes, ...).
    - A widget to show data on the client application.
    - A list of files that can be used in the client (e.g. images).
All this information is stored in a `PitikappModuleDataInfo` object. A plugin can register several `PitikappModuleDataInfo` types as long as each has a unique ID.
All resources (QML files, images, ...) of a `PitikappModuleDataInfo` must be embedded in a Qt resource file.

Each `PitikappModuleInstanceData` takes as constructor the ID of a `PitikappModuleDataInfo` that is registered by the plugin.

A module can also use one of the predefined `PitikappModuleInstanceData` classes. For now there is only one available: `PitikappModuleInstanceGaugeData`.
By using `PitikappModuleInstanceGaugeData` or other predefined classes, a plugin doesn't need to register its own `PitikappModuleDataInfo` type as everything is included.


# How it works

At startup, Pitikapp looks for a shared library file called `plugin.dll` in each sub-directory of `./plugins`.

If the file exists and is a valid shared library it will try to call a function `getPitikappPluginInstance()` that returns a pointer to an object of type `PitikappPlugin`.

If the file cannot be loaded (for example incompatible architecture or missing dependencies) or if the `getPitikappPluginInstance` symbol is not foun, the plugin is ignored and won't be listed anywhere.

# Example

The attached demo project shows the all the possibilities offered by the plugin library.

Below is a step-by-step explanation of how to create the plugin.

## 1 - Creation of a Pitikapp plugin class

`PitikappPlugin` is an abstract class. `createModuleInstance` must be overriden in a subclass.
In the example, this class is called `PitikappExamplePlugin`.

The constructor of `PitikappPlugin` takes two parameters:
    - The ID of the plugin (must be unique globally)
    - The name of the plugin. This name must be short to fit in all the screens.
```cpp
PitikappExamplePlugin::PitikappExamplePlugin() : PitikappPlugin("com.pitikapp.plugins.example", "Example Plugin")
```
The constructor of the plugin object must initialize the meta-data for all plugins and modules.

### 1.1 - Plugin meta-data

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

### 1.2 - Module data classes

```cpp
registerModuleDataClass(ModuleProcessRunTimeData::ModuleDataInfo());
```

Register all the data classes that will be used by the modules of this plugin.

If a module will use a predefined data class (such as `PitikappModuleInstanceGaugeData`), it doesn't need to be registered.

### 1.3 - Declaration of module meta-data

```cpp
std::list<PitikappModuleInfo> m_moduleInfo;

for (PitikappModuleInfo &moduleInfo : m_moduleInfo)
{
    registerModuleClass(&moduleInfo);
}
```

All module meta-data is stored in a vector.
Note that meta-data must remain valid until the program is shut down. If it is deleted, the module won't be available anymore.

An invalid module will display a warning sign.
An invisible module will not be displayed in the module selection window but still displayed in the information window.

All those properties can be changed at any time and the displayed data will be updated in real time.

### 1.4 - Creation of a module instance

```cpp
PitikappModuleInstance *createModuleInstance(const QString &moduleId) override final;
```

This is the function that will be called when the user picks a type in the module selection window.

The plugin must return a `PitikappModuleInstance` for the given ID. If the ID is invalid, a null pointer can be returned.

```cpp
PitikappModuleInstance *PitikappExamplePlugin::createModuleInstance(const QString &moduleId)
{
    if (MODULE_ID_1 == moduleId)
    {
        return new Module1();
    }

    return nullptr;
}
```

### 1.4 - Make the plugin available to the application

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

## 2 - Creation of a Pitikapp module class

A module is composed of several classes:
    - `PitikappModuleInstance`
      An object of this type will be created upon request from the user.

    - `PitikappModuleInstanceData`
      Each `PitikappModuleInstance` object requires an instance of `PitikappModuleInstanceData`.
      This is the class that stores and display any kind of information and allows the user to modify the parameters.
      This class can be used by several modules.
      There are some pre-defined instance data classes.
      For example, to have a gauge using the same type as the default Pitikapp plugins, PitikappModuleInstanceGaugeData can be used.

    - PitikappModuleDataInfo
      Each `PitikappModuleInstanceData` is linked to a `PitikappModuleDataInfo` class.
      It will be used by the application to show the correct widgets when necessary.

### 2.1 - Simple module using a predefined data type

`ModuleCounter` is a simple module that shows a counter from 0 to 60 and sends a notification when reaching 60.
It uses `PitikappModuleInstanceGaugeData` as data type

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

`ModuleProcessRunTime` uses a custom data type. This section explains how to get there.

#### 2.3.1 - PitikappModuleDataInfo

#### 2.3.2 - PitikappModuleInstanceData

#### 2.3.4 - PitikappModuleInstance




## 3 - Resources

All files required by a plugin should be included in a Qt resource file.

It is recommended to have a unique path for each plugin in the Qt resource system.
Here, all files are stored in a folder `com.pitikapp.plugins.example.resources` and added to the `resources.qrc` file.

About image format:
 - A plugin icon should be an image with a transparent background.
 - A module icon should be a black image with transparent background.
