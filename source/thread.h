#ifndef THREAD_H
#define THREAD_H

#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QThread>

#include <fstream>
#include <memory>
#include <vector>

#include <QFile>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    virtual  ~Thread ();

    void run();
    void setIsDecrypted(const bool isDecrypted);
    void setPassword(const QString &password);
    void setSourceDestionationFiles(const QList<std::shared_ptr<QString>> &source,
                                    const QList<std::shared_ptr<QString>> &destination);

private:
    QList<std::shared_ptr<QString>> m_destinationFiles;
    bool                            m_decrypted;
    QString                         m_password;
    QList<std::shared_ptr<QString>> m_sourceFiles;

private:
    void encryptFile(const QString &inFile, const QString &outFile, const QString &key);
    void decryptFile(const QString &inFile, const QString &outFile, const QString &key);
    void readFile (const std::string &filename, std::vector<char> &buffer, size_t &size);
    void writeFile (const QString &filename, const QByteArray &buffer);

signals:
    void processFinished(const bool);
    void setLabel(const QString &);

};

#endif // THREAD_H
