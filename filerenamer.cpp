#include "filerenamer.h"



FileRenamer::FileRenamer(){}

bool FileRenamer::hasNext(){
    return !files.isEmpty();
}

FileInfo FileRenamer::next(){
    FileInfo file;
    if(hasNext()){
        file = files.first();
        files.removeFirst();
    }
    return file;
}

void FileRenamer::addNewFilesToWorkLoad(QVector<FileInfo> files){
    this->files << files;
}

bool FileRenamer::renameFile(FileInfo fileInfo){

    if(isCancelled()) return true;

    QFile file;
    file.setFileName(fileInfo.oldName);
    if(file.exists()){
        if(file.rename(fileInfo.newName)){
            emit fileRenamed(fileInfo);
            return true;
        } else{
            qDebug() << "Error renaming file: rename failed";
            return false;
        }
    } else{
        qDebug() << "Error renaming file: file does not exist";
        return false;
    }
    return true;
}

void FileRenamer::renameFiles(){

    QEventLoop eventLoop;
    while(hasNext()){
        for(int i=0; i<50 && hasNext(); i++){
            if(isCancelled()) return;
            renameFile(next());
        }
        eventLoop.processEvents();
    }

    if(isSupplierFinished) emit allFinished();
}

void FileRenamer::onRenameBatch(QVector<FileInfo> files){
    addNewFilesToWorkLoad(files);
    renameFiles();
}

void FileRenamer::onSupplierFinished(){
    isSupplierFinished = true;
    onRenameBatch({});
}
