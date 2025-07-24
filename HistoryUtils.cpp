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
                               const QVariantMap& metadata,
                               const QString& message)
{
    QFile file(historyFilePath);
    QJsonArray historyArray;

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open existing history file for reading:" << historyFilePath;
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            historyArray = doc.array();
        }
    }

    QJsonObject entry;
    entry["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    entry["user"] = user;
    entry["action"] = action;
    entry["message"] = message;
    entry["metadata"] = QJsonObject::fromVariantMap(metadata);

    historyArray.append(entry);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
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

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const QJsonValue& val : array) {
            historyList.append(val.toObject().toVariantMap());
        }
    }

    return historyList;
}
