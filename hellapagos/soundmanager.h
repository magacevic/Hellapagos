#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager();

    void playBackgroundMusic(const QString &filePath);
    void stopBackgroundMusic();
    void setVolume(float volume);

private:
    QSoundEffect *backgroundMusic;  // The QSoundEffect for background music
};

#endif // SOUNDMANAGER_H

