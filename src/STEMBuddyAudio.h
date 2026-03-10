/**
 * STEMBuddyAudio.h
 *
 * Audio File Player: ESP32 tells the phone to play MP3/WAV files
 * stored on the phone's local storage.
 *
 * Usage:
 *   buddy.audio.play("alert.mp3");       // Play a file
 *   buddy.audio.play("music/song.mp3");  // Subfolder
 *   buddy.audio.stop();                  // Stop playback
 *   buddy.audio.pause();                 // Pause
 *   buddy.audio.resume();               // Resume
 *   buddy.audio.setVolume(80);           // 80%
 *   buddy.audio.setLoop(true);           // Loop playback
 */

#ifndef STEMBUDDY_AUDIO_H
#define STEMBUDDY_AUDIO_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyAudio {
public:
    STEMBuddyAudio() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Play an audio file stored on the phone.
     * Files should be imported into the STEM Buddy app first.
     * Supports MP3, WAV, OGG, and other browser-supported formats.
     * @param filename  Name of the file (e.g., "alert.mp3", "sounds/beep.wav")
     */
    void play(const char* filename);
    void play(const String& filename) { play(filename.c_str()); }

    /** Stop playback and reset position. */
    void stop();

    /** Pause playback (retains position). */
    void pause();

    /** Resume playback from paused position. */
    void resume();

    /**
     * Set playback volume.
     * @param volume  0-100
     */
    void setVolume(uint8_t volume);

    /**
     * Set loop mode.
     * @param loop  true = repeat, false = play once
     */
    void setLoop(bool loop);

private:
    STEMBuddy* _parent;
};

#endif
