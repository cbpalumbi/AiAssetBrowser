#include "MetadataUtils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSet>
#include <QList>
#include <QString>

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

QStringList MetadataUtils::computeChangedKeys(const QVariantMap& oldMetadata, const QVariantMap& newMetadata)
{
    QStringList oldKeys = oldMetadata.keys();  // keep a named variable to extend lifetime. segfault otherwise if you try to construct a set from qvariantmap.keys directly
    QStringList newKeys = newMetadata.keys();

    QSet<QString> oldKeysSet(oldKeys.begin(), oldKeys.end());
    QSet<QString> newKeysSet(newKeys.begin(), newKeys.end());

    QSet<QString> allKeys;
    allKeys.unite(oldKeysSet);
    allKeys.unite(newKeysSet);


    QStringList changedKeys;

    for (const QString& key : allKeys) {
        const QVariant& oldValue = oldMetadata.value(key);
        const QVariant& newValue = newMetadata.value(key);

        if (!oldMetadata.contains(key) || !newMetadata.contains(key)) {
            changedKeys.append(key);
        } else if (oldValue != newValue) {
            changedKeys.append(key);
        }
    }

    return changedKeys;
}

