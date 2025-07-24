#include "MetadataUtils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QVariantMap MetadataUtils::loadMetadata(const QString& metadataFilePath)
{
    QFile file(metadataFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open metadata file for reading:" << metadataFilePath;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error in" << metadataFilePath << ":" << parseError.errorString();
        return {};
    }

    if (!doc.isObject()) {
        qWarning() << "Metadata JSON root is not an object:" << metadataFilePath;
        return {};
    }

    QJsonObject rootObj = doc.object();

    // Convert QJsonObject to QVariantMap
    return rootObj.toVariantMap();
}

bool MetadataUtils::saveMetadata(const QString& metadataFilePath, const QVariantMap& metadata)
{
    QFile file(metadataFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open metadata file for writing:" << metadataFilePath;
        return false;
    }

    QJsonObject jsonObj = QJsonObject::fromVariantMap(metadata);
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    qint64 bytesWritten = file.write(data);
    file.close();

    return bytesWritten == data.size();
}
