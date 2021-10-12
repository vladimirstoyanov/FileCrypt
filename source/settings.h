#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include "file_operations.h"

class Settings
{
public:
    Settings();
    virtual ~Settings ();
    QString getDestinationPath () const { return this->m_destinationPath; }
    void    setDestinationPath (const QString & destinationPath) { m_destinationPath = destinationPath; }
private:
    QString m_destinationPath;
    QString m_settingsFileName;

    void loadSettings();
    void saveSettings();

};

#endif // SETTINGS_H
