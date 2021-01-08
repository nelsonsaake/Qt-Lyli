#include "countupanimation.h"
#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "writinganimation.h"

#include <QDir>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QMessageBox>
#include <QMovie>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    getUiHandles();
    init();
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// PRIVATE METHODS
void MainWindow::getUiHandles()
{

    filesUi = ui->filesListWidget;
    foldersUi = ui->foldersListWidget;
    rootDirPathInput = ui->rootDirLineEdit;
    statusBar = ui->statusBar;
    liveGifLabel = ui->loaderAnimationlabel;
    startButton = ui->expertModeScanPushButton;
    cancelButton = ui->cancelPushButton;
    scanButton = ui->scanPushButton;
    expertModeButton = ui->expertModePushButton;
    simpleInterfaceButton = ui->simpleInterfacePushButton;
    expertModeScanButton = ui->expertModeScanPushButton;
    stackedWidget = ui->stackedWidget;
    folderInputArea = ui->actionAreaRow1;
    browseButton = ui->browsePushButton;
    scanCompletedLabel = ui->scanCompletedLabel;
}

void MainWindow::init()
{

    simpleInterfaceSpecificWidgets << scanButton << expertModeButton;
    expertModeSpecificWidgets << expertModeScanButton << folderInputArea << simpleInterfaceButton << rootDirPathInput << browseButton;
    scanningModeSpecificWidgets << cancelButton;

    listViewThreshold = 50;
    renamedFilesCount = 0;
    foldersScannedCount = 0;
    writeScanCompleted = nullptr;
}

void MainWindow::setup()
{
    startLiveGif();

    simpleInterface();

    connect(this, &MainWindow::prepForWork, &cancelManager, &Worker::clearIsCancel);

    connect(this, &MainWindow::cancelled, &cancelManager, &Worker::setIsCancelled);

    connect(this, &MainWindow::cancelled, this, &MainWindow::returnToPreviousMode);

    connect(this, &MainWindow::scanFolder, this, &MainWindow::scanningMode);

    connect(&ticker, &Ticker::ticked, this, &MainWindow::on_showNextUiUpdate);
}


void MainWindow::showMessageOnStatus(QString message){
    int msec = 1000;
    statusBar->showMessage(message, msec);
}

void MainWindow::startLiveGif()
{
    // heart beat is the spinning animation
    // it to show the user the program is live

    QString fileName = ":/resources/loader_white.gif";
    liveGif = new QMovie;
    liveGif->setFileName(fileName);
    liveGifLabel->setMovie(liveGif);
    liveGif->start();
    liveGifLabel->setWindowOpacity(0.5);
}

void MainWindow::clearUi()
{
    // clear ui we start a fresh
    foldersUi->clear();
    filesUi->clear();
}

void MainWindow::clearState()
{
    tempRenamedFiles.clear();
    tempScannedFolders.clear();
    foldersScannedCount = 0;
    renamedFilesCount = 0;
}

void MainWindow::hideModeWidgets()
{
    for(QWidget *widget: simpleInterfaceSpecificWidgets){
        widget->setVisible(false);
    }

    for(QWidget *widget: expertModeSpecificWidgets){
        widget->setVisible(false);
    }

    for(QWidget *widget: scanningModeSpecificWidgets){
        widget->setVisible(false);
    }
}

void MainWindow::changeMode(MainWindow::Mode mode)
{
    if(mode==FinishedMode) gotoPage(FinishedPage);
    else gotoPage(OperationPage);

    if(currentMode!=Mode::ScanningMode){
        previousMode=currentMode;
    }
    currentMode = mode;
}

void MainWindow::gotoPage(Page page)
{
    stackedWidget->setCurrentIndex(page);
}

void MainWindow::prepForScan()
{
    // clear everything for a fresh scan
    clearUi();
    clearState();

    emit prepForWork();
}


