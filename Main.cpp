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
// ========== PLAYLIST MANAGEMENT ==========
void savePlaylist() {
    ofstream file(playlistFile, ios::binary);
    if (!file.is_open()) {
        cerr << "Error saving playlist!\n";
        return;
    }

    for (Node* temp = head; temp; temp = temp->next) {
        size_t titleLen = temp->song->title.size();
        size_t artistLen = temp->song->artist.size();
        size_t pathLen = temp->song->filePath.size();
        size_t lyricsLen = temp->song->lyrics.size();

        file.write((char*)&temp->song->id, sizeof(int));
        file.write((char*)&titleLen, sizeof(size_t));
        file.write(temp->song->title.c_str(), titleLen);
        file.write((char*)&artistLen, sizeof(size_t));
        file.write(temp->song->artist.c_str(), artistLen);
        file.write((char*)&pathLen, sizeof(size_t));
        file.write(temp->song->filePath.c_str(), pathLen);
        file.write((char*)&lyricsLen, sizeof(size_t));
        file.write(temp->song->lyrics.c_str(), lyricsLen);
    }
    file.close();
}
// feature loadPlayList added by Bahiru
void loadPlaylist() {
    ifstream file(playlistFile, ios::binary);
    if (!file.is_open()) {
        cout << "No existing playlist found. Creating new one.\n";
        return;
    }

    while (file.peek() != EOF) {
        Song* s = new Song();
        size_t titleLen, artistLen, pathLen, lyricsLen;

        if (!file.read((char*)&s->id, sizeof(int))) {
            delete s;
            break;
        }
        if (s->id >= nextId) nextId = s->id + 1;

        if (!file.read((char*)&titleLen, sizeof(size_t))) {
            delete s;
            break;
        }
        s->title.resize(titleLen);
        if (!file.read(&s->title[0], titleLen)) {
            delete s;
            break;
        }

        if (!file.read((char*)&artistLen, sizeof(size_t))) {
            delete s;
            break;
        }
        s->artist.resize(artistLen);
        if (!file.read(&s->artist[0], artistLen)) {
            delete s;
            break;
        }

        if (!file.read((char*)&pathLen, sizeof(size_t))) {
            delete s;
            break;
        }
        s->filePath.resize(pathLen);
        if (!file.read(&s->filePath[0], pathLen)) {
            delete s;
            break;
        }

        if (!file.read((char*)&lyricsLen, sizeof(size_t))) {
            delete s;
            break;
        }
        s->lyrics.resize(lyricsLen);
        if (!file.read(&s->lyrics[0], lyricsLen)) {
            delete s;
            break;
        }

        Node* newNode = new Node{s, tail, nullptr};
        (head ? tail->next : head) = newNode;
        tail = newNode;
    }
    current = head;
    file.close();
}

void addSong(string title, string artist, string path, string lyrics, bool saveFile) {
    if (!path.empty()) {
        ifstream fileCheck(path);
        if (!fileCheck.good() || path.substr(path.length() - 4) != ".mp3") {
            cout << "Error: Invalid or inaccessible MP3 file path!\n";
            return;
        }
        fileCheck.close();
    }

    string validatedArtist = artist;
    while (!isValidArtistName(validatedArtist)) {
        cout << "Error: Artist name must contain only letters and spaces (no numbers or symbols).\n";
        cout << "Artist: ";
        getline(cin, validatedArtist);
    }

    string finalLyrics = lyrics;
    if (finalLyrics.empty()) {
        string lyricsPath;
        cout << "Enter lyrics file path (or press Enter for manual input): ";
        getline(cin, lyricsPath);
        if (!lyricsPath.empty()) {
            ifstream lyricsFile(lyricsPath);
            if (lyricsFile.is_open()) {
                string line;
                while (getline(lyricsFile, line)) {
                    finalLyrics += line + "\n";
                }
                lyricsFile.close();
            } else {
                cout << "Error: Could not open lyrics file. Enter lyrics manually (or press Enter to skip): ";
                getline(cin, finalLyrics);
            }
        } else {
            cout << "Enter lyrics (or press Enter to skip): ";
            getline(cin, finalLyrics);
        }
    }

    Song* newSong = new Song{nextId++, validatedArtist, title, path, finalLyrics};
    Node* newNode = new Node{newSong, tail, nullptr};

    if (!head) head = newNode;
    if (tail) tail->next = newNode;
    tail = newNode;

    if (saveFile) savePlaylist();
    cout << "Song added successfully!\n";
}

