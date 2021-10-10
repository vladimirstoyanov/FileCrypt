#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QFile>
#include <QString>

class FileOperations
{
public:
    FileOperations();

    QByteArray readFile(const QString &filename);
    void writeFile (const QString &filename, const QByteArray &buffer);
};

#endif // FILEOPERATIONS_H
