#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMessageBox>
#include <QList>
#include <QDebug>

#include <iostream>
#include <cryptopp/files.h>
#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/default.h>
#ifdef CRYPTOPP_WIN32_AVAILABLE
#include <windows.h>
#endif

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    void setSourceDestionationFiles(QList<QString*>source, QList<QString*> destination);
    void isEncrypt(bool is_encrypt);
    void run();
    void setPassword(QString password_);
private:
    QList<QString*> l_source_files;
    QList<QString*> l_destination_files;
    bool encrypt;
    QString password;

    void encryptFile(const char *in,const char *out,const char *passPhrase);
    void decryptFile(const char *in,const char *out,const char *passPhrase);
signals:
    void processFinished(bool);
    void setLabel(QString);
public slots:

};

#endif // THREAD_H
