#ifndef TAGEDITORPANEL_H
#define TAGEDITORPANEL_H

#include <QWidget>
#include <QStringList>

class QListWidget;
class QLineEdit;
class QPushButton;

class TagEditorPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TagEditorPanel(QWidget* parent = nullptr);

    // Load tags into UI
    void loadTags(const QStringList& tags);
    void onSaveButtonClicked();

    // Get current tags from UI
    QStringList getTags() const;

signals:
    // Emitted when user clicks Save
    void saveRequested(const QVariantMap& updatedMetadata);

private slots:
    void addTag();
    void removeSelectedTag();

private:
    QListWidget* tagListWidget;
    QLineEdit* newTagLineEdit;
    QPushButton* addTagButton;
    QPushButton* removeTagButton;
    QPushButton* saveButton;
};

#endif // TAGEDITORPANEL_H
