#include "encrypt.h"

Encrypt::Encrypt(const std::vector<File> &files,
                 const QString &destinationDirectory,
                 const QString &password ,
                 std::shared_ptr<QAESEncryption> aes, std::shared_ptr<LoadingWindow> loadingWindow):
    m_aes (aes),
    m_destinationDirectory(destinationDirectory),
    m_loadingWindow(loadingWindow),
    m_password(password),
    m_sourceFiles(files)
{

}

void Encrypt::execute ()
{
    for (unsigned int i = 0; i< m_sourceFiles.size(); ++i)
    {
        m_loadingWindow->setCurrentFile("Encrypting: " + m_sourceFiles[i].getFileName());
        encrypt(i);
    }
}

void Encrypt::encrypt(const unsigned int index)
{
    QString outFile = m_destinationDirectory +  m_sourceFiles[index].getFileName();

    //read
    QByteArray fileData = m_fileOperations.readFile( m_sourceFiles[index].getPath ());

    //ecrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(m_password.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodedText = m_aes->encode(fileData, hashKey, hashIV);

    //write
    m_fileOperations.writeFile (outFile, encodedText);
}
