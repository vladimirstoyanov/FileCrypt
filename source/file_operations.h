#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <QFile>
#include <QString>
#include <QTextStream>

class FileOperations
{
public:
    FileOperations();

    QByteArray            readFile(const QString &filename);
    std::vector<QString>  readFileLines(const QString &fileName);
    bool                  writeFile (const QString &filename, const QByteArray &buffer);
    bool                  writeFileLines (const QString &filename, const std::vector<QString> &lines);
};

#endif // FILE_OPERATIONS_H
