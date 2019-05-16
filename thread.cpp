#include "thread.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
    , m_decrypted(false)
    , m_password ("")
{
}

void Thread::setSourceDestionationFiles(const QList<std::shared_ptr<QString> > &source, const QList<std::shared_ptr<QString> > &destination)
{
    m_sourceFiles = source;
    m_destinationFiles = destination;
}

void Thread::setIsDecrypted(const bool isDecrypted)
{
    this->m_decrypted = isDecrypted;
}

void Thread::run()
{
    qDebug()<<__PRETTY_FUNCTION__;

    if (m_sourceFiles.size()!=m_destinationFiles.size())
    {
        exec();
        emit processFinished(m_decrypted);
        return;
    }

    if (m_decrypted)
    {
        //encryptFile(v.toString().toLatin1().data(),dest_path.toLatin1().data(),text.toLatin1().data());
        for (int i = 0; i< m_sourceFiles.size(); i++)
        {
            emit setLabel("Encrypt: " + *m_sourceFiles[i]);
            encryptFile(m_sourceFiles[i]->toLatin1().data(), m_destinationFiles[i]->toLatin1().data(), m_password.toLatin1().data());
        }
    }
    else
    {
        for (int i = 0; i<m_sourceFiles.size(); i++)
        {
            emit setLabel("Decrypt: " + *m_sourceFiles[i]);
            decryptFile(m_sourceFiles[i]->toLatin1().data(), m_destinationFiles[i]->toLatin1().data(), m_password.toLatin1().data());
        }
    }

    emit setLabel("");
    emit processFinished(m_decrypted);
    exec();
}
void Thread::setPassword(const QString &password)
{
    this->m_password = password;
}

void Thread::encryptFile(const char *in,const char *out,const char *passPhrase)
{
    CryptoPP::FileSource f(in, true, new CryptoPP::DefaultEncryptorWithMAC(passPhrase,
    new CryptoPP::FileSink(out)));
}

void Thread::decryptFile(const char *in,const char *out,const char *passPhrase)
{
    CryptoPP::FileSource f(in, true,
    new CryptoPP::DefaultDecryptorWithMAC(passPhrase, new CryptoPP::FileSink(out)));
}
