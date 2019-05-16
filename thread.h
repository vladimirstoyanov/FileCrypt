#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMessageBox>
#include <QList>
#include <QDebug>

#include <memory>
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

    void run();
    void setIsDecrypted(const bool isDecrypted);
    void setPassword(const QString &password);
    void setSourceDestionationFiles(const QList<std::shared_ptr<QString>> &source, const QList<std::shared_ptr<QString>> &destination);

private:
    QList<std::shared_ptr<QString>> m_destinationFiles;
    bool                            m_decrypted;
    QString                         m_password;
    QList<std::shared_ptr<QString>> m_sourceFiles;

private:
    void encryptFile(const char *in, const char *out, const char *passPhrase);
    void decryptFile(const char *in, const char *out, const char *passPhrase);

signals:
    void processFinished(const bool);
    void setLabel(const QString &);

};

#endif // THREAD_H
