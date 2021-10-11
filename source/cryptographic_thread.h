#ifndef CRYPTOGRAPHIC_THREAD_H
#define CRYPTOGRAPHIC_THREAD_H

#include <QThread>

#include <memory>

#include "cryptography.h"
#include "null_cryptography.h"

class CryptographicThread : public QThread
{
    Q_OBJECT
public:
    explicit CryptographicThread(QObject *parent = 0);
    virtual  ~CryptographicThread ();

    void run();
    void setCryptography (std::shared_ptr<ICryptography> cryptography);

private:
    std::shared_ptr<ICryptography>  cryptography;

signals:
    void processFinished();
};

#endif // CRYPTOGRAPHIC_THREAD_H
