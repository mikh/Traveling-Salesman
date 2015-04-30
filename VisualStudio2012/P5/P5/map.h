#ifndef MAP_H
#define MAP_H

#include "city.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

#define BFS_LEVEL 5

class Map{
public:
	vector<City*> map;
	vector<City*> path_list;
	vector<vector<City*> > p_trix;
	vector<vector<int> > d_trix;
	
	int roads;
	int max_distance;



	Map();
	void add_road(string s_city, string e_city, int distance);
	void print_statistics();
	bool set_start_city(string name);

	bool greedy_pathing();

	vector<City*> BFS(City *start_city, int levels, bool find_unvisited);

	void FloydWarshall();
	void free_map();

private:

	City* start_city;


	City* find_city(string city_name, vector<City*> &list);
	int find_distance(string city_name, vector<City*> &c_list, vector<int> &d_list);

	void add_city(string &city_name);
	bool is_city_null(City &city);
	void delete_city(City* city, vector<City*> &list);

	void BellmanFord(vector<City*> &map, City* source);
	vector<City> get_path(City &source, City &dest);
	void return_home(City &source, City &home);

};



Map::Map(){
	roads = 0;
	max_distance = 0;
}


void Map::FloydWarshall(){
	//initialize matrices
	for (int ii = 0; ii < map.size(); ii++){
		vector<City *> p_row;
		vector<int> d_row;
		for (int jj = 0; jj < map.size(); jj++){
			int dd = find_distance(map[jj]->name, map[ii]->o_dest, map[ii]->o_dist);
			if (ii == jj){
				d_row.push_back(0);
				p_row.push_back(NULL);
			} else if(dd != -1){
				d_row.push_back(dd);
				p_row.push_back(map[ii]);
			} else{
				d_row.push_back(-1);
				p_row.push_back(NULL);
			}
		}
		d_trix.push_back(d_row);
		p_trix.push_back(p_row);
	}


}


bool Map::greedy_pathing(){
	vector<City*> remaining_city_list(map);
	City* cur_city = start_city;
	path_list.push_back(start_city);

	delete_city(cur_city, remaining_city_list);

	printf("Starting greedy pathing. Start City = %s. %d cities remaining.\n", cur_city->name.c_str(), remaining_city_list.size());

	
	while (remaining_city_list.size() > 0){
		int search_level = BFS_LEVEL;

		vector<City*> unvistited_cities = BFS(cur_city, search_level, true);

		printf("Performing search - %d unvisited cities found in a radius of %d\n", unvistited_cities.size(), search_level);
	
		while (unvistited_cities.size() == 0){
			search_level *= 2;
			unvistited_cities = BFS(cur_city, search_level, true);
			printf("Performing search - %d unvisited cities found in a radius of %d\n", unvistited_cities.size(), search_level);
		}

		vector<City*> submap = BFS(cur_city, search_level, false);
	
		BellmanFord(submap, cur_city);
	/*
	City smallest_path;
	int path_size = -1;
	for (int ii = 0; ii < unvistited_cities.size(); ii++){
	if (path_size == -1 || unvistited_cities[ii].pred_dist < path_size){
	smallest_path = unvistited_cities[ii];
	path_size = unvistited_cities[ii].pred_dist;
	}
	}

	vector<City> path = get_path(cur_city, smallest_path);

	for (int ii = 0; ii < path.size() - 1; ii++){
	path_list.push_back(path[ii]);
	path[ii].visited = true;
	}

	cur_city = smallest_path; */
		delete_city(cur_city, remaining_city_list);
		printf("%d cities remaining\n", remaining_city_list.size());
	}
	

	//return_home(cur_city, start_city);
	return true;
}


/*
void Map::return_home(City* source, City* home){
	int search_level = BFS_LEVEL;
	vector<City> submap = BFS(source, search_level, false);
	City c = find_city(home.name, submap);
	while (is_city_null(c)){
		search_level *= 2;
		submap = BFS(source, search_level, false);
		c = find_city(home.name, submap);
	}

	BellmanFord(submap, source);
	vector<City> path = get_path(source, home);
	for (int ii = 0; ii < path.size(); ii++){
		path_list.push_back(path[ii]);
		path[ii].visited = true;
	}
}
*/

vector<City> Map::get_path(City &source, City &dest){
	vector<City> reverse_path;

	City *cur_city = &dest;
	while (cur_city->name.compare(source.name) != 0){
		reverse_path.push_back(*cur_city);
		cur_city = cur_city->pred;
	}

	vector<City> path;
	for (int ii = 0; ii < reverse_path.size(); ii++){
		path.push_back(reverse_path.back());
		reverse_path.pop_back();
	}

	return path;
}


