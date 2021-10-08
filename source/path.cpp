#include "path.h"

Path::Path()
{

}

bool Path::getFileNameByPath(const QString &path, QString &filename)
{
    QString separator, filename_tmp = "";
    separator = "/";

    if (separator.length()<1)
    {
        return false;
    }

    for (int i=path.length()-1; i>=0; --i)
    {
        if (path[i] == separator[0])
        {
            break;
        }
        if (0==i)
        {
            return false;
        }
        filename_tmp = path[i] + filename_tmp;
    }
    filename = filename_tmp;
    return true;
}

bool Path::getDirectoryNameByPath(const QString &path, QString &dirname)
{
    QString separator;
    separator = "/";

    int index= -1;
    for (int i=path.length()-1; i>=0; --i)
    {

        if (path[i] == separator[0])
        {
            index = i;
            break;
        }
    }
    if (-1 == index)
    {
        return false;
    }

    for (int i=0; i<=index; ++i)
    {
        dirname+=path[i];
    }

    return true;
}
