#include "path.h"

Path::Path()
{

}

QString Path::getFileNameByPath(const QString &path)
{
    QString separator, filename_tmp = "";
    separator = "/";

    if (separator.length()<1)
    {
        return "";
    }

    for (int i=path.length()-1; i>=0; --i)
    {
        if (path[i] == separator[0])
        {
            break;
        }
        if (0==i)
        {
            return "";
        }
        filename_tmp = path[i] + filename_tmp;
    }
    return filename_tmp;
}

QString Path::getDirectoryNameByPath(const QString &path)
{
    QString separator;
    separator = "/";

    QString dirname = "";
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
        return dirname;
    }

    for (int i=0; i<=index; ++i)
    {
        dirname+=path[i];
    }

    return dirname;
}
