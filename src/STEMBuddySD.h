/**
 * STEMBuddySD.h
 *
 * Virtual MicroSD Card: ESP32 reads/writes files stored on the phone.
 * The phone acts as a virtual SD card — no physical SD module needed.
 *
 * Usage:
 *   buddy.sd.writeFile("log.csv", "time,temp\n");     // Create/overwrite file
 *   buddy.sd.appendFile("log.csv", "10:30,25.5\n");   // Append to file
 *   buddy.sd.readFile("log.csv");                      // Request file content
 *   buddy.sd.deleteFile("log.csv");                    // Delete file
 *   buddy.sd.exists("log.csv");                        // Check if file exists
 *   buddy.sd.listFiles();                              // Request file listing
 *
 *   // In loop(), check for responses:
 *   if (buddy.sd.available()) {
 *     String content = buddy.sd.readResult();
 *   }
 *   if (buddy.sd.fileExists()) { ... }
 */

#ifndef STEMBUDDY_SD_H
#define STEMBUDDY_SD_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySD {
public:
    STEMBuddySD() : _parent(nullptr), _fileExists(false), _fileSize(0),
                     _hasExistsResult(false), _hasReadResult(false),
                     _hasListResult(false) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Write (create/overwrite) a file on the phone's virtual SD storage.
     * @param filename  Name of the file (e.g., "data.csv", "config.json")
     * @param content   Text content to write
     */
    void writeFile(const char* filename, const char* content);
    void writeFile(const String& filename, const String& content) {
        writeFile(filename.c_str(), content.c_str());
    }

    /**
     * Write binary data to a file.
     * @param filename  Name of the file
     * @param data      Binary data buffer
     * @param len       Length of data in bytes
     */
    void writeFile(const char* filename, const uint8_t* data, size_t len);

    /**
     * Append text to an existing file (or create if it doesn't exist).
     * @param filename  Name of the file
     * @param content   Text content to append
     */
    void appendFile(const char* filename, const char* content);
    void appendFile(const String& filename, const String& content) {
        appendFile(filename.c_str(), content.c_str());
    }

    /**
     * Append binary data to a file.
     * @param filename  Name of the file
     * @param data      Binary data buffer
     * @param len       Length of data in bytes
     */
    void appendFile(const char* filename, const uint8_t* data, size_t len);

    /**
     * Request to read a file's contents from the phone.
     * The result arrives asynchronously — check available() and readResult().
     * @param filename  Name of the file to read
     */
    void readFile(const char* filename);
    void readFile(const String& filename) { readFile(filename.c_str()); }

    /**
     * Delete a file from the phone's virtual SD storage.
     * @param filename  Name of the file to delete
     */
    void deleteFile(const char* filename);
    void deleteFile(const String& filename) { deleteFile(filename.c_str()); }

    /**
     * Check if a file exists on the phone's virtual SD.
     * Result arrives asynchronously — check hasExistsResult() and fileExists().
     * @param filename  Name of the file
     */
    void exists(const char* filename);
    void exists(const String& filename) { exists(filename.c_str()); }

    /**
     * Request a listing of all files on the virtual SD.
     * File names arrive asynchronously via _onListEntry / _onListEnd.
     */
    void listFiles();

    // ─── Async result access ─────────────────────────────────────

    /** Check if read result data is available. */
    bool available() const { return _hasReadResult; }

    /** Get the result of the last readFile() call. Clears the buffer. */
    String readResult();

    /** Check if exists() result is available. */
    bool hasExistsResult() const { return _hasExistsResult; }

    /** Get the result of the last exists() call. Clears the flag. */
    bool fileExists();

    /** Get the size reported by the last exists() call (in bytes). */
    uint32_t fileSize() const { return _fileSize; }

    /** Check if file list result is available. */
    bool hasListResult() const { return _hasListResult; }

    /** Get the file list from the last listFiles() call. Clears the list. */
    String getFileList();

    // ─── Internal callbacks (called by STEMBuddy::_processMessage) ─

    void _onFileData(const uint8_t* data, uint8_t len);
    void _onFileDataEnd();
    void _onFileStatus(const uint8_t* data, uint8_t len);
    void _onListEntry(const uint8_t* data, uint8_t len);
    void _onListEnd();

private:
    STEMBuddy* _parent;

    // File exists result
    bool     _fileExists;
    uint32_t _fileSize;
    bool     _hasExistsResult;

    // File read result
    String   _readBuffer;
    bool     _hasReadResult;

    // File list result
    String   _listBuffer;
    String   _listEntryBuffer;
    bool     _hasListResult;
};

#endif
