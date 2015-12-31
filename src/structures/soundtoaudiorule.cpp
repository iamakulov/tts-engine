#include "soundtoaudiorule.h"

SoundToAudioRule::SoundToAudioRule(List<Token> sounds, AudioDefinition audio)
    : m_sounds(sounds), m_audio(audio)
{

}

List<Token> SoundToAudioRule::sounds() const
{
    return m_sounds;
}

AudioDefinition SoundToAudioRule::audio() const
{
    return m_audio;
}

bool SoundToAudioRule::operator==(const SoundToAudioRule &other) const
{
    return m_sounds == other.m_sounds && m_audio == other.m_audio;
}
