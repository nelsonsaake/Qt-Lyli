#ifndef LYLI_H
#define LYLI_H

#include "filerenamer.h"
#include "mainwindow.h"
#include "folderscanner.h"

#include <QThread>

class Lyli{

public:
    Lyli();

    void start();

    void stop();

protected:

    void moveWorkerToTheirThreads();

    void startWorkerThreads();

    void stopWorkerThreads();


    void registerMetaTypes();

    void enable_prepingForWork();

    void enable_cancellingWork();

    void enable_uiUpdates();

    void enable_uiTaskingFolderScanning();

    void enable_folderScannerSharingCorruptedFiles();

    void enable_workFinishedNoifications();

private:
    MainWindow w;
    FolderScanner folderScanner;
    FileRenamer fileRenamer;

    QThread scanningThread;
    QThread renamingThread;
};

#endif // LYLI_H
