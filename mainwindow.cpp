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
    // pass handles from ui form to member variable

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
    // init member variables

    // group ui widget handles
    simpleInterfaceSpecificWidgets << scanButton << expertModeButton;
    expertModeSpecificWidgets << expertModeScanButton << folderInputArea << simpleInterfaceButton << rootDirPathInput << browseButton;
    scanningModeSpecificWidgets << cancelButton;

    // init variables
    listViewThreshold = 50;
    renamedFilesCount = 0;
    foldersScannedCount = 0;
    writeScanCompleted = nullptr;
}

void MainWindow::setup()
{
    //

    // start live git
    startLiveGif();

    // change mode to simple interface
    simpleInterface();

    // when preping for scan clear cancel state
    connect(this, &MainWindow::prepForWork, &cancelManager, &Worker::clearIsCancel);

    // when cancelling set cancel state
    connect(this, &MainWindow::cancelled, &cancelManager, &Worker::setIsCancelled);

    // when cancelled move to previous scan mode
    connect(this, &MainWindow::cancelled, this, &MainWindow::returnToPreviousScanMode);

    // when scanning for folder, let UI show scanning mode
    connect(this, &MainWindow::scanFolder, this, &MainWindow::scanningMode);

    // when ticker ticks, show the list of recently scanned folders and renamed files
    connect(&ticker, &Ticker::ticked, this, &MainWindow::on_showNextUiUpdate);
}


void MainWindow::showMessageOnStatus(QString message)
{
    // so message on statusbar

    int msec = 1000;
    statusBar->showMessage(message, msec);
}

void MainWindow::startLiveGif()
{
    // live gif is the spinning animation
    // it to show the user the program is still running

    QString fileName = ":/resources/loader_white.gif";
    liveGif = new QMovie;
    liveGif->setFileName(fileName);
    liveGifLabel->setMovie(liveGif);
    liveGif->start();
    liveGifLabel->setWindowOpacity(0.5);
}

void MainWindow::clearUi()
{
    // clear the list of renamed files and
    // scanned folders displayed in UI

    foldersUi->clear();
    filesUi->clear();
}

void MainWindow::clearState()
{
    // reset variable affected by scanning

    tempRenamedFiles.clear();
    tempScannedFolders.clear();
    foldersScannedCount = 0;
    renamedFilesCount = 0;
    cancelManager.clearIsCancel();
}

void MainWindow::hideModeWidgets()
{
    // hide all mode widgets
    //
    // that's the folder selecting button
    // the scan buttons, cancel button ...

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
    // mark the previous scan mode
    // change the mode
    // change the page if neccessary
    //
    // scan mode == expert mode or simple interface mode

    // all mode are on the scanning
    // (operation/home) page except  finished mode
    if(mode==FinishedMode) gotoPage(FinishedPage);
    else gotoPage(OperationPage);

    // if the current mode is a scanning mode, i.e
    // expert mode or simple interface mode
    // save it before you change it
    if(currentMode!=Mode::ScanningMode){
        previousScanMode=currentMode;
    }

    // store the current mode in a member variable
    currentMode = mode;
}

void MainWindow::gotoPage(Page page)
{
    // go to page

    stackedWidget->setCurrentIndex(page);
}

void MainWindow::prepForScan()
{
    // prepare for a new scan operation:
    // fo ui i.e. clear ui, clear state
    // signal other objects to also prepare

    clearUi();
    clearState();

    emit prepForWork();
}


// PRIVATE SLOTS
void MainWindow::on_showNextUiUpdate()
{
    // display the list of recently renamed files
    // display the list of recently scanned folders
    // make sure no list pass the threshold
    //
    // this function is also cancel co-operative
    // checks to see if cancel is called

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
    // signal any concerned to cancel operations
    // this is triggered by a push of button

    emit cancelled();
}

void MainWindow::on_expertModePushButton_clicked()
{
    // go to expert mode
    // this is triggered by a push of button

    expertMode();
}

void MainWindow::on_simpleInterfacePushButton_clicked()
{
    // go to expert mode
    // this is triggered by a push of button

    simpleInterface();
}

void MainWindow::on_scanPushButton_clicked()
{
    // scan all drives on the computer
    // this is triggered by a push of button

    prepForScan();

    emit scanFolder(toFolderInfoVector(QDir::drives()));
}

void MainWindow::on_expertModeScanPushButton_clicked()
{
    // scan selected folder

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

    // emit scanFolder cause the folderScanner may be on another thread
    // signal is the best way of communicating between thread
    emit scanFolder({{rootDirPath}});
}

void MainWindow::on_runAnotherScanButton_clicked()
{
    // return to last scan mode
    // i.e. expertMode or simpleInterface mode

    returnToPreviousScanMode();
}


//  PROTECTED SLOTS
void MainWindow::expertMode()
{
    // hide all widgets
    // show mode specific widgets

    hideModeWidgets();
    for(QWidget *widget: expertModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ExpertMode);
}

void MainWindow::simpleInterface()
{
    // hide all widgets
    // show mode specific widgets

    hideModeWidgets();
    for(QWidget *widget: simpleInterfaceSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::SimpleInterface);
}

void MainWindow::scanningMode()
{
    // hide all widgets
    // show mode specific widgets

    hideModeWidgets();
    for(QWidget *widget: scanningModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ScanningMode);
    ticker.start();
}

void MainWindow::returnToPreviousScanMode()
{
    // go back to SimpleInterface, or expertMode
    // which ever was the last one activated

    if(previousScanMode==Mode::SimpleInterface) simpleInterface();
    else expertMode();
}


// PUBLIC SLOTS
void MainWindow::onFolderScanned(FolderInfo scannedFolder)
{
    // add scannedFolder to list of scanned folders
    // increment the number of foldersScanned

    if(cancelManager.isCancelled()) return;
    tempScannedFolders << scannedFolder.path;
    foldersScannedCount += tempScannedFolders.size();
}

void MainWindow::onFileRenamed(FileInfo renamedFile)
{
    // add renamedFile to the list of files renamed
    // increment the number of files renamed

    if(cancelManager.isCancelled()) return;
    tempRenamedFiles << renamedFile.oldName;
    renamedFilesCount++;
}

void MainWindow::onAllFinished()
{
    // switch to finishedMode
    // write scan completed message as if it was being typed

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




