#include "soundmanager.h"
#include <QSoundEffect>
#include <QUrl>

SoundManager::SoundManager(QObject *parent)
    : QObject(parent), backgroundMusic(new QSoundEffect(this))
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
    backgroundMusic->setVolume(0.5f);  // Set the volume (0.0 to 1.0)
    backgroundMusic->play();  // Start playing the music
}

void SoundManager::stopBackgroundMusic()
{
    backgroundMusic->stop();  // Stop the background music
}
