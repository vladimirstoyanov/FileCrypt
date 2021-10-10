#ifndef FILE_H
#define FILE_H

#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include <memory>

#include "fileoperations.h"
#include "aes.h"

class File
{
public:
    File(const QString &fileName,
         const QString &fileDirectory,
         const QString &path, std::shared_ptr<QAESEncryption> aes);
    void encrypt(const QString &outFile, const QString &key);
    QString getFileName () const { return this->m_fileName; }
    void decrypt(const QString &outFile, const QString &key);

private:
    std::shared_ptr<QAESEncryption>  m_aes;
    QString         m_directoryName;
    QString         m_fileName;
    FileOperations  m_fileOperations;
    QString         m_path;
};

#endif // FILE_H
