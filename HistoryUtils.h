#pragma once

#include <QString>
#include <QVariantMap>
#include <QVariantList>

class HistoryUtils {
public:
    struct HistoryEntry {
        QString timestamp;
        QString user;
        QString action;
        QString message;
        QVariantMap metadata;
    };

    static bool appendEntry(
                const QString& historyFilePath,
                const QString& user,
                const QString& action,
                const QVariantMap& data,
                const QString& comment,
                const QStringList& changedKeys);


    static QVariantList readHistory(const QString& historyFilePath);
};
