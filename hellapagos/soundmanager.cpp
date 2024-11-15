#include "soundmanager.h"
#include <QSoundEffect>
#include <QUrl>
#include <QDebug>

SoundManager::SoundManager(QObject *parent)
    : QObject(parent),
    backgroundMusic(new QSoundEffect(this)),
    isMuted(false),                         //(starts as false = unmuted)
    previousVolume(0.5f)
{
}

SoundManager::~SoundManager()
{
    stopBackgroundMusic();
}

void SoundManager::playBackgroundMusic(const QString &filePath)
{
    QUrl musicUrl = QUrl::fromLocalFile(filePath);  // Path to your music file
    backgroundMusic->setSource(musicUrl);  // Set the music file
    backgroundMusic->setLoopCount(1);  // Loop indefinitely
    backgroundMusic->setVolume(previousVolume);  // Set the volume (0.0 to 1.0)
    backgroundMusic->play();  // Start playing the music
}

void SoundManager::stopBackgroundMusic()
{
    backgroundMusic->stop();  // Stop the background music
}

void SoundManager::setVolume(float volume)
{
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    backgroundMusic->setVolume(volume); // Adjust volume
    if (!isMuted) {
        previousVolume = volume;
    }
}

float SoundManager::getPreviousVolume() const
{
    return previousVolume;
}

void SoundManager::toggleMute()
{
    isMuted = !isMuted;

    if (isMuted) {
        previousVolume = backgroundMusic->volume();
        backgroundMusic->setVolume(0.0f);
        qDebug() << "Muted: Volume set to 0";
    } else {
        backgroundMusic->setVolume(previousVolume);
        qDebug() << "Unmuted: Volume restored to" << previousVolume;

    }
}

