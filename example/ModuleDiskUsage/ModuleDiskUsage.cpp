#include "ModuleDiskUsage.h"

#include <QStorageInfo>

ModuleDiskUsage::ModuleDiskUsage(const QString &rootPath) :
    PitikappModuleInstance(&m_data),
    m_rootPath(rootPath)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout, std::bind(&ModuleDiskUsage::updateUsage, this));

    m_updateTimer.start(5000);

    updateUsage();
}

void ModuleDiskUsage::updateUsage()
{
    QStorageInfo storageInfo(m_rootPath);

    if (storageInfo.isValid())
    {
        // Divide all by 1000 to avoid overflows
        auto bytesFree  = storageInfo.bytesFree()  / 1000;
        auto bytesTotal = storageInfo.bytesTotal() / 1000;
        auto bytesUsed  = bytesTotal - bytesFree;

        if (bytesTotal > 0)
        {
            m_data.setValue(bytesUsed * 100 / bytesTotal);
        }
    }
}
