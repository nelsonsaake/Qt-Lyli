#include "folderscanner.h"



FolderScanner::FolderScanner() {}

QVector<FolderInfo> FolderScanner::getSubFolders(const QDir &dir)
{
    //  return the list of folders in this folder

    if(!dir.exists()) {
        handleFolderDoesNotExist(dir);
        return {};
    }

    QList<QFileInfo> dirSubFolders = dir.entryInfoList({}, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QVector<FolderInfo> folders = toFolderInfoVector(dirSubFolders);

    return folders;
}

QVector<FileInfo> FolderScanner::getCorruptedFiles(const QDir &dir)
{
    // return the list of files in this folder ending with .lyli

    QString lyli = ".lyli"; // the type of files we are looking for
    QString lyliExtension = "*" + lyli;
    QStringList nameFilter{lyliExtension};

    if(!dir.exists()) {
        handleFolderDoesNotExist(dir);
        return {};
    }

    QList<QFileInfo> dirFiles = dir.entryInfoList(nameFilter, QDir::Files);
    QVector<FileInfo> files = toFileInfoVector(dirFiles);

    return files;
}


bool FolderScanner::hasNext()
{
    // bool = true, if workload is not empty

    return !folders.isEmpty();
}

FolderInfo FolderScanner::next()
{
    // take the first folder from workload
    // delete it from workload
    // return the folder

    FolderInfo folder;
    if(hasNext()){
        folder=folders.first();
        folders.removeFirst();
    }
    return folder;
}

void FolderScanner::newWorkLoad(QVector<FolderInfo> folders)
{
    // add folders to current workload

    this->folders << folders;
}

void FolderScanner::newResults(QVector<FileInfo> files)
{
    // add new corrupted files found to the list of corrupted files
    // list of corrupted files part of state of object

    this->corruptedFiles << files;
}

void FolderScanner::scanFolder(FolderInfo folder)
{
    // scans this single folder

    QDir dir(folder.path);

    if(!dir.exists()) {
        handleFolderDoesNotExist(dir);
        return;
    }

    QVector<FileInfo> files = getCorruptedFiles(dir);
    QVector<FolderInfo> folders = getSubFolders(dir);

    if(!files.isEmpty()) newResults(files);
    if(!folders.isEmpty()) newWorkLoad(folders);
    emit folderScanned(folder);
}

void FolderScanner::wrapup()
{
    // handle after scan operation
    // send work finished signal
    // send corruptedFilesFound signal

    // if we are wrapping up because cancelled is called
    // we don't want to do anything else
    if(isCancelled()) return;

    // let anyone concerned know work is finished
    emit workFinished();

    // pass on results from work done
    emit corruptedFilesFound(corruptedFiles);
}

void FolderScanner::scan()
{
    // scan the entire workload
    // this function is also cancel cooperative
    // checks if cancel is called periodically and handles it

    QEventLoop eventLoop;
    while(hasNext()){
        for(int i=0; i<50 && hasNext(); i++){
            if(isCancelled()) {
                dumpCurrentWorkLoad();
                return;
            }
            scanFolder(next());
        }

        // we work and break periodically so the eventloop
        // can handle signals coming in and other events
        eventLoop.processEvents();
    }

    // if we done for good
    if(!hasNext()) wrapup();
}

void FolderScanner::dumpCurrentWorkLoad()
{
    // delete workload AKA list of folders to be scanned
    // delete files found durring the scan AKA corrupted files

    folders.clear();
    corruptedFiles.clear();
}

void FolderScanner::handleFolderDoesNotExist(QDir dir)
{
    // check if folder does not exist
    // if folder doesnot exist show and error

    if(!dir.exists()) {
        qDebug() << "Error extracting targets from directory: "
                 << dir.path() << " provided doesnot exist";
        return;
    }
}


void FolderScanner::onScanBatch(QVector<FolderInfo> folders)
{
    // add folders to current workload
    // scan the entire workload

    newWorkLoad(folders);
    scan();
}

void FolderScanner::onPrepForWork()
{
    // delete current workload
    // clear cancel state

    dumpCurrentWorkLoad();
    clearIsCancel();
}

void FolderScanner::onCancelled()
{
    // delete current workload
    // set cancel state, cancel == true

    dumpCurrentWorkLoad();
    setIsCancelled();
}
