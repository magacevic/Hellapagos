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
    void toggleMute();

    float getPreviousVolume() const;
    bool isSoundMuted() const;

private:
    QSoundEffect *backgroundMusic;  // The QSoundEffect for background music
    bool isMuted;                   // Flag to check if the sound is muted
    float previousVolume;           // To store the volume before muting

};

#endif // SOUNDMANAGER_H

