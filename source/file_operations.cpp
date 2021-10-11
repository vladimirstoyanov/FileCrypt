#include "file_operations.h"

FileOperations::FileOperations()
{

}

QByteArray FileOperations::readFile (const QString &filename)
{
    QByteArray buffer;
    QFile CurrentFile(filename);
    if(CurrentFile.open(QIODevice::ReadOnly))
    {
        return CurrentFile.readAll();
    }
    return buffer;
}

void FileOperations::writeFile (const QString &filename, const QByteArray &buffer)
{
    QFile outFile(filename);
    if (!outFile.open(QIODevice::WriteOnly))
    {
            return;
    }
    outFile.write(buffer);
    outFile.close();
}
