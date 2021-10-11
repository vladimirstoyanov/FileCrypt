#ifndef CRYPTOGRAPHY_THREAD_H
#define CRYPTOGRAPHY_THREAD_H

#include <QDebug>
#include <QMessageBox>
#include <QThread>

#include <fstream>
#include <memory>
#include <vector>

#include "file.h"

class CryptographyThread : public QThread
{
    Q_OBJECT
public:
    explicit CryptographyThread(QObject *parent = 0);
    virtual  ~CryptographyThread ();

    void run();
    void setIsDecrypted(const bool isDecrypted);
    void setPassword(const QString &password);
    void setSourceFiles(const std::vector<File> &source);
    void setDestinationFiles (const std::vector<QString> &destination);

private:
    std::vector<QString>                  m_destinationFiles;
    bool                                  m_decrypted;
    QString                               m_password;
    std::vector<File>                     m_sourceFiles;

    void encryptFiles();
    void decryptFiles();

signals:
    void processFinished(const bool);
    void setLable(const QString &);

};

#endif // CRYPTOGRAPHY_THREAD_H
