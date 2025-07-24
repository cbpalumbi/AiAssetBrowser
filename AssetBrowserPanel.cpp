#include "AssetBrowserPanel.h"
#include "TagEditorPanel.h"
#include "MetadataUtils.h"
#include "HistoryUtils.h"

#include <QHBoxLayout>
#include <QFileSystemModel>


AssetBrowserPanel::AssetBrowserPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    previewWidget = new ModelPreviewWidget(this);

    // Create and configure the model
    QFileSystemModel* model = new QFileSystemModel(this);
    model->setRootPath("C:/Users/Bella/Documents/AssetBrowserProjectAssets");
    model->setNameFilters(QStringList() << "*.fbx" << "*.glb" << "*.obj");
    model->setNameFilterDisables(false);

    // Create and configure the tree view
    fileTreeWidget = new QTreeView(this);
    fileTreeWidget->setModel(model);
    fileTreeWidget->setRootIndex(model->index(model->rootPath()));
    fileTreeWidget->setHeaderHidden(false);
    fileTreeWidget->setAnimated(true);
    fileTreeWidget->setSortingEnabled(true);
    fileTreeWidget->setColumnWidth(0, 250);

    tagEditorPanel = new TagEditorPanel(this);

    layout->addWidget(previewWidget, /*stretch=*/3);
    layout->addWidget(fileTreeWidget, /*stretch=*/1);
    layout->addWidget(tagEditorPanel, 1);

    connect(fileTreeWidget, &QTreeView::clicked,
            this, &AssetBrowserPanel::handleFileSelected);

    connect(tagEditorPanel, &TagEditorPanel::saveRequested,
            this, &AssetBrowserPanel::handleMetadataSave);

}

void AssetBrowserPanel::handleFileSelected(const QModelIndex& index)
{
    auto* model = static_cast<QFileSystemModel*>(fileTreeWidget->model());
    QString assetFilePath = model->filePath(index);

    QFileInfo assetFileInfo(assetFilePath);
    if (!assetFileInfo.exists()) {
        qWarning() << "Selected asset file does not exist:" << assetFilePath;
        return;
    }

    currentAssetFilePath = assetFileInfo.absoluteFilePath();

    // Construct .qml preview path by replacing extension
    QString qmlPreviewPath = assetFileInfo.path() + "/" + assetFileInfo.completeBaseName() + ".qml";
    QFileInfo qmlFileInfo(qmlPreviewPath);

    if (!qmlFileInfo.exists()) {
        qWarning() << "QML preview file not found for asset:" << assetFilePath
                   << "\nExpected at:" << qmlPreviewPath;
        return;
    }

    qDebug() << "Loading QML preview:" << qmlPreviewPath;
    previewWidget->loadModel(qmlPreviewPath);


    // Load metadata JSON
    QString metadataFilePath = assetFileInfo.path() + "/" + assetFileInfo.completeBaseName() + ".metadata.json";
    QFileInfo metadataFileInfo(metadataFilePath);

    if (!metadataFileInfo.exists()) {
        qWarning() << "Metadata JSON file not found for asset:" << assetFilePath
                   << "\nExpected at:" << metadataFilePath;
        currentMetadata.clear();
        tagEditorPanel->loadTags(QStringList{});
        return;
    }

    currentMetadata = MetadataUtils::loadMetadata(metadataFilePath);
    if (currentMetadata.isEmpty()) {
        qWarning() << "Loaded metadata is empty or invalid for asset:" << assetFilePath;
        tagEditorPanel->loadTags(QStringList{});
        return;
    }

    tagEditorPanel->setMetadata(currentMetadata);
}

void AssetBrowserPanel::handleMetadataSave(const QVariantMap& updatedMetadata)
{
    if (currentAssetFilePath.isEmpty()) {
        qWarning() << "No asset selected to save metadata for.";
        return;
    }

    QFileInfo assetInfo(currentAssetFilePath);
    QString basePath = assetInfo.path() + "/" + assetInfo.completeBaseName();  // No extension
    QString metadataPath = basePath + ".metadata.json";

    if (!MetadataUtils::saveMetadata(metadataPath, updatedMetadata)) {
        qWarning() << "Failed to save metadata for" << metadataPath;
    } else {
        qDebug() << "Metadata saved to" << metadataPath;
        currentMetadata = updatedMetadata;
    }

    QString historyPath = basePath + ".history.json";

    HistoryUtils::appendEntry(historyPath, "cb", "metadata_update", updatedMetadata, "Updated tags");

}




