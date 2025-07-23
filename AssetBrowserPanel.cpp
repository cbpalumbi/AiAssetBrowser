#include "AssetBrowserPanel.h"

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
    model->setNameFilters(QStringList() << "*.qml");
    model->setNameFilterDisables(false);

    // Create and configure the tree view
    fileTreeWidget = new QTreeView(this);
    fileTreeWidget->setModel(model);
    fileTreeWidget->setRootIndex(model->index(model->rootPath()));
    fileTreeWidget->setHeaderHidden(false);
    fileTreeWidget->setAnimated(true);
    fileTreeWidget->setSortingEnabled(true);
    fileTreeWidget->setColumnWidth(0, 250);

    layout->addWidget(previewWidget, /*stretch=*/3);
    layout->addWidget(fileTreeWidget, /*stretch=*/1);

    connect(fileTreeWidget, &QTreeView::clicked,
            this, &AssetBrowserPanel::handleFileSelected);

}

void AssetBrowserPanel::handleFileSelected(const QModelIndex& index)
{
    QString filePath = static_cast<QFileSystemModel*>(fileTreeWidget->model())->filePath(index);
    qDebug() << "Clicked:" << filePath;
    previewWidget->loadModel(filePath);
}

