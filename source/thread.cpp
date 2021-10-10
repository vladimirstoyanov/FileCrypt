#include "thread.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
    , m_decrypted(false)
    , m_password ("")
{
    this->setTerminationEnabled(true);
}

Thread::~Thread ()
{

}

void Thread::setSourceFiles(const std::vector<File> &source)
{
    m_sourceFiles.clear();
    for (unsigned int i=0; i<source.size(); ++i)
    {
        m_sourceFiles.push_back(source[i]);
    }
}

void Thread::setDestinationFiles (const std::vector<QString> &destination)
{
    m_destinationFiles.clear();
    for (unsigned int i=0; i<destination.size(); ++i)
    {
        m_destinationFiles.push_back(destination[i]);
    }

}

void Thread::setIsDecrypted(const bool isDecrypted)
{
    this->m_decrypted = isDecrypted;
}

void Thread::run()
{
    qDebug()<<__PRETTY_FUNCTION__<<this->currentThreadId();

    if (m_sourceFiles.size()!=m_destinationFiles.size())
    {
        exec();
        emit processFinished(m_decrypted);
        return;
    }

    if (m_decrypted)
    {
        encryptFiles();
    }
    else
    {
        decryptFiles();
    }

    emit setLable("");
    emit processFinished(m_decrypted);
    exec();
}

void Thread::setPassword(const QString &password)
{
    this->m_password = password;
}


void Thread::decryptFiles()
{
    for (unsigned int i = 0; i<m_sourceFiles.size(); ++i)
    {
        emit setLable("Decrypting: " + m_sourceFiles[i].getFileName());
        m_sourceFiles[i].decrypt(m_destinationFiles[i], m_password);
    }
}

void Thread::encryptFiles()
{
    for (unsigned int i = 0; i< m_sourceFiles.size(); ++i)
    {
        emit setLable("Encrypting: " + m_sourceFiles[i].getFileName());
        m_sourceFiles[i].encrypt(m_destinationFiles[i], m_password);
    }
}