void sortPlaylist() {
    if (!head || !head->next) {
        cout << "Not enough songs to sort.\n";
        return;
    }

    cout << "Sort by:\n1. Title\n2. Artist\nChoice: ";
    int choice = getValidInt();
    cin.ignore();

    vector<Song*> songs;
    for (Node* temp = head; temp; temp = temp->next) {
        songs.push_back(temp->song);
    }

    if (choice == 1) {
        sort(songs.begin(), songs.end(), [](Song* a, Song* b) {
            return a->title < b->title;
        });
        cout << "Playlist sorted by title!\n";
    } else if (choice == 2) {
        sort(songs.begin(), songs.end(), [](Song* a, Song* b) {
            return a->artist < b->artist;
        });
        cout << "Playlist sorted by artist!\n";
    } else {
        cout << "Invalid choice. Sorting cancelled.\n";
        return;
    }

    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = tail = current = nullptr;

    for (Song* song : songs) {
        Node* newNode = new Node{song, tail, nullptr};
        if (!head) head = newNode;
        if (tail) tail->next = newNode;
        tail = newNode;
    }
    int newId = 1;
    for (Node* node = head; node; node = node->next) {
        node->song->id = newId++;
    }
    nextId = newId;

    if (head) current = head;

    savePlaylist();
}

void updateSong(int id) {
    Node* temp = head;
    while (temp && temp->song->id != id) {
        temp = temp->next;
    }

    if (!temp) {
        cout << "Song not found!\n";
        return;
    }

    string newTitle, newArtist, newPath, newLyrics;
    cout << "Current Title: " << temp->song->title << "\nNew Title (Enter to keep): ";
    getline(cin, newTitle);
    if (!newTitle.empty()) temp->song->title = newTitle;

    cout << "Current Artist: " << temp->song->artist << "\nNew Artist (Enter to keep): ";
    getline(cin, newArtist);
    if (!newArtist.empty()) {
        while (!isValidArtistName(newArtist)) {
            cout << "Error: Artist name must contain only letters and spaces (no numbers or symbols).\n";
            cout << "New Artist (Enter to keep): ";
            getline(cin, newArtist);
            if (newArtist.empty()) break;
        }
        if (!newArtist.empty()) temp->song->artist = newArtist;
    }

    cout << "Current Path: " << temp->song->filePath << "\nNew Path (Enter to keep): ";
    getline(cin, newPath);
    if (!newPath.empty()) temp->song->filePath = newPath;

    cout << "Current Lyrics:\n" << (temp->song->lyrics.empty() ? "No lyrics" : temp->song->lyrics) << "\nNew Lyrics (Enter to keep, or 'file' to load from file): ";
    getline(cin, newLyrics);
    if (newLyrics == "file") {
        string lyricsPath;
        cout << "Enter lyrics file path: ";
        getline(cin, lyricsPath);
        ifstream lyricsFile(lyricsPath);
        if (lyricsFile.is_open()) {
            string line;
            newLyrics = "";
            while (getline(lyricsFile, line)) {
                newLyrics += line + "\n";
            }
            lyricsFile.close();
            temp->song->lyrics = newLyrics;
        } else {
            cout << "Error: Could not open lyrics file. Keeping existing lyrics.\n";
        }
    } else if (!newLyrics.empty()) {
        temp->song->lyrics = newLyrics;
    }

    savePlaylist();
    cout << "Song updated successfully!\n";
}

void searchSongs(string query) {
    cout << "Search Results:\n";
    bool found = false;
    for (Node* temp = head; temp; temp = temp->next) {
        if (temp->song->title.find(query) != string::npos ||
            temp->song->artist.find(query) != string::npos ||
            temp->song->lyrics.find(query) != string::npos) {
            cout << temp->song->id << ". " << temp->song->title
                 << " - " << temp->song->artist << endl;
            found = true;
        }
    }
    if (!found) cout << "No matches found.\n";
}