//used for programming simplicity. Dijkstra's would be more algorithmically efficient
void Map::BellmanFord(vector<City*> &submap, City* source){
	printf("Running Bellman-Ford Algorithm on submap of V=%d\n", submap.size());
	for (int ii = 0; ii < submap.size(); ii++){
		submap[ii]->pred = NULL;
		submap[ii]->pred_dist = -1;
	}
	source->pred_dist = 0;

	for (int ii = 0; ii < submap.size(); ii++){	//for each vertex
		printf("%.2f%% done.\n", (float)ii / (float)submap.size() * 100);
		for (int jj = 0; jj < submap.size(); jj++){		//for each edge
			for (int kk = 0; kk < submap[jj]->o_dest.size(); kk++){
				City* c = submap[jj]->o_dest[kk];
				City* dd = find_city(c->name, submap);
				if (dd == NULL){	//making sure edge is in the submap
					if (c->pred_dist > submap[jj]->pred_dist + map[jj]->o_dist[kk]){	//relax
						c->pred_dist = map[jj]->pred_dist + map[jj]->o_dist[kk];
						c->pred = map[jj];
					}
				}
			}
		}
	}
}



vector<City*> Map::BFS(City* start_city, int levels, bool find_unvisited){
	vector<City*> found_list;
	vector<City*> search_list;
	vector<City*> neighbor_list;
	vector<City*> seen_list;
	int cur_level = 1;

	search_list.push_back(start_city);
	seen_list.push_back(start_city);

	while (cur_level <= levels && search_list.size() > 0){
		for (int ii = 0; ii < search_list.size(); ii++){
			City* c = search_list[ii];
			printf("Searching for cities around %s has %d neighbors\n", c->name.c_str(), c->o_dest.size());
			for (int jj = 0; jj < c->o_dest.size(); jj++){
				City* dd = find_city(c->o_dest[jj]->name, seen_list);
				if (dd == NULL){
					if (find_unvisited){
						if (!(c->o_dest[jj]->visited))
							found_list.push_back(c->o_dest[jj]);
					}
					else{
						found_list.push_back(c->o_dest[jj]);
					}
					neighbor_list.push_back(c->o_dest[jj]);
					seen_list.push_back(c->o_dest[jj]);
				}
			}
			//char ccc;
			//cin >> ccc;
		}

		search_list.clear();

		for (int ii = 0; ii < neighbor_list.size(); ii++)
			search_list.push_back(neighbor_list[ii]);
		neighbor_list.clear();

		cur_level++;
	}

	return found_list;
}


bool Map::set_start_city(string city_name){
	start_city = find_city(city_name, map);
	if (start_city == NULL){
		printf("Cannot find start city\n");
		return false;
	}
	return true;
}


bool Map::is_city_null(City &city){
	if (city.name.compare("NULL_CITY") == 0)
		return true;
	return false;
}

void Map::add_road(string s_city, string e_city, int distance){
	roads++;
	max_distance += distance;
	City* city_s = find_city(s_city, map);
	if (city_s == NULL){
		add_city(s_city);
		city_s = map[map.size() - 1];
	}
	City* city_e = find_city(e_city, map);
	if (city_e == NULL){
		add_city(e_city);
		city_e = map[map.size() - 1];
	}

	if (!city_s->add_destination(city_e, distance))
		printf("Duplcate roads\n");

	if (!city_e->add_origin(city_s, distance))
		printf("Duplcate roads\n");
}

City* Map::find_city(string city_name, vector<City*> &list){
	for (int ii = 0; ii < list.size(); ii++){
		if (list[ii]->name.compare(city_name) == 0)
			return list[ii];
	}
	return NULL;
}

int Map::find_distance(string city_name, vector<City*> &c_list, vector<int> &d_list){
	for (int ii = 0; ii < c_list.size(); ii++){
		if (c_list[ii]->name.compare(city_name) == 0)
			return d_list[ii];
	}
	return -1;
}

void Map::add_city(string &city_name){
	map.push_back(new City(city_name));
	//new_city = map[map.size()-1];
}

void Map::print_statistics(){
	printf("Map contains: %d cities, %d roads, with a max distance of %d\n", map.size(), roads, max_distance);
}

void Map::free_map(){
	for (int ii = 0; ii < map.size(); ii++){
		free(map[ii]);
	}
}


void Map::delete_city(City* city, vector<City*> &list){
	int index = -1;
	for (int ii = 0; ii < list.size(); ii++){
		if (list[ii]->name.compare(city->name) == 0){
			index = ii;
			break;
		}
	}
	if (index == -1)
		return;
	vector<City*>::iterator iter = list.begin();
	iter += index;

	list.erase(iter);
}

#endif