#ifndef METADATAUTILS_H
#define METADATAUTILS_H

#include <QString>
#include <QVariantMap>
#include <QStringList>

class MetadataUtils
{
public:
    // Load entire JSON metadata as a QVariantMap.
    // Returns empty map if file missing or parse error.
    static QVariantMap loadMetadata(const QString& metadataFilePath);

    // Save entire QVariantMap metadata back to JSON file.
    // Returns true on success.
    static bool saveMetadata(const QString& metadataFilePath, const QVariantMap& metadata);

    // History entries need to know which metadata keys were changed
    static QStringList computeChangedKeys(const QVariantMap& oldMetadata, const QVariantMap& newMetadata);

};

#endif // METADATAUTILS_H
