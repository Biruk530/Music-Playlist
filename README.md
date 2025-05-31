# MP3 Playlist Manager with Lyrics Support

## Overview

The MP3 Playlist Manager with Lyrics Support is a sophisticated, console-based application developed in C++ to provide a robust solution for managing and navigating MP3 playlists. Built with a focus on functionality and extensibility, it utilizes a doubly-linked list data structure to enable seamless bidirectional traversal, persistent storage, and a comprehensive feature set. These include song creation, updating, deletion, searching, sorting, shuffling, playback control, and lyrics management. Designed as a lightweight, Windows-specific tool, it offers an intuitive, resource-efficient alternative to graphical music applications while serving as an educational platform for exploring advanced programming concepts.

## Table of Contents

- [Purpose and Benefits](#purpose-and-benefits)
- [Features](#features)
- [How It Works](#how-it-works)
- [Implementation Details](#implementation-details)
- [Advantages and Limitations](#advantages-and-limitations)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Future Enhancements](#future-enhancements)
- [Contributing](#contributing)
- [Contact](#contact)

## Purpose and Benefits

The MP3 Playlist Manager with Lyrics Support addresses the need for a streamlined, offline playlist management system with integrated audio playback and lyrics support. Its key benefits include:

1. **Comprehensive Playlist Management**: Offers full CRUD (Create, Read, Update, Delete) operations, along with advanced features like searching, sorting, and lyrics management.
2. **Offline Functionality**: Operates independently of an internet connection, ensuring accessibility in any environment.
3. **Resource Efficiency**: Minimal system resource consumption compared to GUI-based music applications.
4. **Educational Value**: Demonstrates advanced programming techniques, including data structures, file I/O, memory management, and multimedia integration.
5. **Extensibility**: Modular architecture facilitates future enhancements and customizations.

## Features

- **Song Management**:
  - Add single or multiple songs with automatic unique ID generation.
  - Update song metadata (title, artist, file path, lyrics).
  - Delete songs by ID with automatic ID renumbering.
  - Persistent storage in a binary file (`playlist.dat`).
- **Playback Control**:
  - Play, pause, stop, and navigate songs (next/previous) with O(1) complexity.
  - Toggle repeat mode for continuous playback.
- **Playlist Operations**:
  - Display the full playlist with markers for the currently playing song.
  - Shuffle songs using the Fisher-Yates algorithm for unbiased randomization.
  - Sort songs by title or artist.
  - Search songs by keywords in title, artist, or lyrics.
- **Lyrics Management**:
  - Add, update, and display lyrics for songs (supports loading from text files).
- **User Interface**:
  - Intuitive, text-based menu system for easy navigation.
  - Clear, concise status messages for user feedback.

## How It Works

The MP3 Playlist Manager employs a doubly-linked list to manage songs, enabling efficient navigation in both directions. Songs are stored in a binary file (`playlist.dat`) with fields for ID, title, artist, file path, and lyrics. The application loads the playlist at startup, maintains an in-memory representation for fast access, and automatically saves changes after modifications. Users interact through a console-based menu, selecting options to add, update, delete, search, sort, shuffle, play, or manage lyrics. The program integrates with the Windows Multimedia API (`winmm.lib`) for MP3 playback and ensures proper memory management and data persistence.

## Implementation Details

### Data Structures

- **Doubly-Linked List**:
  - **Node Struct**: Contains pointers to previous and next nodes, and a `Song` struct with fields for ID (integer), title (string), artist (string), file path (string), and lyrics (string).
  - **Global Pointers**: `head` (first node), `tail` (last node), and `current` (currently playing node) for efficient list management.
- **Song Struct**: Encapsulates song metadata (ID, title, artist, file path, lyrics).

### Algorithms

- **Fisher-Yates Shuffle**: Ensures unbiased randomization of playlist order with O(n) complexity.
- **Linear Search**: Used for searching songs by ID or keywords, with O(n) complexity.
- **Sorting**: Leverages `std::sort` from the C++ Standard Library to sort songs by title or artist, using a temporary vector for stability (O(n log n) complexity).

### File Handling

- **File Format**: Binary format storing song metadata (ID, title length, title, artist length, artist, etc.).
- **Operations**:
  - **Load**: Reads `playlist.dat` at startup, parsing binary data into a `Song` struct and constructing the doubly-linked list.
  - **Save**: Writes the current playlist to `playlist.dat` after add, update, delete, sort, or shuffle operations.
- **Error Handling**: Basic validation for file operations; assumes correct binary format.

### Memory Management

- Dynamic allocation for `Song` and `Node` objects using `new`.
- Proper deallocation of all nodes and songs upon program exit to prevent memory leaks.
- Careful pointer management to maintain list integrity during add, delete, and shuffle operations.

### Code Organization

- **Modular Functions**: Separate functions for each operation (e.g., `addSong`, `updateSong`, `shuffle`, `sortPlaylist`, `playSong`) for maintainability.
- **Global State**: Uses global pointers (`head`, `tail`, `current`) and booleans (`repeatMode`, `isPlaying`, `isPaused`) for simplicity, with potential for refactoring.
- **Dependencies**: Relies on the C++ Standard Library and `winmm.lib` for Windows multimedia support.

## Advantages and Limitations

### Advantages

1. **High Performance**: Doubly-linked list enables O(1) next/previous navigation.
2. **Persistent Storage**: Ensures playlist data persists across sessions.
3. **Comprehensive Functionality**: Supports a wide range of operations, including playback and lyrics management.
4. **Integrated Playback**: Utilizes `winmm.lib` for MP3 playback on Windows.
5. **Modular and Extensible**: Clean code structure facilitates future enhancements.
6. **Educational Tool**: Illustrates advanced concepts like doubly-linked lists, file handling, and multimedia programming.

### Limitations

1. **Console-Based Interface**: Lacks a graphical UI, which may reduce accessibility for non-technical users.
2. **Limited Error Handling**: Minimal input validation; assumes correct user input and file formats.
3. **Windows-Only**: Requires Windows due to `winmm.lib` dependency.
4. **Single Playlist Support**: Does not support multiple playlists or advanced organization.
5. **Basic Playback**: Relies on system codecs for MP3 playback, with potential compatibility issues.
6. **Global State Dependency**: Reliance on global variables may complicate scalability and testing.

## System Requirements

- **Compiler**: C++11-compatible compiler (e.g., g++ 4.8 or later with MinGW, or MSVC).
- **Operating System**: Windows (due to `winmm.lib` dependency).
- **Dependencies**: C++ Standard Library and `winmm.lib` (included with Windows).
- **Disk Space**: Minimal (~1 MB for the executable, plus space for `playlist.dat`).
- **Memory**: Minimal (~1-10 MB, depending on playlist size).
- **MP3 Codec**: System must support MP3 playback (test files in Windows Media Player; install K-Lite Codec Pack if needed).

## Installation

### Prerequisites

Ensure you have a C++ compiler installed:
- **Windows (MinGW)**: Install MinGW and add `g++` to your PATH.
- **Windows (Visual Studio)**: Use MSVC (included with Visual Studio).

### Steps

1. **Clone or Download**: Obtain the source code (`Music Mp3 PlayList with Lyrics.cpp`) by downloading the file.
2. **Compile**:
   - Using g++ (MinGW):
     ```bash
     g++ -o playlist "Music Mp3 PlayList with Lyrics.cpp" -lwinmm
     ```
   - Using Visual Studio: Open the `.cpp` file, ensure `winmm.lib` is linked (handled by `#pragma comment(lib, "winmm.lib")`), and build the project.
3. **Run**:
   - Execute the compiled binary (e.g., `playlist.exe`).
   - Ensure the working directory is writable for `playlist.dat`.

### Notes

- If compilation fails, verify that `winmm.lib` is linked and the C++11 standard is supported.
- Install a codec pack (e.g., K-Lite) if MP3 playback issues occur.

## Usage

1. **Startup**:
   - The program loads `playlist.dat` from the working directory if it exists.
   - If no file exists, it starts with an empty playlist.
2. **Main Menu**:
   ```
   === MP3 PLAYLIST MANAGER ===
   1. Add Song
   2. Add Multiple Songs
   3. Delete Song
   4. Update Song
   5. Play/Pause
   6. Stop
   7. Next Song
   8. Previous Song
   9. Show Playlist
   10. Shuffle
   11. Search
   12. Toggle Repeat
   13. Manage Lyrics
   14. Display Lyrics
   15. Sort Playlist
   16. Exit
   Choice:
   ```
3. **Operations**:
   - **Add Single Song**: Enter title, artist, and MP3 path; lyrics can be added from a file or manually.
   - **Add Multiple Songs**: Specify the number of songs, then enter details for each.
   - **Play/Pause**: Play or pause the current song using option 5.
   - **Stop**: Stop playback with option 6.
   - **Next/Previous**: Navigate with options 7 and 8.
   - **Show Playlist**: Lists all songs with IDs, titles, artists, and playback/lyrics status.
   - **Shuffle**: Randomizes song order.
   - **Toggle Repeat**: Enables/disables repeat mode.
   - **Sort Playlist**: Choose to sort by title or artist.
   - **Search**: Find songs by keywords in title, artist, or lyrics.
   - **Manage Lyrics**: Add or update lyrics for a song by ID.
   - **Display Lyrics**: View lyrics for a song by ID or the current song.
   - **Exit**: Saves the playlist and frees memory.
4. **File Persistence**:
   - Changes (add, update, delete, sort, shuffle) are saved to `playlist.dat` automatically.

### Example Workflow

1. Start the program: Run `playlist.exe`.
2. Add the prototype song: Select option 1, enter:
   ```
   Title: Moonlit Dreams
   Artist: Starry Vibes
   MP3 Path: C:\Music\sample.mp3
   Enter lyrics file path: C:\Music\Moonlit_Dreams_Lyrics.txt
   ```
3. View playlist: Select option 9 to see the song.
4. Play song: Select option 5 to play "Moonlit Dreams".
5. Sort: Select option 15, choose 1 to sort by title.
6. Exit: Select option 16 to save and exit.

## Future Enhancements

1. **Feature Expansions**:
   - Support for song duration and playlist statistics (e.g., total duration).
   - Multiple playlist management with load/save options.
   - Import/export playlists in standard formats (e.g., M3U).
   - Tagging or categorization for songs (e.g., genre, mood).
2. **User Interface Improvements**:
   - Color-coded output using ANSI escape codes for better readability.
   - Enhanced playlist display with additional metadata.
   - Interactive prompts with default values and validation.
3. **Advanced Functionality**:
   - Integration with cross-platform audio libraries (e.g., SDL_mixer) for broader compatibility.
   - Playlist backup and versioning.
   - Advanced search with regex or fuzzy matching.
4. **Technical Enhancements**:
   - Robust error handling for invalid inputs and file operations.
   - Configuration file for customizable settings (e.g., file path, default sort order).
   - Unit tests using frameworks like Catch2 or Google Test.
   - Refactor global variables into a class-based structure for scalability.
   - Optimize for larger playlists with alternative data structures (e.g., balanced trees).

## Contributing

We welcome contributions to enhance the MP3 Playlist Manager with Lyrics Support! To contribute:

1. **Fork the Repository**: Create your own copy of the project.
2. **Create a Feature Branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Commit Changes**:
   ```bash
   git commit -m "Add feature: description of changes"
   ```
4. **Push to Branch**:
   ```bash
   git push origin feature/your-feature-name
   ```
5. **Submit a Pull Request**:
   - Provide a detailed description of changes.
   - Include updates to this README, if applicable.
   - Add test cases to validate new functionality.

Please adhere to the project's coding style (e.g., consistent indentation, clear variable names) and ensure changes are well-tested.

## Contact

For questions, bug reports, or feature suggestions, please open an issue on the GitHub repository or contact the development team at [biruksolomonti@gmail.com](mailto:biruksolomonti@gmail.com).

---

**Note**: The MP3 Playlist Manager with Lyrics Support is an educational project designed to illustrate advanced programming concepts, including doubly-linked lists, file handling, multimedia integration, and algorithm implementation. It is not intended for production use without significant enhancements to error handling, security, and scalability. This README was last updated at 03:38 PM EAT on Saturday, May 31, 2025.

## Credits: Developed by:
### Leader
- Biruk Tibebe
### Members
- Addishiwot Abdisa
- Anene Hachalu
- Bereket Teketel
- Bahiru Gebeyehu
- Biniyam Birhanu