#ifndef CBC_DECRYPT_H
#define CBC_DECRYPT_H

#include "aes.h"
#include "cryptography.h"
#include "file_operations.h"
#include "loading_window.h"

class CBCDecrypt: public ICryptography
{
public:
    CBCDecrypt(const std::vector<File> &files,
            const QString &destinationDirectory,
            const QString &m_password,
            std::shared_ptr<LoadingWindow> loadingWindow);
    virtual ~CBCDecrypt () = default;
    void execute () override;

private:
    std::shared_ptr<QAESEncryption>  m_aes;
    QString                          m_destinationDirectory;
    FileOperations                   m_fileOperations;
    std::shared_ptr<LoadingWindow>   m_loadingWindow;
    QString                          m_password;
    std::vector <File>               m_sourceFiles;

    void decrypt(const unsigned int index);
};

#endif // CBC_DECRYPT_H
