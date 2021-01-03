#include "countupanimation.h"
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
    //    graphicsView = ui->graphicsView;

    simpleInterfaceSpecificWidgets << scanButton << expertModeButton;
    expertModeSpecificWidgets << expertModeScanButton << folderInputArea << simpleInterfaceButton << rootDirPathInput << browseButton;
    scanningModeSpecificWidgets << cancelButton;

    listViewThreshold = 50;
    renamedFilesCount = 0;
    foldersScannedCount = 0;
    writeScanCompleted = nullptr;

    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessageOnStatus(QString message){
    int msec = 1000;
    statusBar->showMessage(message, msec);
}

void MainWindow::setup()
{
    startLiveGif();

    simpleInterface();

    connect(this, &MainWindow::prepForWork, &cancelManager, &Worker::clearIsCancel);

    connect(this, &MainWindow::cancelled, &cancelManager, &Worker::setIsCancelled);

    connect(&ticker, &Ticker::ticked, this, &MainWindow::on_showNextUiUpdate);
}

void MainWindow::startLiveGif()
{
    // heart beat is the spinning animation
    // it to show the user the program is live

    QString fileName = ":/resources/loader356.gif";
    liveGif = new QMovie;
    liveGif->setFileName(fileName);
    liveGifLabel->setMovie(liveGif);
    liveGif->start();
    liveGifLabel->setWindowOpacity(0.5);
}

void MainWindow::expertMode()
{
    hideStateModeWidgets();
    for(QWidget *widget: expertModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ExpertMode);
}

void MainWindow::simpleInterface()
{
    hideStateModeWidgets();
    for(QWidget *widget: simpleInterfaceSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::SimpleInterface);
}

void MainWindow::scanningMode()
{
    hideStateModeWidgets();
    for(QWidget *widget: scanningModeSpecificWidgets){
        widget->setVisible(true);
    }
    changeMode(Mode::ScanningMode);
    ticker.start();
}

void MainWindow::returnToPreviousMode()
{
    //
    clearState();

    if(previousMode==Mode::SimpleInterface) simpleInterface();
    else expertMode();
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

void MainWindow::hideStateModeWidgets()
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

void MainWindow::finishViewLiveAnimation()
{
    QPixmap pixmap(":/resources/ic_bubble_chart_white_48dp.png");
    QGraphicsItem *item = new QGraphicsPixmapItem(pixmap);
    item->setScale(0.3);
    item->setPos(100,100);

    QTimeLine *timer = new QTimeLine(5000);
    timer->setFrameRange(0, 100);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(item);
    animation->setTimeLine(timer);

    for (int i = 0; i < 360; ++i)
        animation->setRotationAt(i/360.0, i);

    scene.setParent(this);
    scene.setSceneRect(0, 0, 200, 200);
    scene.addItem(item);

    graphicsView->setScene(&scene);

    timer->start();

    connect(timer, &QTimeLine::finished, timer, &QTimeLine::start);
}

void MainWindow::on_showNextUiUpdate()
{
    if(cancelManager.isCancelled()) return;

    if(filesUi->count()>listViewThreshold) filesUi->clear();
    filesUi->addItems(tempRenamedFiles);
    filesUi->scrollToBottom();
    tempRenamedFiles.clear();

    if(foldersUi->count() > listViewThreshold) foldersUi->clear();
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

    clearState();

    returnToPreviousMode();
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
    // scans all drives on the computer for
    // corrupted files

    // tell concerned resource to get ready for work
    // that may mean for them clear state
    // or re-enable some functionalities ...
    emit prepForWork();

    // clear everything for a fresh start
    clearUi();
    clearState();

    // convert qfileinfo to folderinfo
    QVector<FolderInfo> paths;
    for(QFileInfo fileInfo: QDir::drives()){
        paths << FolderInfo{fileInfo.path()};
    }
    if(!paths.isEmpty()) emit scanFolder(paths);

    // show the scanning mode interface
    scanningMode();
}

void MainWindow::on_expertModeScanPushButton_clicked()
{
    emit prepForWork();

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
    if(rootDir.exists()){

        clearUi();
        clearState();

        // start
        emit scanFolder({{rootDirPath}});

        // to make it possible for the user
        // to cancel the operation
        scanningMode();

    }else {
        QString errorMessage = "Error: directory provided doesnot exist";
        QMessageBox::information(this, title, errorMessage + message, QMessageBox::Ok);
        showMessageOnStatus(errorMessage);
    }
}

void MainWindow::on_runAnotherScanButton_clicked()
{
    returnToPreviousMode();
}

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
    renamedFilesCount += tempRenamedFiles.size();
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




