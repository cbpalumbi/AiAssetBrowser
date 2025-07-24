#ifndef METADATAUTILS_H
#define METADATAUTILS_H

#include <QString>
#include <QVariantMap>

class MetadataUtils
{
public:
    // Load entire JSON metadata as a QVariantMap.
    // Returns empty map if file missing or parse error.
    static QVariantMap loadMetadata(const QString& metadataFilePath);

    // Save entire QVariantMap metadata back to JSON file.
    // Returns true on success.
    static bool saveMetadata(const QString& metadataFilePath, const QVariantMap& metadata);
};

#endif // METADATAUTILS_H
