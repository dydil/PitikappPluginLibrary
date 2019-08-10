#include "ModuleProcessRunTimeData.h"

#include <QByteArray>
#include <QUrl>
#include <QDebug>

namespace
{
    // Class ID
    const QString DATA_CLASS_ID = QStringLiteral("com.pitikapp.plugins.example.ModuleProcessRunTime.data");

    // Local parameter keys
    const QString LOCAL_PARAMETER_KEY_NOTIFICATIONS = QStringLiteral("Notifications");

    // Client paramenter keys
    const QString CLIENT_PARAMETER_KEY_PROCESS_DATA = QStringLiteral("ProcessList");
    const QString CLIENT_PARAMETER_KEY_SHOW_NAMES   = QStringLiteral("ShowNames");
    const QString CLIENT_PARAMETER_KEY_ALIGNMENT    = QStringLiteral("Alignment");

    // Data keys
    const QString CLIENT_DATA_KEY_RUN_TIME = QStringLiteral("ProcessRunTime");

    // Helpers
    const QString HELPER_KEY_PROCESS_SELECTION = QStringLiteral("ProcessSelection");

    // Map subkeys
    const QString KEY_PROCESS_DATA_ID        = QStringLiteral("ID");
    const QString KEY_PROCESS_DATA_FULL_PATH = QStringLiteral("FullPath");
    const QString KEY_PROCESS_DATA_NAME      = QStringLiteral("Name");
    const QString KEY_PROCESS_DATA_IMAGE     = QStringLiteral("Icon");

    // Enumeration for the possible alignments.
    // An equivalent enum is necessary in the QML data file.
    enum ProcessAlignment
    {
        NoAlignment,
        AlignTop,
        AlignCenter,
        AlignBottom
    };

    // Convert process data to a QVariantMap to store in the parameters.
    QVariantMap processDataToVariantMap(const ProcessData_t &processData)
    {
        return QVariantMap
        {
            {KEY_PROCESS_DATA_FULL_PATH, processData.Path},
            {KEY_PROCESS_DATA_NAME     , processData.Name},
            {KEY_PROCESS_DATA_IMAGE    , processData.Icon},
            {KEY_PROCESS_DATA_ID       , processData.Id},
        };
    }

    // Convert a QVariantMap to process data to read from parameter.
    ProcessData_t variantMapToProcessData(const QVariantMap &map)
    {
        ProcessData_t processData;

        processData.Path = map[KEY_PROCESS_DATA_FULL_PATH].toString();
        processData.Name = map[KEY_PROCESS_DATA_NAME].toString();
        processData.Icon = map[KEY_PROCESS_DATA_IMAGE].toString();
        processData.Id   = map[KEY_PROCESS_DATA_ID].toString();

        return processData;
    }
}

ModuleProcessRunTimeData::ModuleProcessRunTimeData() : PitikappModuleInstanceData(DATA_CLASS_ID)
{
    // ---------------------------------------------------------------------------------------------
    // Assign a default value to all parameters.
    // If this is not done, there will be many "Undefined property" warnings when a widget
    // is created for this module.
    // ---------------------------------------------------------------------------------------------
    // Local
    setLocalParameter(LOCAL_PARAMETER_KEY_NOTIFICATIONS, QVariantMap{});
    // Client
    setClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA, QVariantList());
    setClientParameter(CLIENT_PARAMETER_KEY_SHOW_NAMES, false);
    setClientParameter(CLIENT_PARAMETER_KEY_ALIGNMENT, NoAlignment);

    // ---------------------------------------------------------------------------------------------
    // Set module data to default.
    // ---------------------------------------------------------------------------------------------
    setClientData(CLIENT_DATA_KEY_RUN_TIME, QVariantMap());

    // ---------------------------------------------------------------------------------------------
    // Register the helper.
    // It will be accessible from the parameter QML file.
    // ---------------------------------------------------------------------------------------------
    addHelper(HELPER_KEY_PROCESS_SELECTION, &m_processSelectionHelper);

    // ---------------------------------------------------------------------------------------------
    // Attach callbacks to the actions from the helper.
    // Note: std::bind is used because the current class is not a QObject.
    // ---------------------------------------------------------------------------------------------
    QObject::connect(&m_processSelectionHelper, &ProcessSelectionHelper::processSelected,
                     std::bind(&ModuleProcessRunTimeData::onProcessSelected, this, std::placeholders::_1));
    QObject::connect(&m_processSelectionHelper, &ProcessSelectionHelper::processRemoved,
                     std::bind(&ModuleProcessRunTimeData::onProcessRemoved, this, std::placeholders::_1));
    QObject::connect(&m_processSelectionHelper, &ProcessSelectionHelper::processNameChanged,
                     std::bind(&ModuleProcessRunTimeData::onProcessNameChanged, this, std::placeholders::_1, std::placeholders::_2));
}

