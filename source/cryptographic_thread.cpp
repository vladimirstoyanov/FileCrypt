#include "cryptographic_thread.h"

CryptographicThread::CryptographicThread(QObject *parent):
    QThread(parent),
    cryptography(nullptr)
{
    this->setTerminationEnabled(true);
}

CryptographicThread::~CryptographicThread ()
{

}


void CryptographicThread::run()
{
    cryptography->execute();
    emit processFinished();
    exec();
}

void CryptographicThread::setCryptography (std::shared_ptr<ICryptography> cryptography)
{
    this->cryptography = cryptography; //copy assigment releases 'use_count' of previous object
}


