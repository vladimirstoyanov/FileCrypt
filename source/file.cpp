#include "file.h"

File::File(const QString &fileName,
           const QString &fileDirectory,
           const QString &path,
           std::shared_ptr<QAESEncryption>  aes):
    m_aes (aes),
    m_directoryName (fileDirectory),
    m_fileName (fileName),
    m_path (path)
{

}

void File::encrypt(const QString &outFile, const QString &key)
{
    //read
    QByteArray fileData = m_fileOperations.readFile(m_path);

    //ecrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodedText = m_aes->encode(fileData, hashKey, hashIV);

    //write
    m_fileOperations.writeFile (outFile, encodedText);
}

void File::decrypt(const QString &outFile, const QString &key)
{
    //read
    QByteArray fileData = m_fileOperations.readFile(m_path);

    //decrypt
    QString iv("your-IV-vector"); //FIXME
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray decodedText = m_aes->decode(fileData, hashKey, hashIV);

    //write
    m_fileOperations.writeFile (outFile, decodedText);
}
