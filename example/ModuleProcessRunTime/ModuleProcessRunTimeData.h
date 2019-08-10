#pragma once

#include <QVector>

#include "PitikappModuleInstanceData.h"
#include "PitikappModuleDataInfo.h"
#include "ProcessSelectionHelper.h"

class ModuleProcessRunTimeData : public PitikappModuleInstanceData
{
    public:
        ModuleProcessRunTimeData();

        static PitikappModuleDataInfo ModuleDataInfo();

        QVector<ProcessData_t> getConfiguredProcesses() const;
        void setProcessExecutionDuration(const QString &processId, const QString &formattedDuration);

    private:
        void onProcessSelected(const ProcessData_t &processData);
        void onProcessRemoved(const QString &processId);
        void onProcessNameChanged(const QString &processId, const QString &newName);

        ProcessSelectionHelper m_processSelectionHelper;
};
