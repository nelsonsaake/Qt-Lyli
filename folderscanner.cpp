#include "folderscanner.h"



FolderScanner::FolderScanner() {}

bool FolderScanner::hasNext(){
    return !folders.isEmpty();
}

FolderInfo FolderScanner::next(){
    FolderInfo folder;
    if(hasNext()){
        folder=folders.first();
        folders.removeFirst();
    }
    return folder;
}

void FolderScanner::newWorkLoad(QVector<FolderInfo> folders){
    this->folders << folders;
}

void FolderScanner::scanFolder(FolderInfo folder){

    QString path = folder.path;
    QDir dir(path);
    QList<QFileInfo> dirFiles;
    QList<QFileInfo> dirSubFolders;

    QString lyli = ".lyli"; // the type of files we are looking for
    QString lyliExtension = "*" + lyli;
    QStringList nameFilter{lyliExtension};

    if(!dir.exists()) {
        qDebug() << "Error extracting targets from directory: " << path << " provided doesnot exist";
        return;
    }

    dirFiles = dir.entryInfoList(nameFilter, QDir::Files);
    dirSubFolders = dir.entryInfoList({}, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QVector<FileInfo> files;
    QVector<FolderInfo> folders;

    for(QFileInfo dirFile: dirFiles){
        QString oldName = dirFile.filePath();
        QString newName = dirFile.filePath().replace(lyli, "");
        files << FileInfo{oldName, newName};
    }
    for(QFileInfo dirSubFolder: dirSubFolders){
        folders << FolderInfo{dirSubFolder.filePath()};
    }

    if(!files.isEmpty()) this->files << files;
    if(!folders.isEmpty()) newWorkLoad(folders);
    emit folderScanned(folder);
}

void FolderScanner::wrapup(){

    // if we exit by cancelling
    // we don't want to send any futher signals
    // that might indicate that everything is going on as usually
    if(isCancelled()) return;

    // let anyone concerned know work is finished
    emit workFinished();

    // pass on results from work done
    emit renameBatch(files);
}

void FolderScanner::scan(){

    // we work and break periodically so the eventloop
    // can handle signals coming in and other events

    QEventLoop eventLoop;
    while(hasNext()){
        for(int i=0; i<50 && hasNext(); i++){
            if(isCancelled()) return;
            scanFolder(next());
        }
        eventLoop.processEvents();
    }

    // if we done for good
    if(!hasNext()) wrapup();
}

void FolderScanner::onScanBatch(QVector<FolderInfo> folders){
    newWorkLoad(folders);
    scan();
}

void FolderScanner::onPrepForWork(){
    this->folders.clear();
    clearIsCancel();
}

void FolderScanner::onCancelled(){
    folders.clear();
    setIsCancelled();
}
