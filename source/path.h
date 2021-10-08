#ifndef PATH_H
#define PATH_H

#include <QString>

class Path
{
public:
    Path();

    bool  getDirectoryNameByPath  (const QString &path, QString &dirname);
    bool getFileNameByPath(const QString &path, QString &filename);
};

#endif // PATH_H
