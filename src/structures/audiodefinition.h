#ifndef AUDIODEFINITION_H
#define AUDIODEFINITION_H

#include <QString>

class AudioDefinition
{
public:
    AudioDefinition(QString file = QString(), int startTime = int(), int endTime = int());

    QString file() const;
    int startTime() const;
    int endTime() const;

    bool operator==(const AudioDefinition &other) const;

private:
    QString m_file;
    int m_startTime;
    int m_endTime;
};

#endif // AUDIODEFINITION_H
