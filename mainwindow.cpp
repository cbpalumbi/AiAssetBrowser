#include "mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create and configure the model
    model = new QFileSystemModel(this);
    model->setRootPath("C:/Users/Bella/Documents/AssetBrowserProjectAssets");
    model->setNameFilters(QStringList() << "*.fbx" << "*.glb" << "*.obj");
    model->setNameFilterDisables(false);

    // Create and configure the tree view
    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(model->rootPath()));
    treeView->setHeaderHidden(false);
    treeView->setAnimated(true);
    treeView->setSortingEnabled(true);
    treeView->setColumnWidth(0, 250);

    setCentralWidget(treeView);
    setWindowTitle("AI Asset Browser - Home");
    resize(800, 600);
}

MainWindow::~MainWindow() {}
