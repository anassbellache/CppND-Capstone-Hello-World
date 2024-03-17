//
// Created by anass on 3/15/24.
//

#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QGraphicsView>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QGraphicsPixmapItem>
#include <QToolBar>
#include "opencv4/opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    initUI();
}

MainWindow::~MainWindow()
{
    delete imageScene;
    delete imageView;
    delete mainStatusBar;
    delete mainStatusLabel;
    delete fileMenu;
    delete viewMenu;
    delete fileToolBar;
    delete viewToolBar;
    delete openAction;
    delete saveAsAction;
    delete exitAction;
    delete zoomInAction;
    delete zoomOutAction;
    delete prevAction;
    delete nextAction;
}

void MainWindow::initUI() {

    this->resize(800, 600);
    fileMenu = menuBar()->addMenu(tr("&File"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    editMenu = menuBar()->addMenu(tr("&Edit"));

    fileToolBar = addToolBar(tr("File"));
    viewToolBar = addToolBar(tr("View"));
    editToolBar = addToolBar(tr("Edit"));

    imageScene = new QGraphicsScene(this);
    imageView = new QGraphicsView(imageScene);
    setCentralWidget(imageView);

    mainStatusBar = statusBar();
    mainStatusLabel = new QLabel(mainStatusBar);
    mainStatusBar->addPermanentWidget(mainStatusLabel);
    mainStatusLabel->setText("Image information!");
    createActions();
}

void MainWindow::openImage() {
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    QStringList filePaths;
    if (dialog.exec()){
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
    }
}

void MainWindow::showImage(QString filePath) {
    imageScene->clear();
    imageView->resetTransform();
    QPixmap image(filePath);
    currentImage = imageScene->addPixmap(image);
    imageScene->update();
    imageView->setSceneRect(image.rect());
    QString status = QString("Image: %1, %2x%3")
            .arg(filePath).arg(image.width()).arg(image.height());
    mainStatusLabel->setText(status);
    currentImagePath = filePath;
}

void MainWindow::zoomIn() {
    mainStatusLabel->setText("Zoom In");
    imageView->scale(1.2, 1.2);
}

void MainWindow::zoomOut() {
    mainStatusLabel->setText("Zoom Out");
    imageView->scale(0.8, 0.8);
}

void MainWindow::saveAs()
{
    if (currentImage == nullptr) {
        QMessageBox::information(this, tr("Error"), tr("No image to save!"));
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Image As...");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if (QRegularExpression(".+\\.(png|xpm|jpg)").match(fileNames.at(0)).hasMatch()) {
            currentImage->pixmap().save(fileNames.at(0));
        } else {
            QMessageBox::information(this, tr("Error"), tr("Invalid file format!"));
        }
    }
}

void MainWindow::prevImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png" << "*.xpm" << "*.jpg";
    QStringList imageFiles = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int idx = imageFiles.indexOf(current.fileName());
    if (idx > 0) {
        showImage(dir.absoluteFilePath(imageFiles.at(idx - 1)));
    } else {
        QMessageBox::information(this, tr("Error"), tr("No previous image!"));
    }
}

void MainWindow::nextImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png" << "*.xpm" << "*.jpg";
    QStringList imageFiles = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int idx = imageFiles.indexOf(current.fileName());
    if (idx < imageFiles.size() - 1) {
        showImage(dir.absoluteFilePath(imageFiles.at(idx + 1)));
    } else {
        QMessageBox::information(this, tr("Error"), tr("No next image!"));
    }
}

void MainWindow::blurImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, tr("Error"), tr("No image to blur!"));
        return;
    }
    QPixmap pixmap = currentImage->pixmap();
    QImage image = pixmap.toImage();

}

void MainWindow::setupShortcuts()
{
    QList<QKeySequence> shortcuts;
    shortcuts << Qt::Key_Minus << Qt::Key_Equal;
    zoomInAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
    zoomOutAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Down << Qt::Key_Left;
    prevAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Down << Qt::Key_Right;
    nextAction->setShortcuts(shortcuts);
}

void MainWindow::createActions()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setStatusTip(tr("Open an existing image"));
    fileMenu->addAction(openAction);
    fileToolBar->addAction(openAction);
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));

    saveAsAction = new QAction(tr("&Save As..."), this);
    saveAsAction->setStatusTip(tr("Save the image to disk"));
    fileMenu->addAction(saveAsAction);
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Exit the application"));
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));

    zoomInAction = new QAction(tr("Zoom &In"), this);
    zoomInAction->setStatusTip(tr("Zoom in the image"));
    viewMenu->addAction(zoomInAction);
    viewToolBar->addAction(zoomInAction);
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));

    zoomOutAction = new QAction(tr("Zoom &Out"), this);
    zoomOutAction->setStatusTip(tr("Zoom out the image"));
    viewMenu->addAction(zoomOutAction);
    viewToolBar->addAction(zoomOutAction);
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));

    prevAction = new QAction(tr("Pre&v"), this);
    prevAction->setStatusTip(tr("Show the previous image"));
    viewMenu->addAction(prevAction);
    viewToolBar->addAction(prevAction);
    connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(prevImage()));

    nextAction = new QAction(tr("&Next"), this);
    nextAction->setStatusTip(tr("Show the next image"));
    viewMenu->addAction(nextAction);
    viewToolBar->addAction(nextAction);
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));

    blurAction = new QAction(tr("&Blur"), this);
    editMenu->addAction(blurAction);
    editToolBar->addAction(blurAction);
    connect(blurAction, SIGNAL(triggered(bool)), this, SLOT(blurImage()));

    setupShortcuts();
}