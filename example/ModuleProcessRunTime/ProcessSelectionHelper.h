#pragma once

#include <QObject>

struct ProcessData_t
{
    QString Icon; // base 64 encoding of the icon
    QString Id;   // Unique ID
    QString Name;
    QString Path; // Path with native separators
};

class ProcessSelectionHelper : public QObject
{
    Q_OBJECT

    public:
        explicit ProcessSelectionHelper(QObject *parent = nullptr);

        Q_INVOKABLE void selectProcess(const QUrl &processPath);

    signals:
        void processSelected(const ProcessData_t &processData);
        void processRemoved(const QString &processId);
        void processNameChanged(const QString &processId, const QString &processName);
};
