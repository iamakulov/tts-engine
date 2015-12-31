#include "audiodefinition.h"

AudioDefinition::AudioDefinition(QString file, int startTime, int endTime)
    : m_file(file), m_startTime(startTime), m_endTime(endTime)
{

}

QString AudioDefinition::file() const
{
    return m_file;
}

int AudioDefinition::startTime() const
{
    return m_startTime;
}

int AudioDefinition::endTime() const
{
    return m_endTime;
}

bool AudioDefinition::operator==(const AudioDefinition &other) const
{
    return m_file == other.m_file && m_startTime == other.m_startTime && m_endTime == other.m_endTime;
}

