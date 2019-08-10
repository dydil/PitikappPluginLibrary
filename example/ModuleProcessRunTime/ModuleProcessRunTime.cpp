#include "ModuleProcessRunTime.h"

#include <functional>
#include <optional>

#include <tlhelp32.h>
#include <psapi.h>

#include <QDebug>
#include <QDateTime>
#include <QTimeZone>

namespace
{
    QString formatDuration(int milliseconds)
    {
        QString format;

        int seconds = milliseconds / 1000;
        int minutes = seconds / 60;
        int hours = minutes / 60;

        minutes -= hours * 60;
        seconds -= hours * 3600;
        seconds -= minutes * 60;

        if (hours > 0)
        {
            format += QString("%1 h ").arg(hours);
        }

        if (minutes > 0 || hours > 0)
        {
            format += QString(" %1 min").arg(minutes);
        }

        // Do not show seconds once the hour is reached.
        if ((seconds > 0 || minutes > 0) && (hours == 0))
        {
            format += QString(" %1 s").arg(seconds);
        }

        return format;
    }

    QString getProcessFullPath(const PROCESSENTRY32 &processInfo)
    {
        QString path;

        if (HANDLE hProcess ; hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID))
        {
            CHAR processPath[MAX_PATH];
            if (0 != GetModuleFileNameExA(hProcess, nullptr, processPath, MAX_PATH))
            {
                path = QString(processPath);
            }


            CloseHandle(hProcess);
        }

        return path;
    }

    std::optional<PROCESSENTRY32> getProcessEntry(const QString &processFullPath)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        std::optional<PROCESSENTRY32> returnValue;

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
        {
            qInfo() << "CreateToolhelp32Snapshot failed";
        }
        else
        {
            Process32First(processesSnapshot, &processInfo);
            if (processFullPath == getProcessFullPath(processInfo))
            {
                returnValue = processInfo;
            }

            while (Process32Next(processesSnapshot, &processInfo))
            {
                if (processFullPath == getProcessFullPath(processInfo))
                {
                    returnValue = processInfo;
                }
            }
        }

        return returnValue;
    }

    QDateTime getProcessStartTime(const PROCESSENTRY32 &processInfo)
    {
        QDateTime startTime;

        if (HANDLE hProcess ; hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID))
        {
            FILETIME creationTime;
            FILETIME exitTime;
            FILETIME kernelTime;
            FILETIME userTime;

            if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
            {
                // Convert to system time
                SYSTEMTIME systemTime;
                FileTimeToSystemTime(&creationTime, &systemTime);

                startTime = QDateTime(
                            QDate(systemTime.wYear, systemTime.wMonth, systemTime.wDay),
                            QTime(systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds),
                            Qt::UTC); // System time is always UTC
            }


            CloseHandle(hProcess);
        }

        return startTime;
    }

    QString getProcessRunDuration(const QString &processFullPath)
    {
        QString duration;

        auto processInfo = getProcessEntry(processFullPath);

        if (processInfo)
        {
            duration = formatDuration(getProcessStartTime(*processInfo).msecsTo(QDateTime::currentDateTime()));
        }

        return duration;
    }

    void terminateProcess(const PROCESSENTRY32 &processInfo)
    {
        if (HANDLE hProcess ; hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID))
        {
            TerminateProcess(hProcess, 0);

            CloseHandle(hProcess);
        }
    }
}

ModuleProcessRunTime::ModuleProcessRunTime() : PitikappModuleInstance(&m_data)
{
    QObject::connect(&m_updateTimer, &QTimer::timeout,
                     std::bind(&ModuleProcessRunTime::update, this));

    m_updateTimer.start(1000);
}

void ModuleProcessRunTime::processData(const QVariantMap &data)
{
    QVector<ProcessData_t> processList = m_data.getConfiguredProcesses();

    auto iterator = std::find_if(
        processList.begin(),
        processList.end(),
        [&](const ProcessData_t &processData)
        {
            return processData.Id == data["Terminate"].toString();
        }
    );

    if (iterator != processList.end())
    {
        auto processEntry = getProcessEntry(iterator->Path);

        if (processEntry)
        {
            qInfo() << "Terminating process" << iterator->Name;
            terminateProcess(*processEntry);
        }
    }
}

void ModuleProcessRunTime::update()
{
    QVector<ProcessData_t> processList = m_data.getConfiguredProcesses();

    for (const ProcessData_t &processData : processList)
    {
        m_data.setProcessExecutionDuration(processData.Id, getProcessRunDuration(processData.Path));
    }
}
