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

Lyli::~Lyli()
{
    // make sure threads stop
    stopWorkerThreads();
}

void Lyli::start()
{
    // show ui

    startWorkerThreads();
    w.show();
}

void Lyli::moveWorkersToTheirThreads()
{
    // move objects to their threads

    folderScanner.moveToThread(&scanningThread);
    fileRenamer.moveToThread(&renamingThread);
}

void Lyli::startWorkerThreads()
{
    // start threads

    scanningThread.start();
    renamingThread.start();
}

void Lyli::stopWorkerThreads()
{
    // dispose of the threads
    // we quit and wait
    // wait() for the thread to finish what its doing

    scanningThread.quit();
    renamingThread.quit();

    scanningThread.wait();
    renamingThread.wait();
}


void Lyli::registerMetaTypes()
{
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

void Lyli::enable_prepingForWork()
{
    // this connection will enable the UI to
    // send prepForWork signal to
    // folderScanner and fileRenamer

    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &folderScanner,
                     &FolderScanner::onPrepForWork);

    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &fileRenamer,
                     &FileRenamer::onPrepForWork);
}

void Lyli::enable_cancellingWork()
{
    // this connection will make it posible for the ui to
    // send cancel signal to workers
    // workers = fileRenamer & folderScanner

    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &fileRenamer,
                     &FileRenamer::setIsCancelled);

    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &folderScanner,
                     &FolderScanner::onCancelled);
}

void Lyli::enable_uiUpdates()
{
    // this connection will enable:
    // 1. fileRenamer to notify the ui when it renames
    // a file
    // 2. folderScanner to notigy the ui when it scans
    // a folder


    QObject::connect(&fileRenamer,
                     &FileRenamer::fileRenamed,
                     &w,
                     &MainWindow::onFileRenamed);

    QObject::connect(&folderScanner,
                     &FolderScanner::folderScanned,
                     &w,
                     &MainWindow::onFolderScanned);
}

void Lyli::enable_uiTaskingFolderScanning()
{
    // this connection will enable the ui to
    // give a list of folders to folderScanner to scan

    QObject::connect(&w,
                     &MainWindow::scanFolder,
                     &folderScanner,
                     &FolderScanner::onScanBatch);
}

void Lyli::enable_folderScannerSharingCorruptedFiles()
{
    // this connection will enable the folderScanner
    // to share corrupted files found durring scan
    //
    // currently the only one waiting to receive this
    // corrupted files is the fileRenamer
    //
    // that is mainly, pass corrupted files from
    // folderScanner to fileRenamer

    QObject::connect(&folderScanner,
                     &FolderScanner::corruptedFilesFound,
                     &fileRenamer,
                     &FileRenamer::onRenameBatch);
}

void Lyli::enable_workFinishedNoifications()
{
    // this connection will enable the folderScanner to
    // send a finished notification to the fileRenamer
    // and
    // the enable the fileRenamer to send an allFinished
    // signal to the ui
    //
    // allFinished means the scanning and renaming is
    // completed

    QObject::connect(&folderScanner,
                     &FolderScanner::workFinished,
                     &fileRenamer,
                     &FileRenamer::onSupplierFinished);

    QObject::connect(&fileRenamer,
                     &FileRenamer::allFinished,
                     &w,
                     &MainWindow::onAllFinished);
}
