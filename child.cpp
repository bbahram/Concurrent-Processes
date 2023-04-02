#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;


void show(int** cities, int city_count)
{
	for (int i = 0; i < city_count; i++)
	{
		for (int j = 0; j < city_count; j++)
			cout << cities[i][j] << "	";
		cout << endl;
	}
}

bool ifis(int x, int* selected_city, int city_num)
{
	for (int i = 0; i < city_num; i++)
		if (x == selected_city[i])
			return true;
	return false;
}

void random_select(int city_count, int* selected_city, int city_num, int process_num)
{
	int i = 0;
	while (i < city_num)
	{
		int x = rand() % city_count;
		if (!ifis(x, selected_city, i))
		{
			selected_city[i] = x;
			i++;
		}
	}
}
void trans(int* a, int* b, int city_num)
{
	for (int i = 0; i < city_num; i++)
		a[i] = b[i];
}

int find_max(int** cities, int city_count, int* selected_city, int city_num)
{
	int max = 0, min = INT_MAX;
	for (int i = 0; i < city_count; i++, min = INT_MAX)
	{
		if (i != selected_city[i])
		{
			for (int j = 0; j < city_num; j++)
				if (min > cities[i][selected_city[j]])
					min = cities[i][selected_city[j]];
			//cout << "min: " << min << endl;
			if (max < min)
				max = min;
		}
	}
	return max;
}

int main()
{
	// shared memory
	TCHAR information[] = TEXT("info");
	TCHAR answers[] = TEXT("answers");
	TCHAR cit[] = TEXT("cities");
	HANDLE i = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, information);
	HANDLE a = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, answers);
	HANDLE c = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, cit);
	int* result;
	int* info;
	int* citi;
	info = (int *)MapViewOfFile(i, FILE_MAP_ALL_ACCESS, 0, 0, 5 * sizeof(int));
	result = (int *)MapViewOfFile(a, FILE_MAP_ALL_ACCESS, 0, 0, info[1] * sizeof(int));
	citi = (int *)MapViewOfFile(c, FILE_MAP_ALL_ACCESS, 0, 0, info[0] * info[1] * sizeof(int));
	int process_num = info[2];
	cout << "process number " << process_num + 1 << " is online" << endl;
	info[1] = -1;
	// main program*****************************************************
	int** cities = NULL, city_count, *selected_city = NULL, city_num = info[0], *answer = NULL, best = INT_MAX, max;
	ifstream f;
	f.open("C:\\Users\\bahram\\Desktop\\os\\os.txt");
	f >> city_count;
	cities = new int*[city_count];
	for (int i = 0; i < city_count; i++)
		cities[i] = new int[city_count];
	for (int i = 0; i < city_count; i++)
		for (int j = 0; j < city_count; j++)
			f >> cities[i][j];
	if (city_count < city_num)
		city_num %= city_count;
	selected_city = new int[city_num];
	answer = new int[city_num];
	for (int i = 0; i < city_num; i++)
		answer[i] = selected_city[i] = -1;
	int Time = time(NULL);
	int trycount = 0;
	srand(time(NULL) + process_num);
	while (time(NULL) - Time != info[3])
	{
		random_select(city_count, selected_city, city_num, process_num);
		max = find_max(cities, city_count, selected_city, city_num);
		if (best > max)
		{
			best = max;
			for (int i = 0; i < city_num; i++)
				answer[i] = selected_city[i];
		}
	}
	result[process_num] = best;
	for (int i = 0; i < city_num; i++)
		citi[(process_num*city_num)+i]=answer[i];
		
	UnmapViewOfFile(result);
	UnmapViewOfFile(info);
	UnmapViewOfFile(citi);
	CloseHandle(c);
	CloseHandle(a);
	CloseHandle(i);
	delete[] answer;
	delete[] selected_city;
	for (int i = 0; i < city_count; i++)
		delete[] cities[i];
	delete[] cities;
	answer = selected_city = NULL;
	cities = NULL;
	return 0;
}