//AssetBrowserPanel

#include <QWidget>
#include <QTreeView>
#include "ModelPreviewWidget.h"

class AssetBrowserPanel : public QWidget {
    Q_OBJECT
public:
    AssetBrowserPanel(QWidget* parent = nullptr);

private slots:
    void handleFileSelected(const QModelIndex& index);

private:
    ModelPreviewWidget* previewWidget;
    QTreeView* fileTreeWidget;
};
