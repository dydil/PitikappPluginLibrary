#include "ProcessSelectionHelper.h"

#include <QDebug>
#include <QFileInfo>
#include <QIcon>
#include <QFileIconProvider>
#include <QUrl>
#include <QDir>
#include <QImage>
#include <QBuffer>

namespace
{
    // Images are displayed using base64.
    // This function convers a QImage to a base64 string that can be used as the source of an
    // Image QML object.
    QString convertImageToDisplayableBase64(const QImage &image, const QString &imageFormat)
    {
        if (!image.isNull())
        {
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::ReadWrite);
            image.save(&buffer, imageFormat.toUtf8());

            return QString("data:image/%1;base64,%2").arg(imageFormat).arg(QString(imageData.toBase64()));
        }
        else
        {
            return QString();
        }
    }
}

ProcessSelectionHelper::ProcessSelectionHelper(QObject *parent) : QObject(parent)
{

}

void ProcessSelectionHelper::selectProcess(const QUrl &processPath)
{
    ProcessData_t processData;

    // Extract the icon from the process path.
    QFileInfo fileInfo(processPath.toLocalFile());
    QIcon fileIcon = QFileIconProvider().icon(fileInfo);
    QPixmap filePixmap = fileIcon.pixmap(fileIcon.actualSize(fileIcon.availableSizes().last()));

    processData.Icon = convertImageToDisplayableBase64(filePixmap.toImage(), "ico");
    processData.Name = processPath.fileName();
    processData.Path = QDir::toNativeSeparators(processPath.toLocalFile());
    processData.Id   = processData.Path.toUtf8().toBase64(QByteArray::Base64UrlEncoding);

    emit processSelected(processData);
}
