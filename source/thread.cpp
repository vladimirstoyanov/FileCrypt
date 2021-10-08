#include "thread.h"

Thread::Thread(std::shared_ptr<QAESEncryption> aes, QObject *parent) :
    QThread(parent)
    , m_aes(aes)
    , m_decrypted(false)
    , m_password ("")
{
    this->setTerminationEnabled(true);
}

Thread::~Thread ()
{

}

void Thread::setSourceDestionationFiles(const std::vector<std::shared_ptr<QString> > &source, const std::vector<std::shared_ptr<QString> > &destination)
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
    qDebug()<<__PRETTY_FUNCTION__<<this->currentThreadId();

    if (m_sourceFiles.size()!=m_destinationFiles.size())
    {
        exec();
        emit processFinished(m_decrypted);
        return;
    }

    if (m_decrypted)
    {
        for (unsigned int i = 0; i< m_sourceFiles.size(); ++i)
        {
            emit setLable("Encrypting: " + *m_sourceFiles[i]);
            encryptFile(*m_sourceFiles[i], *m_destinationFiles[i], m_password);
        }
    }
    else
    {
        for (unsigned int i = 0; i<m_sourceFiles.size(); ++i)
        {
            emit setLable("Decrypting: " + *m_sourceFiles[i]);
            decryptFile(*m_sourceFiles[i], *m_destinationFiles[i], m_password);
        }
    }

    emit setLable("");
    emit processFinished(m_decrypted);
    exec();
}
void Thread::setPassword(const QString &password)
{
    this->m_password = password;
}

QByteArray Thread::readFile (const QString &filename)
{
    QByteArray buffer;
    QFile CurrentFile(filename);
    if(CurrentFile.open(QIODevice::ReadOnly))
    {
        return CurrentFile.readAll();
    }
    return buffer;
}

void Thread::writeFile (const QString &filename, const QByteArray &buffer)
{
    QFile outFile(filename);
    if (!outFile.open(QIODevice::WriteOnly)) {
            qDebug() << "can't open outFile";
            return;
    }
    outFile.write(buffer);
    outFile.close();
}

void Thread::encryptFile(const QString &inFile, const QString &outFile, const QString &key)
{
    //read
    QByteArray fileData = readFile(inFile);

    //ecrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodedText = m_aes->encode(fileData, hashKey, hashIV);

    //write
    writeFile (outFile, encodedText);
}

void Thread::decryptFile(const QString &inFile, const QString &outFile, const QString &key)
{
    //read
    QByteArray fileData = readFile(inFile);

    //decrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray decodedText = m_aes->decode(fileData, hashKey, hashIV);

    //write
    writeFile (outFile, decodedText);
}
