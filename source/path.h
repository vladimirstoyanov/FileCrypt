#ifndef PATH_H
#define PATH_H

#include <QString>

class Path
{
public:
    Path();

    QString getDirectoryNameByPath  (const QString &path);
    QString getFileNameByPath(const QString &path);
};

#endif // PATH_H
