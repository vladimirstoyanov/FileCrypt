#include "thread.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
{
}

void Thread::setSourceDestionationFiles(QList<QString *> source, QList<QString *> destination)
{
    l_source_files = source;
    l_destination_files = destination;
}

void Thread::isEncrypt(bool is_encrypt)
{
    encrypt = is_encrypt;
}

void Thread::run()
{
    qDebug()<<"run";
    if (l_source_files.size()!=l_destination_files.size())
    {
        exec();
        emit processFinished(encrypt);
        return;
    }

    if (encrypt)
    {
         //encryptFile(v.toString().toLatin1().data(),dest_path.toLatin1().data(),text.toLatin1().data());
        for (int i = 0; i< l_source_files.size(); i++)
        {
            emit setLabel("Encrypt: " + *l_source_files[i]);
            encryptFile(l_source_files[i]->toLatin1().data(), l_destination_files[i]->toLatin1().data(), password.toLatin1().data());
        }
    }
    else
    {
        for (int i = 0; i<l_source_files.size(); i++)
        {
            emit setLabel("Decrypt: " + *l_source_files[i]);
            decryptFile(l_source_files[i]->toLatin1().data(), l_destination_files[i]->toLatin1().data(), password.toLatin1().data());
        }
    }
    emit setLabel("");
    emit processFinished(encrypt);
    exec();
}
void Thread::setPassword(QString password_)
{
    password = password_;
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
