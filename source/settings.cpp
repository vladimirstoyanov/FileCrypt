#include "settings.h"

Settings::Settings():
    m_destinationPath (""),
    m_settingsFileName ("settings")
{
    loadSettings();
}

Settings::~Settings()
{
    saveSettings();
}

void Settings::loadSettings()
{
    FileOperations fileOperations;
    std::vector<QString> lines = fileOperations.readFileLines (m_settingsFileName);

    if (0 == lines.size())
    {
        return;
    }

    QStringList splited = lines[0].split(":");
    if (splited.size()<2 && splited[0]!="dist_path")
    {
        return;
    }

    m_destinationPath = splited[1];
    m_destinationPath = m_destinationPath.replace("\n","");
}

void Settings::saveSettings()
{
    FileOperations fileOperations;
    std::vector<QString> lines;
    lines.push_back("dist_path:" + m_destinationPath);
    fileOperations.writeFileLines(m_settingsFileName, lines);
}
