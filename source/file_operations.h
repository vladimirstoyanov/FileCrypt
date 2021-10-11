#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <QFile>
#include <QString>

class FileOperations
{
public:
    FileOperations();

    QByteArray readFile(const QString &filename);
    void writeFile (const QString &filename, const QByteArray &buffer);
};

#endif // FILE_OPERATIONS_H