// ---------------------------------------------------------------------------------------------
// Static function to return all necessary information for this data type.
// ---------------------------------------------------------------------------------------------
PitikappModuleDataInfo ModuleProcessRunTimeData::ModuleDataInfo()
{
    PitikappModuleDataInfo info(DATA_CLASS_ID);

    // Widget to use to show data in the client.
    info.setClientDataDisplayWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Data.qml"));
    // Widget to use to modify the parameters in the server.
    info.setParameterEditionWidgetPath(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/Parameters.qml"));
    // Files that are used by the data widget.
    info.addClientFile(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/icon-confirm.png"));
    info.addClientFile(QUrl("qrc:/com.pitikapp.plugins.example.resources/Module-ProcessRunTime/icon-cancel.png"));

    return info;
}

QVector<ProcessData_t> ModuleProcessRunTimeData::getConfiguredProcesses() const
{
    QVariantList processDataListVariant = getClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA).toList();
    QVector<ProcessData_t> processDataList(processDataListVariant.size());

    int index = 0;
    for (const QVariant &processDataVariant : processDataListVariant)
    {
        processDataList[index] = variantMapToProcessData(processDataVariant.toMap());
        index++;
    }

    return processDataList;
}

void ModuleProcessRunTimeData::setProcessExecutionDuration(const QString &processId, const QString &formattedDuration)
{
    QVariantMap processData = getClientData(CLIENT_DATA_KEY_RUN_TIME).toMap();

    bool modified = false;

    auto iterator = processData.find(processId);
    if ((iterator != processData.end()) && (iterator.value().toString() != formattedDuration))
    {
        iterator.value() = formattedDuration;
        modified = true;
    }
    else
    {
        processData[processId] = formattedDuration;
        modified = true;
    }

    if (modified)
    {
        setClientData(CLIENT_DATA_KEY_RUN_TIME, processData);
    }
}

void ModuleProcessRunTimeData::onProcessSelected(const ProcessData_t &processData)
{
    qInfo() << "Process selected:" << processData.Path;

    QVariantList processDataList = getClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA).toList();

    processDataList.append(processDataToVariantMap(processData));

    setClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA, processDataList);
}

void ModuleProcessRunTimeData::onProcessRemoved(const QString &processId)
{
    QVariantList processDataList = getClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA).toList();

    auto iterator = std::find_if(
        processDataList.begin(),
        processDataList.end(),
        [&](const QVariant &processDataVariant)
        {
            return variantMapToProcessData(processDataVariant.toMap()).Id == processId;
        }
    );

    if (iterator != processDataList.end())
    {
        processDataList.erase(iterator);

        setClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA, processDataList);
    }
}

void ModuleProcessRunTimeData::onProcessNameChanged(const QString &processId, const QString &newName)
{
    QVariantList processDataList = getClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA).toList();

    auto iterator = std::find_if(
        processDataList.begin(),
        processDataList.end(),
        [&](const QVariant &processDataVariant)
        {
            return variantMapToProcessData(processDataVariant.toMap()).Id == processId;
        }
    );

    if (iterator != processDataList.end())
    {
        ProcessData_t processData = variantMapToProcessData(iterator->toMap());
        processData.Name = newName;
        *iterator = processDataToVariantMap(processData);

        setClientParameter(CLIENT_PARAMETER_KEY_PROCESS_DATA, processDataList);
    }
}
