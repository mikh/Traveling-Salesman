#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class City{
public:
	string name;
	bool visited;
	int unvisited_destinations;

	//Bellman Ford variables
	City *pred;
	int pred_dist;

	vector<City*> o_dest;	//cities with paths from this city
	vector<int> o_dist;

	vector<City*> i_dest;	//cities with paths to this city
	vector<int> i_dist;

	City();
	City(string c_name);
	bool add_destination(City *dest, int dist);
	bool add_origin(City *dest, int dist);

private:
	City* find_city(string &city_name, vector<City*> &list);
};

City::City(){
	name = "NULL_CITY";
	visited = false;
	unvisited_destinations = 0;
}

City::City(string c_name){
	name = c_name;
	visited = false;
	unvisited_destinations = 0;
}



bool City::add_destination(City* dest, int dist){
	City* c = find_city(dest->name, o_dest);
	if (c != NULL){
		return false;
	}
	o_dest.push_back(dest);
	o_dist.push_back(dist);
	unvisited_destinations++;
	return true;
}

bool City::add_origin(City* dest, int dist){
	City* c = find_city(dest->name, i_dest);
	if (c != NULL){
		return false;
	}
	i_dest.push_back(dest);
	i_dist.push_back(dist);
	return true;
}

City* City::find_city(string &city_name, vector<City*> &list){
	for (int ii = 0; ii < list.size(); ii++){
		if (list[ii]->name.compare(city_name) == 0)
			return list[ii];
	}
	return NULL;
}

#endif