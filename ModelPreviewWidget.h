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
