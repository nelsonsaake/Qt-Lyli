#ifndef LYLI_H
#define LYLI_H

#include "filerenamer.h"
#include "mainwindow.h"
#include "folderscanner.h"

#include <QThread>

/**
 * Lyli
 *
 * Lyli manages the app components:
 * ui,
 * folder scanner and it's thread,
 * file renamer and it's thread,
 *
 * connects component so the can talk to each other
 *
 * start thread and make sure the end before app completes
*/
class Lyli
{

public:
    Lyli();

    // clean up
    ~Lyli();

    // start app
    // mainly show ui
    void start();

protected:
    // move workes to their threads
    void moveWorkersToTheirThreads();

    // start threads
    void startWorkerThreads();

    // stop threads
    void stopWorkerThreads();


    // registerMetaTypes
    // help qt copy heavy objects over threads and
    // help qt manage queue heavy objects
    void registerMetaTypes();

    // connect objects, to receive signal to prepare for work
    void enable_prepingForWork();

    // connect objects, to receive signal to cancel work
    void enable_cancellingWork();

    // connect objects, to send updates to ui
    void enable_uiUpdates();

    // connect objects, to send folders to be scanned;
    // from ui to folder scanner
    void enable_uiTaskingFolderScanning();

    // connect objects, to allow folder scanner
    // to share list of corrupted files found durring the scan
    void enable_folderScannerSharingCorruptedFiles();

    // connect objects, to allow ui to know when work is done
    void enable_workFinishedNoifications();

private:
    // ui
    MainWindow mainWindow;

    // folder scanner object
    FolderScanner folderScanner;

    // file renamer object
    FileRenamer fileRenamer;


    // folder scanner thread
    QThread scanningThread;

    // file renamer thread
    QThread renamingThread;
};

#endif // LYLI_H
