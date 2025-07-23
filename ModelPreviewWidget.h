#ifndef MODEL_PREVIEW_WIDGET_H
#define MODEL_PREVIEW_WIDGET_H

// ModelPreviewWidget.h
#include <QQuickWidget>
#include <QWidget>

class ModelPreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit ModelPreviewWidget(QWidget *parent = nullptr);
    void loadModel(const QString &filePath);

private:
    QQuickWidget *quickWidget;
};

#endif // MODEL_PREVIEW_WIDGET_H
