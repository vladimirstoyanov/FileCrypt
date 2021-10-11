#ifndef PATH_H
#define PATH_H

#include <QString>

class Path
{
public:
    Path();

    QString getDirectoryNameByPath  (const QString &path);
    QString getFileNameByPath(const QString &path);

private:
    QString m_separator;
};

#endif // PATH_H
