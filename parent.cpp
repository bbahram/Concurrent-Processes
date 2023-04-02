#include <Windows.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <thread>
using namespace std;
int main()
{
	// create process
	STARTUPINFO* si = NULL;
	PROCESS_INFORMATION* pi = NULL;
	unsigned int cores = std::thread::hardware_concurrency();
	si = new STARTUPINFO[cores];
	pi = new PROCESS_INFORMATION[cores];
	for (int i = 0; i < cores; i++)
	{
		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si[i]);
		ZeroMemory(&pi[i], sizeof(pi[i]));
	}
	// shared memory
	TCHAR information[] = TEXT("info");
	TCHAR answers[] = TEXT("answers");
	TCHAR cit[] = TEXT("cities");
	HANDLE i = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4 * sizeof(int), information);
	HANDLE a = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, cores * sizeof(int), answers);
	int* result;
	int* info;
	result = (int *)MapViewOfFile(a, FILE_MAP_ALL_ACCESS, 0, 0, cores * sizeof(int));
	info = (int *)MapViewOfFile(i, FILE_MAP_ALL_ACCESS, 0, 0, 4 * sizeof(int));
	cout << "how many shops do u want to open? " << endl;
	cin >> info[0];
	cout << "how many seconds? " << endl;
	cin >> info[3];
	HANDLE c = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, info[0] * cores * sizeof(int), cit);
	int* citi;
	citi = (int *)MapViewOfFile(c, FILE_MAP_ALL_ACCESS, 0, 0, info[0] * cores * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < cores; i++)
	{
		info[1] = cores;
		info[2] = i;
		if (!CreateProcess(TEXT("C:\\Users\\bahram\\Desktop\\child.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i]))
			cout << "create process failed" << endl;
		while (info[1] == cores);
	}
	for (int i = 0; i < cores; i++)
		WaitForSingleObject(pi[i].hProcess, INFINITE);
	int min = INT_MAX, x;
	cout << endl << "the answers:" << endl;
	for (int i = 0; i < cores; i++)
		cout << "process number " << i + 1 << "'s result: " << result[i] << endl;
	cout << endl;
	for (int i = 0; i < cores; i++)
		if (min > result[i])
		{
			min = result[i];
			x = i;
		}
	cout << "the best answer: " << min << endl;
	cout << "cities:" << endl;
	for (int i = 0; i < info[0]; i++)
		cout<<i+1<<". "<<citi[(x*info[0])+i] << endl;
	UnmapViewOfFile(result);
	UnmapViewOfFile(info);
	UnmapViewOfFile(citi);
	CloseHandle(c);
	CloseHandle(a);
	CloseHandle(i);
	system("pause");
	return 0;
}
