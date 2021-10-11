#include "cbc_decrypt.h"

CBCDecrypt::CBCDecrypt(const std::vector<File> &files,
                 const QString &destinationDirectory,
                 const QString &password,
                 std::shared_ptr<QAESEncryption> aes,
                 std::shared_ptr<LoadingWindow> loadingWindow):
    m_aes (aes),
    m_destinationDirectory(destinationDirectory),
    m_loadingWindow (loadingWindow),
    m_password(password),
    m_sourceFiles(files)
{

}

void CBCDecrypt::execute ()
{
    for (unsigned int i = 0; i< m_sourceFiles.size(); ++i)
    {
        m_loadingWindow->setCurrentFile("Decrypting: " + m_sourceFiles[i].getFileName());
        decrypt(i);
    }
}

void CBCDecrypt::decrypt(const unsigned int index)
{
    QString outFile = m_destinationDirectory +  m_sourceFiles[index].getFileName();

    //read
    QByteArray fileData = m_fileOperations.readFile(m_sourceFiles[index].getPath ());

    //decrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(m_password.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray decodedText = m_aes->decode(fileData, hashKey, hashIV);

    //write
    m_fileOperations.writeFile (outFile, decodedText);
}