// PRIVATE SLOTS
void MainWindow::on_showNextUiUpdate()
{
    if(cancelManager.isCancelled()) return;

    if(filesUi->count()>listViewThreshold) filesUi->clear();
    filesUi->addItems(tempRenamedFiles);
    filesUi->scrollToBottom();
    tempRenamedFiles.clear();

    if(foldersUi->count()>listViewThreshold) foldersUi->clear();
    foldersUi->addItems(tempScannedFolders);
    foldersUi->scrollToBottom();
    tempScannedFolders.clear();
}

void MainWindow::on_browsePushButton_clicked()
{
    // Opens up a dialog box that will allow user to select a folder
    // This folder and all of it's subfolders will be searched for
    // files with .lyli extension

    QFileDialog rootFolderDialog(this);
    rootFolderDialog.setViewMode(QFileDialog::List);
    rootFolderDialog.setFileMode(QFileDialog::Directory);
    QString path = rootFolderDialog.getExistingDirectory();
    if(!path.isEmpty()) rootDirPathInput->setText(path);
}

void MainWindow::on_cancelPushButton_clicked()
{
    emit cancelled();
}

void MainWindow::on_expertModePushButton_clicked()
{
    expertMode();
}

void MainWindow::on_simpleInterfacePushButton_clicked()
{
    simpleInterface();
}

void MainWindow::on_scanPushButton_clicked()
{
    prepForScan();

    emit scanFolder(toFolderInfoVector(QDir::drives()));
}

void MainWindow::on_expertModeScanPushButton_clicked()
{
    prepForScan();

    QString title = "Error";
    QString message = ".\n"
                      "Please try again, type in a full directory path or "
                      "drag and drop a folder on the text input. "
                      "\n\n"
                      "Click Ok to exit.";

    QString rootDirPath = rootDirPathInput->text();
    if(rootDirPath.isEmpty()) {

        QString errorMessage = "Error: input is empty";
        QMessageBox::information(this, title, errorMessage + message, QMessageBox::Ok);
        showMessageOnStatus(errorMessage);
        return;
    }

    QDir rootDir(rootDirPath);
    if(!rootDir.exists()){
        QString errorMessage = "Error: directory provided doesnot exist";
        QMessageBox::information(this, title, errorMessage + message, QMessageBox::Ok);
        showMessageOnStatus(errorMessage);
        return;
    }

    emit scanFolder({{rootDirPath}});
}

void MainWindow::on_runAnotherScanButton_clicked()
{
    returnToPreviousMode();
}


//  PROTECTED SLOTS
void MainWindow::expertMode()
{
    hideModeWidgets();
    for(QWidget *widget: expertModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ExpertMode);
}

void MainWindow::simpleInterface()
{
    hideModeWidgets();
    for(QWidget *widget: simpleInterfaceSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::SimpleInterface);
}

void MainWindow::scanningMode()
{
    hideModeWidgets();
    for(QWidget *widget: scanningModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ScanningMode);
    ticker.start();
}

void MainWindow::returnToPreviousMode()
{
    //
    if(previousMode==Mode::SimpleInterface) simpleInterface();
    else expertMode();
}


// PUBLIC SLOTS
void MainWindow::onFolderScanned(FolderInfo scannedFolder)
{
    if(cancelManager.isCancelled()) return;
    tempScannedFolders << scannedFolder.path;
    foldersScannedCount += tempScannedFolders.size();
}

void MainWindow::onFileRenamed(FileInfo renameFile)
{
    if(cancelManager.isCancelled()) return;
    tempRenamedFiles << renameFile.oldName;
    renamedFilesCount++;
}

void MainWindow::onAllFinished()
{
    changeMode(FinishedMode);

    if(writeScanCompleted==nullptr){
        // so we will lazily create this writeScanCompleted object
        // that is to say if a scan never finishes it would never be created
        constexpr int sec = 1500;
        QString text = "Scan completed ... <br>" +
                QString::number(foldersScannedCount) + " Folders scanned ... <br>" +
                QString::number(renamedFilesCount) + " files restored to original format ... <br>" +
                "Thanks for choosing lyli ...";
        writeScanCompleted = new WritingAnimation(sec, text);

        connect(writeScanCompleted,
                &WritingAnimation::update,
                scanCompletedLabel,
                &QLabel::setText);
    }

    writeScanCompleted->startWriting();
}




