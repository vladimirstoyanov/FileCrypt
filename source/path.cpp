#include "path.h"

Path::Path():
    m_separator("/")
{

}

QString Path::getFileNameByPath(const QString &path)
{
    QString  fileName = "";

    for (int i=path.length()-1; i>=0; --i)
    {
        if (path[i] == m_separator[0])
        {
            break;
        }
        if (0==i)
        {
            return "";
        }
        fileName = path[i] + fileName;
    }
    return fileName;
}

QString Path::getDirectoryNameByPath(const QString &path)
{
    QString directoryName = "";
    int index = -1;
    for (int i=path.length()-1; i>=0; --i)
    {

        if (path[i] == m_separator[0])
        {
            index = i;
            break;
        }
    }
    if (-1 == index)
    {
        return directoryName;
    }

    for (int i=0; i<=index; ++i)
    {
        directoryName+=path[i];
    }

    return directoryName;
}
