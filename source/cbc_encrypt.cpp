#include "cbc_encrypt.h"

CBCEncrypt::CBCEncrypt(const std::vector<File> &files,
                 const QString &destinationDirectory,
                 const QString &password,
                 std::shared_ptr<LoadingWindow> loadingWindow):
    m_aes (std::make_shared <QAESEncryption>(QAESEncryption::AES_256, QAESEncryption::CBC)),
    m_destinationDirectory(destinationDirectory),
    m_loadingWindow(loadingWindow),
    m_password(password),
    m_sourceFiles(files)
{
    //percentage update slot and signal
    QObject::connect(m_aes.get(),
                     SIGNAL(percentageUpdated(const int)),
                     m_loadingWindow.get(),
                     SLOT(on_percentageUpdated(const int)));
}

void CBCEncrypt::execute ()
{
    for (unsigned int i = 0; i< m_sourceFiles.size(); ++i)
    {
        m_loadingWindow->setCurrentFile("Encrypting: " + m_sourceFiles[i].getFileName());
        encrypt(i);
    }
}

void CBCEncrypt::encrypt(const unsigned int index)
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
