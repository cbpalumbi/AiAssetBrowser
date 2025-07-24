#include "mainwindow.h"
#include "AssetBrowserPanel.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    // ModelPreviewWidget* modelPreview = new ModelPreviewWidget(this);
    // modelPreview->loadModel("C:/Users/Bella/Documents/AssetBrowser/Duck.qml");


    //setCentralWidget(modelPreview);

    AssetBrowserPanel* assetBrowserPanel = new AssetBrowserPanel(this);
    setCentralWidget(assetBrowserPanel);
    setWindowTitle("AI Asset Browser - Home");
    resize(800, 600);


}

MainWindow::~MainWindow() {}
