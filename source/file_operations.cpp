#include "file_operations.h"

FileOperations::FileOperations()
{

}

bool  FileOperations::isFileExist(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
          return false;
    }
    file.close();

    return true;
}

std::vector<QString> FileOperations::readFileLines(const QString &fileName)
{
    QFile file(fileName);
    std::vector <QString> lines;

    if(!file.open(QIODevice::ReadOnly))
    {
        return lines;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        lines.push_back( in.readLine());
    }

    file.close();

    return lines;
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

bool FileOperations::writeFile(const QString &filename, const QByteArray &buffer)
{
    QFile outFile(filename);
    if (!outFile.open(QIODevice::WriteOnly))
    {
            return false;
    }
    outFile.write(buffer);
    outFile.close();
    return true;
}

bool FileOperations::writeFileLines (const QString &filename, const std::vector<QString> &lines)
{
    bool result = false;
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        for (auto &item: lines)
        {
            stream<<item + "\n";
        }

        result=true;
    }
    file.close();
    return result;
}
