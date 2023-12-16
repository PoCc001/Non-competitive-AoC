#include <iostream>
#include <chrono>

/*
* A word of warning especially for the low-level implementations: This program tries to be as fast as possible, so if a runtime-error occurs, the
* program will likely crash. Therefore, make sure that you place a file called '1' in the same directory as the executable generated from this source
* code. The file must only contain a valid input from the Advent of Code website from 12/01/2015! Also, the low-level code will only work on Windows
* (probably Windows 10 and 11), as it uses non-standard functions.
* 
* RUN THIS PROGRAM AT YOUR OWN RISK!!!
*/

#define USE_LOW_LEVEL_IO 2	// 2 = Windows API; 1 = other non-standard low-level library; 0 = standard C++ library
#define MEASURE_IO_TIME 0	// 0 = Don't measure the time of preparing the input (= loading it from the file to memory); else also measure the time loading the input

#if USE_LOW_LEVEL_IO == 0
#include <fstream>
#include <string>
#include <vector>
#elif USE_LOW_LEVEL_IO == 1
#include <fcntl.h>
#include <io.h>
#else
#include <Windows.h>
#endif

#define FILE_NAME "1"

using namespace std;

int main() {
#if MEASURE_IO_TIME != 0
	auto start = chrono::steady_clock::now();
#endif
#if USE_LOW_LEVEL_IO == 2
	HANDLE file = CreateFileA(FILE_NAME, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD size = GetFileSize(file, NULL);
	HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, size, NULL);
	unsigned char* content = (unsigned char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, size);
	CloseHandle(file);
#if MEASURE_IO_TIME == 0
	auto start = chrono::steady_clock::now();
#endif
	DWORD i = 0;
	int_fast32_t result = 81 * size;
	int_fast32_t sum = 0;

	for (; i < size; i++) {
		sum += content[i];
	}
	result -= sum << 1;
#elif USE_LOW_LEVEL_IO == 1
	int file = open(FILE_NAME, O_RDONLY);
	long size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);
	unsigned char* content = (unsigned char*)malloc(size);
	read(file, content, size);
	close(file);
#if MEASURE_IO_TIME == 0
	auto start = chrono::steady_clock::now();
#endif
	size_t i = 0;
	int_fast32_t result = 81 * size;
	int_fast32_t sum = 0;

	for (; i < size; i++) {
		sum += content[i];
	}
	result -= sum << 1;
#else
	ifstream file(FILE_NAME, ios::in);
	file.seekg(0, ios::end);
	size_t size = file.tellg();
	file.seekg(0, ios::beg);
	string strContent;
	getline(file, strContent);
	const char* content = strContent.c_str();
#if MEASURE_IO_TIME == 0
	auto start = chrono::steady_clock::now();
#endif
	int_fast32_t sum = 0;
	int_fast32_t result = 0;
	for (; result < size; result++) {
		sum += content[result];
	}
	result = result * 81 - (sum << 1);
#endif

	auto end = chrono::steady_clock::now();
	cout << "Part 1: " << result << " (" << chrono::duration_cast<chrono::nanoseconds> (end - start).count() / 1.0E+9 << " sec.)" << endl;

	start = chrono::steady_clock::now();
#if USE_LOW_LEVEL_IO == 1 || USE_LOW_LEVEL_IO == 2
	i = 0;
	result = 0;
	while (result != -1) {
		result += (81 - (content[i] << 1));
		i++;
	}
#else
	size_t i = 0;
	result = 0;
	while (result != -1) {
		result += (81 - (content[i] << 1));
		i++;
	}

#endif
#if MEASURE_IO_TIME == 0
	end = chrono::steady_clock::now();
#endif
#if USE_LOW_LEVEL_IO == 2
	UnmapViewOfFile(content);
#elif USE_LOW_LEVEL_IO == 1
	free(content);
#else
	file.close();
#endif
#if MEASURE_IO_TIME != 0
	end = chrono::steady_clock::now();
#endif
	cout << "Part 2: " << i << " (" << chrono::duration_cast<chrono::nanoseconds> (end - start).count() / 1.0E+9 << " sec.)" << endl;
	system("PAUSE");
	return 0;
}