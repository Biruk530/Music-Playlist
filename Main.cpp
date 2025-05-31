#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <limits>

using namespace std;

// ========== STRUCT DECLARATIONS ==========
struct Song {
    int id;
    string title;
    string artist;
    string filePath;
    string lyrics;
};

struct Node {
    Song* song;
    Node* prev;
    Node* next;
};

// ========== GLOBAL VARIABLES ==========
Node* head = nullptr;
Node* tail = nullptr;
Node* current = nullptr;
bool repeatMode = false;
bool isPlaying = false;
bool isPaused = false;
const string playlistFile = "playlist.dat";
int nextId = 1;
MCIDEVICEID mciDevice = 0;

// ========== FORWARD DECLARATIONS ==========
void savePlaylist();
void loadPlaylist();
void displaySongs();
void addSong(string title, string artist, string path = "", string lyrics = "", bool saveFile = true);
void addMultipleSongs(int count);
void deleteSong(int id);
void playSong();
void stopPlayback();
void togglePause();
void playNext();
void playPrevious();
void shufflePlaylist();
void sortPlaylist();
void searchSongs(string query);
void updateSong(int id);
void manageLyrics(int id);
void displayLyrics(int id = -1);
void cleanUp();
int getValidInt();
bool isValidArtistName(const string& artist);

// ========== PLAYBACK CONTROL FUNCTIONS ==========
void stopPlayback() {
    if (mciDevice) {
        mciSendCommand(mciDevice, MCI_STOP, 0, 0);
        mciSendCommand(mciDevice, MCI_CLOSE, 0, 0);
        mciDevice = 0;
    }
    isPlaying = false;
    isPaused = false;
}
void playSong() {
    if (!current || current->song->filePath.empty()) {
        cout << "No song selected or no audio file.\n";
        return;
    }

    stopPlayback();

    MCI_OPEN_PARMS openParms = {0};
    openParms.lpstrDeviceType = "MPEGVideo";
    openParms.lpstrElementName = current->song->filePath.c_str();

    DWORD result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openParms);
    if (result != 0) {
        char errorMsg[256];
        mciGetErrorString(result, errorMsg, 256);
        cerr << "Error opening MP3 file '" << current->song->filePath << "': " << errorMsg << endl;
        openParms.lpstrDeviceType = "WaveAudio";
        result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openParms);
        if (result != 0) {
            mciGetErrorString(result, errorMsg, 256);
            cerr << "Fallback to WaveAudio failed: " << errorMsg << endl;
            return;
        }
    }

    mciDevice = openParms.wDeviceID;

    MCI_PLAY_PARMS playParams = {0};
    result = mciSendCommand(mciDevice, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&playParams);
    if (result != 0) {
        char errorMsg[256];
        mciGetErrorString(result, errorMsg, 256);
        cerr << "Error playing audio: " << errorMsg << endl;
        stopPlayback();
        return;
    }

    isPlaying = true;
    isPaused = false;
    cout << "Now playing: " << current->song->title << " (" << current->song->filePath << ")\n";
}

void togglePause() {
    if (!mciDevice) {
        cout << "No active playback.\n";
        return;
    }

    if (isPaused) {
        MCI_PLAY_PARMS playParams = {0};
        DWORD result = mciSendCommand(mciDevice, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&playParams);
        if (result == 0) {
            isPaused = false;
            cout << "Playback resumed\n";
        }
    } else {
        DWORD result = mciSendCommand(mciDevice, MCI_PAUSE, 0, 0);
        if (result == 0) {
            isPaused = true;
            cout << "Playback paused\n";
        }
    }
}