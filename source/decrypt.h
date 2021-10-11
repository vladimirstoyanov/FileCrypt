#ifndef DECRYPT_H
#define DECRYPT_H

#include "aes.h"
#include "cryptography.h"
#include "fileoperations.h"
#include "loadingwindow.h"

class Decrypt: public ICryptography
{
public:
    Decrypt(const std::vector<File> &files,
            const QString &destinationDirectory,
            const QString &m_password,
            std::shared_ptr<QAESEncryption> aes,
            std::shared_ptr<LoadingWindow> loadingWindow);

    void execute () override;

private:
    std::shared_ptr<QAESEncryption>  m_aes;
    QString            m_destinationDirectory;
    FileOperations     m_fileOperations;
    std::shared_ptr<LoadingWindow> m_loadingWindow;
    QString            m_password;
    std::vector <File> m_sourceFiles;

    void decrypt(const unsigned int index);
};

#endif // DECRYPT_H
