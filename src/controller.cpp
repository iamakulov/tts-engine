#include "controller.h"
#include "tokenizer.h"
#include "reassembler.h"
#include "transcriber.h"
#include "lettersplitter.h"
#include "phonetizer.h"
#include "audioconvertor.h"

Controller::Controller(QObject *parent)
    : QObject(parent)
{

}

void Controller::setTokenDefinitions(List<TokenDefinition> tokenDefinitions)
{
    m_tokenDefinitions = tokenDefinitions;
}

void Controller::setReassemblerRules(List<TransformationRule> reassemblerRules)
{
    m_reassemblerRules = reassemblerRules;
}

void Controller::setTranscriptionRules(List<TransformationRule> transcriptionRules)
{
    m_transcriptionRules = transcriptionRules;
}

void Controller::setLtsRules(List<LtsRule> ltsRules)
{
    m_ltsRules = ltsRules;
}

void Controller::setSoundToAudioRules(List<SoundToAudioRule> soundToAudioRules)
{
    m_soundToAudioRules = soundToAudioRules;
}

void Controller::speakText(const QString &text) const
{
    auto tokens = Tokenizer(m_tokenDefinitions).tokenize(text);
    auto reassembledTokens = Reassembler(m_reassemblerRules).reassemble(tokens);
    auto words = Transcriber(m_transcriptionRules).transcribe(reassembledTokens);
    auto letters = LetterSplitter().splitToLetters(words);
    auto sounds = Phonetizer(m_ltsRules).phonetize(letters);
    auto audios = AudioConvertor(m_soundToAudioRules).convert(sounds);
    m_player.play(audios);
}
