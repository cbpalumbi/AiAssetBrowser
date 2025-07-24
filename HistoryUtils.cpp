#include "HistoryUtils.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

bool HistoryUtils::appendEntry(const QString& historyFilePath,
                               const QString& user,
                               const QString& action,
                               const QVariantMap& data,
                               const QString& comment,
                               const QStringList& changedKeys)
{
    QFile file(historyFilePath);
    QJsonArray historyArray;

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open existing history file for reading:" << historyFilePath;
            return false;
        }

        QByteArray rawData = file.readAll();
        file.close();

        if (!rawData.trimmed().isEmpty()) {
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "Failed to parse existing history JSON:" << parseError.errorString();
                return false;
            }
            if (doc.isArray()) {
                historyArray = doc.array();
            } else {
                qWarning() << "History file does not contain a JSON array:" << historyFilePath;
                return false;
            }
        }
    }

    QJsonObject entry;
    entry["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    entry["user"] = user;
    entry["action"] = action;
    entry["changed_keys"] = QJsonArray::fromStringList(changedKeys);
    entry["data"] = QJsonObject::fromVariantMap(data);
    entry["comment"] = comment;

    historyArray.append(entry);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open history file for writing:" << historyFilePath;
        return false;
    }

    QJsonDocument outDoc(historyArray);
    file.write(outDoc.toJson());
    file.close();

    return true;
}


QVariantList HistoryUtils::readHistory(const QString& historyFilePath)
{
    QFile file(historyFilePath);
    QVariantList historyList;

    if (!file.exists()) {
        return historyList;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open history file for reading:" << historyFilePath;
        return historyList;
    }

    QByteArray data = file.readAll();
    file.close();

    if (data.trimmed().isEmpty()) {
        qWarning() << "History file is empty:" << historyFilePath;
        return historyList;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse history JSON:" << parseError.errorString();
        return historyList;
    }

    if (!doc.isArray()) {
        qWarning() << "History file does not contain a JSON array:" << historyFilePath;
        return historyList;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        if (val.isObject()) {
            historyList.append(val.toObject().toVariantMap());
        }
    }

    return historyList;
}

