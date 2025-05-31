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