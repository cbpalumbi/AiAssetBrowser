//AssetBrowserPanel

#include <QWidget>
#include <QTreeView>
#include "ModelPreviewWidget.h"
#include "TagEditorPanel.h"

class AssetBrowserPanel : public QWidget {
    Q_OBJECT
public:
    AssetBrowserPanel(QWidget* parent = nullptr);

private slots:
    void handleFileSelected(const QModelIndex& index);
    void handleMetadataSave(const QVariantMap& updatedMetadata);

private:
    ModelPreviewWidget* previewWidget;
    QTreeView* fileTreeWidget;
    TagEditorPanel* tagEditorPanel;
    QVariantMap currentMetadata;
    QString currentAssetFilePath;
};
