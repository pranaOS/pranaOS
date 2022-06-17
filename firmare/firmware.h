#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct regs_t {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
} regs_t;

extern int cursorX;
extern int cursorY;

/**
 * @brief abstraction call
 * 
 * @param num 
 * @param in 
 * @return regs_t 
 */
regs_t abstractionCall(int num, regs_t in);

/**
 * @brief setup abstraction library
 * 
 */
void setupAbstractionLibrary();


/**
 * @brief strlen
 * 
 * @param string 
 * @return int 
 */
int __strlen(char* string);

/**
 * @brief memcpy
 * 
 * @param destination 
 * @param source 
 * @param num 
 * @return void* 
 */
void* __memcpy(void* destination, const void* source, size_t num);

/**
 * @brief memset
 * 
 * @param b 
 * @param c 
 * @param len 
 * @return void* 
 */
void* __memset(void* b, int c, int len);

/**
 * @brief write hex
 * 
 * @param data 
 */
void writeHex(uint32_t data);

/**
 * @brief Get the File Data object
 * 
 * @param filename 
 * @param lbaOut 
 * @param lenOut 
 * @return true 
 * @return false 
 */
bool getFileData(char* filename, uint32_t* lbaOut, uint32_t* lenOut);

enum AbstractionCalls {
	ClearScreen,
	DriveType,
	ReadSector,
	WriteSector,
	ReadRAM,
	WriteCharacter,
	FindVideoMode,
	SetVideoMode,
	KeyboardBlocking,
	KeyboardNonblocking,
	BootFATInformation,
	QuarterSecondDelay,
	PagingSetup,
	DetectCPUFeatures,
};

/**
 * @brief TerminalColors with ids
 * 
 */
enum TerminalColours {
	Black = 0,
	Blue = 1,
	Green = 2,
	Teal = 3,
	Maroon = 4,
	Magenta = 5,
	Brown = 6,
	LightGrey = 7,
	DarkGrey = 8,
	Sky = 9,
	Lime = 10,
	Cyan = 11,
	Red = 12,
	Pink = 13,
	Yellow = 14,
	White = 15,
};


/**
 * @brief sleep
 * 
 * @param secs 
 */
void sleep(int secs);

/**
 * @brief millisleep
 * 
 * @param milli 
 */
void millisleep(int milli);

/**
 * @brief Set the Foreground Col object
 * 
 * @param col 
 */
void setFgCol(uint8_t col);

/**
 * @brief blocking keyboard
 * 
 * @return char 
 */
char blockingKeyboard();

/**
 * @brief nonblocking keyboard
 * 
 * @return char 
 */
char nonBlockingKeyboard();

/**
 * @brief beep
 * 
 * @param hz 
 */
void beep(int hz);

/**
 * @brief clear screen
 * 
 */
void clearScreen();

/**
 * @brief clear screen to color 
 * 
 * @param col 
 */
void clearScreenToColour(uint8_t col);

/**
 * @brief read sector from cd rom
 * 
 * @param sector 
 * @param data 
 */
void readSectorFromCDROM(uint32_t sector, uint8_t* data);

/**
 * @brief read and write sector
 * 
 * @param lba 
 * @param location 
 * @param disk 
 * @return true 
 * @return false 
 */
bool readSector(uint32_t lba, void* location, uint8_t disk);
bool writeSector(uint32_t lba, void* location, uint8_t disk);

/**
 * @brief write character
 * 
 * @param x 
 * @param y 
 * @param c 
 * @param bg 
 * @param fg 
 */
void writeCharacter(int x, int y, char c, uint8_t bg, uint8_t fg);

/**
 * @brief write string
 * 
 * @param s 
 */
void writeString(char* s);

/**
 * @brief write hex
 * 
 * @param data 
 */
void writeHex(uint32_t data);

/**
 * @brief execute from 
 * 
 * @param addr 
 */
void executeFrom(uint32_t addr);

/**
 * @brief get ram map
 * 
 * @param addr 
 * @return int 
 */
int getRAMMap(void* addr);

/**
 * @brief Get the First HDD object
 * 
 * @return uint8_t 
 */
uint8_t getFirstHDD();

/**
 * @brief get cd
 * 
 * @return uint8_t 
 */
uint8_t getCD();

/**
 * @brief Get the First Floppy object
 * 
 * @return uint8_t 
 */
uint8_t getFirstFloppy();

/**
 * @brief read fat from hdd
 * 
 * @param filename 
 * @param location 
 */
void readFATFromHDD(char* filename, void* location);

/**
 * @brief read iso from cd
 * 
 * @param filename 
 * @param location 
 */
void readISO9660FromCD(char* filename, void* location);

/**
 * @brief read fat file sector
 * 
 * @param filename 
 * @return uint32_t 
 */
uint32_t readFATFileSectorStart(char* filename);

#endif