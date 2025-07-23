// ModelPreviewWidget.cpp
#include "ModelPreviewWidget.h"

#include <QQuickItem>
#include <QUrl>
#include <QObject>
#include <QQmlContext>
#include <QVBoxLayout>

ModelPreviewWidget::ModelPreviewWidget(QWidget *parent) : QWidget(parent) {
    quickWidget = new QQuickWidget(this);
    quickWidget->setSource(QUrl(QStringLiteral("qrc:/ModelViewer.qml")));
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    //setCentralWidget(quickWidget);


    auto *layout = new QVBoxLayout(this);
    layout->addWidget(quickWidget);
    layout->setContentsMargins(0, 0, 0, 0);
}

void ModelPreviewWidget::loadModel(const QString &filePath) {
    QQuickItem* rootItem = quickWidget->rootObject();
    if (!rootItem) {
        qWarning() << "Root QML object is null";
        return;
    }
    QVariant urlString = QVariant(QUrl::fromLocalFile(filePath).toString());
    bool success = rootItem->setProperty("modelComponentSource", urlString);

    if (!success) {
        qWarning() << "Failed to set modelComponentSource property";
    } else {
        QVariant val = rootItem->property("modelComponentSource");
        qDebug() << "modelComponentSource is now set to:" << val.toString();
    }
}


