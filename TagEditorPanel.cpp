#include "TagEditorPanel.h"

#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

TagEditorPanel::TagEditorPanel(QWidget* parent)
    : QWidget(parent)
{
    tagListWidget = new QListWidget(this);
    newTagLineEdit = new QLineEdit(this);
    addTagButton = new QPushButton("Add Tag", this);
    removeTagButton = new QPushButton("Remove Selected", this);
    saveButton = new QPushButton("Save", this);

    // Layout for adding new tag
    QHBoxLayout* addLayout = new QHBoxLayout();
    addLayout->addWidget(newTagLineEdit);
    addLayout->addWidget(addTagButton);

    // Main vertical layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tagListWidget);
    mainLayout->addLayout(addLayout);
    mainLayout->addWidget(removeTagButton);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);

    // Connections
    connect(addTagButton, &QPushButton::clicked, this, &TagEditorPanel::addTag);
    connect(removeTagButton, &QPushButton::clicked, this, &TagEditorPanel::removeSelectedTag);
    connect(saveButton, &QPushButton::clicked, this, &TagEditorPanel::onSaveButtonClicked);
}

void TagEditorPanel::setMetadata(const QVariantMap& metadata) {
    currentMetadata = metadata;

    // Populate UI with current tags
    if (currentMetadata.contains("tags")) {
        loadTags(currentMetadata["tags"].toStringList());
    }
}


void TagEditorPanel::onSaveButtonClicked()
{
    currentMetadata["tags"] = getTags();
    emit saveRequested(currentMetadata);
}

void TagEditorPanel::loadTags(const QStringList& tags)
{
    tagListWidget->clear();
    for (const QString& tag : tags) {
        tagListWidget->addItem(tag);
    }
}

QStringList TagEditorPanel::getTags() const
{
    QStringList tags;
    for (int i = 0; i < tagListWidget->count(); ++i) {
        tags << tagListWidget->item(i)->text();
    }
    return tags;
}

void TagEditorPanel::addTag()
{
    QString newTag = newTagLineEdit->text().trimmed();
    if (newTag.isEmpty()) {
        QMessageBox::warning(this, "Invalid Tag", "Tag cannot be empty.");
        return;
    }

    // Check for duplicates
    for (int i = 0; i < tagListWidget->count(); ++i) {
        if (tagListWidget->item(i)->text().compare(newTag, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Duplicate Tag", "This tag already exists.");
            return;
        }
    }

    tagListWidget->addItem(newTag);
    newTagLineEdit->clear();
}

void TagEditorPanel::removeSelectedTag()
{
    auto selectedItems = tagListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::information(this, "Remove Tag", "No tag selected to remove.");
        return;
    }

    for (QListWidgetItem* item : selectedItems) {
        delete tagListWidget->takeItem(tagListWidget->row(item));
    }
}
