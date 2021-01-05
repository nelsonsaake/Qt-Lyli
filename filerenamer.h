#ifndef FILERENAMER_H
#define FILERENAMER_H

#include "worker.h"
#include "fileinfo.h"

#include <QFile>
#include <QDebug>
#include <QEventLoop>



class FileRenamer : public Worker
{

    Q_OBJECT

public:
    FileRenamer();

protected:
    bool hasNext();

    FileInfo next();

    void addNewFilesToWorkLoad(QVector<FileInfo> files);

    bool renameFile(FileInfo fileInfo);

    void renameFiles();

public slots:
    void onRenameBatch(QVector<FileInfo> files);

    void onSupplierFinished();

signals:
    void fileRenamed(FileInfo file);
    void allFinished();

private:
    QVector<FileInfo> files;
    bool isSupplierFinished;
};

#endif // FILERENAMER_H
