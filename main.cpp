#include "filerenamer.h"
#include "mainwindow.h"
#include "folderscanner.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    FolderScanner folderScanner;
    FileRenamer fileRenamer;
    QThread targetBuilderThread;
    QThread fileRenamerThread;

    // show the main window
    w.show();

    // register meta type
    // this will enable queuing of QVector<FolderInfo> types
    // without this signals and slots passing this type won't work
    // something about qt not knowing how to copy these object type over threads
    qRegisterMetaType<QVector<FolderInfo>>("QVector<FolderInfo>");
    qRegisterMetaType<QVector<FileInfo>>("QVector<FileInfo>");
    qRegisterMetaType<FolderInfo>("FolderInfo");
    qRegisterMetaType<FileInfo>("FileInfo");

    // for ui to tell the resources to get ready for work
    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &folderScanner,
                     &FolderScanner::onPrepForWork);

    QObject::connect(&w,
                     &MainWindow::prepForWork,
                     &fileRenamer,
                     &FileRenamer::clearIsCancel);

    // targetBuilder tells renamer to rename new set of files
    QObject::connect(&folderScanner,
                     &FolderScanner::renameBatch,
                     &fileRenamer,
                     &FileRenamer::onRenameBatch);

    QObject::connect(&w,
                     &MainWindow::scanFolder,
                     &folderScanner,
                     &FolderScanner::onScanBatch);

    // tell renamer to stop renaming files
    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &fileRenamer,
                     &FileRenamer::setIsCancelled);

    // let scanner / targetbuilder know that is supposed to cancel op.
    QObject::connect(&w,
                     &MainWindow::cancelled,
                     &folderScanner,
                     &FolderScanner::onCancelled);

    // let ui know once a file has been renamed
    QObject::connect(&fileRenamer,
                     &FileRenamer::fileRenamed,
                     &w,
                     &MainWindow::onFileRenamed);

    // let ui know once a folder has been scanned
    QObject::connect(&folderScanner,
                     &FolderScanner::folderScanned,
                     &w,
                     &MainWindow::onFolderScanned);

    // folder scanner will notify the ui when no
    // files will be supplied for renaming
    // at this point when rename is done with current work load,
    // it will tell ui it is all finished
    QObject::connect(&folderScanner,
                     &FolderScanner::workFinished,
                     &fileRenamer,
                     &FileRenamer::onSupplierFinished);

    // allfinished tell the ui the program is done with work
    QObject::connect(&fileRenamer,
                     &FileRenamer::allFinished,
                     &w,
                     &MainWindow::onAllFinished);

    // target builder will do it's work in a different thread
    folderScanner.moveToThread(&targetBuilderThread);
    targetBuilderThread.start();

    // renaming will go on in another thread
    fileRenamer.moveToThread(&fileRenamerThread);
    fileRenamerThread.start();

    // keep the application running
    int res = a.exec();

    // stop threads
    targetBuilderThread.quit();
    fileRenamerThread.quit();

    // wait for threads to stop executing
    targetBuilderThread.wait();
    fileRenamerThread.wait();

    return res;
}

//
// have to keep track of 1. number of folders scanned
// 2. number of files renamed
// create a class called animated display of text
// that would use to animate displaying of stats after the app is finished
// make ticker inif
