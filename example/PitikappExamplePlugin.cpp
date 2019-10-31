#include "PitikappExamplePlugin.h"

#include <QStorageInfo>

#include "PitikappModuleInstanceGaugeData.h"
#include "ModuleProcessRunTime/ModuleProcessRunTime.h"
#include "ModuleDiskUsage/ModuleDiskUsage.h"
#include "ModuleCounter/ModuleCounter.h"
#include "ModuleCustomCounter/ModuleCustomCounter.h"

namespace
{
    const QString MODULE_ID_PROCESS_RUN_TIME = QStringLiteral("com.pitikapp.plugins.example.module1");
    const QString MODULE_ID_DISK_USAGE       = QStringLiteral("com.pitikapp.plugins.example.module2");
    const QString MODULE_ID_COUNTER          = QStringLiteral("com.pitikapp.plugins.example.module3");
    const QString MODULE_ID_CUSTOM_COUNTER   = QStringLiteral("com.pitikapp.plugins.example.module4");

    PitikappExamplePlugin *Instance = nullptr;

    // Create a module ID unique for each volume on the computer.
    QString makeVolumeId(const QStorageInfo &storageInfo)
    {
        return MODULE_ID_DISK_USAGE + storageInfo.rootPath();
    }
}

extern "C" PitikappPlugin *getPitikappPluginInstance()
{
    if (!Instance)
    {
        Instance = new PitikappExamplePlugin();
    }

    return Instance;
}

PitikappExamplePlugin::PitikappExamplePlugin() :
    PitikappPlugin("com.pitikapp.plugins.example", "Example Plugin"),
    m_processRunTimeModuleInfo(MODULE_ID_PROCESS_RUN_TIME, "Process Run Time"),
    m_counterModuleInfo(MODULE_ID_COUNTER, "Counter"),
    m_customCounterModuleInfo(MODULE_ID_CUSTOM_COUNTER, "Custom Counter")
{
    // ------------------------------------------
    //  Init plugin data
    // ------------------------------------------
    setValid(true);
    setVisible(true);
    setDescription("This is an example plugin.");
    setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/PluginIcon.png"));

    // ------------------------------------------
    //  Register data classes.
    // ------------------------------------------
    registerModuleDataClass(ModuleProcessRunTimeData::ModuleDataInfo());
    registerModuleDataClass(ModuleCustomCounterData::ModuleDataInfo());

    // ------------------------------------------
    //  Register module meta-data.
    // ------------------------------------------
    // ----- Counter
    m_counterModuleInfo.setVisible(true);
    m_counterModuleInfo.setValid(true);
    m_counterModuleInfo.setDescription("Counter that goes from 0 to 60.");
    m_counterModuleInfo.setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-Counter/ModuleIcon.png"));
    registerModuleClass(&m_counterModuleInfo);
    // ----- Custom counter
    m_customCounterModuleInfo.setVisible(true);
    m_customCounterModuleInfo.setValid(true);
    m_customCounterModuleInfo.setDescription("Counter that goes from 0 to a custom value.");
    m_customCounterModuleInfo.setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-CustomCounter/ModuleIcon.png"));
    registerModuleClass(&m_customCounterModuleInfo);
    // ----- Process Run Time
    m_processRunTimeModuleInfo.setVisible(true);
    m_processRunTimeModuleInfo.setValid(true);
    m_processRunTimeModuleInfo.setDescription(
                               "<center><b style=\"color:black\">Test Module: Process Run Time.</b></center>"
                               "<br/>"
                               "Basic HTML is supported (Qt Rich Text)."
                               "<br/>"
                               "Visit <a href=\"https://www.pitikapp.com\">pitikapp.com</a> for more information.");
    m_processRunTimeModuleInfo.setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/ModuleIcon.png"));
    registerModuleClass(&m_processRunTimeModuleInfo);

    // ----- Disk Usage
    updateStorageList();
    QObject::connect(&m_updateStorageTimer, &QTimer::timeout,
                     std::bind(&PitikappExamplePlugin::updateStorageList, this));
    m_updateStorageTimer.start(10000);
}

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
    else if (MODULE_ID_CUSTOM_COUNTER == moduleId)
    {
        return new ModuleCustomCounter;
    }
    else if (auto iterator = m_storageModuleInfo.find(moduleId) ; iterator != m_storageModuleInfo.end())
    {
        return new ModuleDiskUsage(iterator->second.second);
    }

    return nullptr;
}

void PitikappExamplePlugin::updateStorageList()
{
    QList<QStorageInfo> mountedVolumes = QStorageInfo::mountedVolumes();

    // --------------------------------------------------------------
    // For each mounted volume, ensure there is one module available.
    // --------------------------------------------------------------
    for (const QStorageInfo &storageInfo : mountedVolumes)
    {
        QString moduleId = makeVolumeId(storageInfo);

        // ----------------------------------------
        // If there is no module for this, add one.
        // ----------------------------------------
        auto iterator = m_storageModuleInfo.find(moduleId);
        if (iterator == m_storageModuleInfo.end())
        {
            PitikappModuleInfo moduleInfo(moduleId, QString("%1 Usage").arg(storageInfo.rootPath()));
            moduleInfo.setVisible(true);
            moduleInfo.setValid(true);
            moduleInfo.setDescription(QString("Watch disk usage on %1 (%2)").arg(storageInfo.displayName()).arg(storageInfo.rootPath()));
            moduleInfo.setIcon(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-DiskUsage/ModuleIcon.png"));

            // -------------------------------------------------------
            // Insert the new module in the map of known disk modules.
            // -------------------------------------------------------
            auto insertIterator = m_storageModuleInfo.emplace(moduleId, std::make_pair(std::move(moduleInfo), storageInfo.rootPath()));

            // ---------------------------------------------------------------------
            // Don't forget to register the module to make it available to the user.
            // ---------------------------------------------------------------------
            registerModuleClass(&(insertIterator.first->second.first));
        }
        else
        {
            // -----------------------------------------------------
            // If there is a module but it was hidden, show it back.
            // -----------------------------------------------------
            if (!iterator->second.first.valid())
            {
                iterator->second.first.setVisible(true);
                iterator->second.first.setValid(true);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------
    // The other way around:
    // For each module, ensure the corresponding volume still exists (for example, if a disk was unplugged).
    // -----------------------------------------------------------------------------------------------------
    for (auto &[moduleId, moduleInfo] : m_storageModuleInfo)
    {
        auto iterator = std::find_if(
            mountedVolumes.begin(),
            mountedVolumes.end(),
            [&](const QStorageInfo &storageInfo)
            {
                return storageInfo.rootPath() ==  moduleInfo.second;
            }
        );

        // -------------------------------------------
        // This volume doesn't exist anymore, hide it.
        // -------------------------------------------
        if (iterator == mountedVolumes.end())
        {
            moduleInfo.first.setVisible(false);
            moduleInfo.first.setValid(false);
        }
    }
}
