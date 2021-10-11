#ifndef CBC_ENCRYPT_H
#define CBC_ENCRYPT_H

#include "aes.h"
#include "cryptography.h"
#include "file_operations.h"
#include "loading_window.h"


class CBCEncrypt: public ICryptography
{
public:
    CBCEncrypt(const std::vector<File> &files,
            const QString &destinationDirectory,
            const QString &m_password,
            std::shared_ptr<QAESEncryption> aes,
            std::shared_ptr<LoadingWindow> loadingWindow);
    virtual ~CBCEncrypt () = default;
    void execute () override;

private:
    std::shared_ptr<QAESEncryption>  m_aes;
    QString                          m_destinationDirectory;
    FileOperations                   m_fileOperations;
    std::shared_ptr<LoadingWindow>   m_loadingWindow;
    QString                          m_password;
    std::vector <File>               m_sourceFiles;

    void encrypt(const unsigned int index);
};

#endif // CBC_ENCRYPT_H
