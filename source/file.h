#ifndef FILE_H
#define FILE_H

#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include <memory>

#include "file_operations.h"


class File
{
public:
    File(const QString &fileName,
         const QString &fileDirectory,
         const QString &path);
    QString getFileName () const { return this->m_fileName; }
    QString getPath () const { return this->m_path; }

private:
    QString         m_directoryName;
    QString         m_fileName;
    FileOperations  m_fileOperations;
    QString         m_path;
};

#endif // FILE_H
