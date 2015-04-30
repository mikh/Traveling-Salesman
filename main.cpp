#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include "map.h"
#include "city.h"

using namespace std;


//#define NUMBER_OF_ROADS 1346039
//#define NUMBER_OF_ROADS 83087
#define NUMBER_OF_ROADS 8660

#define FILENAME "super_small_map.txt"

#define START_CITY "ETCW"


void load_map_file(string filename, Map &map);
void greedy_search(Map &map);

int main(void){
	printf("Solving Problem 5\n");
	Map map;
	load_map_file(FILENAME, map);
	map.print_statistics();

	if(!map.set_start_city(START_CITY))
		return 0;

	map.greedy_pathing();

	printf("Done.\n");
	return 0;
}



void load_map_file(string filename, Map &map){
	ifstream infile;
	infile.open(filename.c_str());
	string line;
	int percent = 0;
	int line_count = 0;
	int percentage = NUMBER_OF_ROADS/100;

	printf("Loading File %s\n", filename.c_str());
	while(getline(infile, line)){
		line_count++;
		if(line_count % percentage == 0){
			percent++;
			printf("%d%% percent done\n", percent);
		}
		string city1 = line.substr(0,4);
		string city2 = line.substr(9,4);
		string distance = line.substr(15);
		map.add_road(city1, city2, atoi(distance.c_str()));
	}

	infile.close();
}

