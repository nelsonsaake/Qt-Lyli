#include "lyli.h"

Lyli::Lyli()
{
    // setup the app and connect objects

    registerMetaTypes();
    enable_prepingForWork();
    enable_cancellingWork();
    enable_uiUpdates();
    enable_uiTaskingFolderScanning();
    enable_workFinishedNoifications();
    enable_folderScannerSharingCorruptedFiles();
    moveWorkersToTheirThreads();
}

void Lyli::start()
{
    // show ui

    w.show();
}

void Lyli::stop()
{
    // stop all threads

    stopWorkerThreads();
}


//
void Lyli::moveWorkersToTheirThreads()
{
    // move objects to their threads

    folderScanner.moveToThread(&scanningThread);
    fileRenamer.moveToThread(&renamingThread);
}

void Lyli::startWorkerThreads()
{

    scanningThread.start();
    renamingThread.start();
}

void Lyli::stopWorkerThreads(){

    scanningThread.quit();
    renamingThread.quit();

    scanningThread.wait();
    renamingThread.wait();
}

//
void Lyli::registerMetaTypes(){

    // register meta type
    // this will enable queuing of QVector<FolderInfo> types
    // without this signals and slots passing this type won't work
    // something about qt not knowing how to copy these object type over threads
    qRegisterMetaType<QVector<FolderInfo>>("QVector<FolderInfo>");
    qRegisterMetaType<QVector<FolderInfo>>("QVector<FolderInfo>");
    qRegisterMetaType<QVector<FileInfo>>("QVector<FileInfo>");
    qRegisterMetaType<FolderInfo>("FolderInfo");
    qRegisterMetaType<FileInfo>("FileInfo");
}

void Lyli::enable_prepingForWork(){

    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &folderScanner,
                     &FolderScanner::onPrepForWork);

    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &fileRenamer,
                     &FileRenamer::onPrepForWork);
}

void Lyli::enable_cancellingWork(){

    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &fileRenamer,
                     &FileRenamer::setIsCancelled);

    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &folderScanner,
                     &FolderScanner::onCancelled);
}

void Lyli::enable_uiUpdates(){

    QObject::connect(&fileRenamer,
                     &FileRenamer::fileRenamed,
                     &w,
                     &MainWindow::onFileRenamed);

    QObject::connect(&folderScanner,
                     &FolderScanner::folderScanned,
                     &w,
                     &MainWindow::onFolderScanned);
}

void Lyli::enable_uiTaskingFolderScanning(){

    QObject::connect(&w,
                     &MainWindow::scanFolder,
                     &folderScanner,
                     &FolderScanner::onScanBatch);
}

void Lyli::enable_folderScannerSharingCorruptedFiles(){

    QObject::connect(&folderScanner,
                     &FolderScanner::corruptedFilesFound,
                     &fileRenamer,
                     &FileRenamer::onRenameBatch);
}

void Lyli::enable_workFinishedNoifications(){

    QObject::connect(&folderScanner,
                     &FolderScanner::workFinished,
                     &fileRenamer,
                     &FileRenamer::onSupplierFinished);

    QObject::connect(&fileRenamer,
                     &FileRenamer::allFinished,
                     &w,
                     &MainWindow::onAllFinished);
}
