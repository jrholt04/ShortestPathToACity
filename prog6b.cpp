//File: prog6b.cpp
//Jacskon Holt, Transy U
//CS 2444, Fall 2023
//
//
//		This is a program for reading in and storing data in a graph and printing it out to a file 
//		using stl for list: https://www.geeksforgeeks.org/list-cpp-stl/
// 
//
#include <list>
#include <iostream>
#include <string> 
#include <fstream>
#include <stack>
using namespace std; 

//symbolic const
const int NO_NODE = -1;
const int BIG_NUMBER = 100000;
const int STARTING_CITY = 0;
const int COMPARISON = 200000;



// Node data for the links in the adj list
struct Node {
	int destinationCity;
	int weight;
};

//data structure for the adjacany list 
struct City {
	string city;
	list<Node> adjNodes;
	bool isFrozen;
	bool isIfinite;
	int crumb;
	int dist;
};

//prototypes 
void shortestPathAlg (City adjacencyList[], int numberOfCities);
bool unfronzenNodes (City adjacencyList[], int numberOfCities);
void printShortestPath(const City adjacencyList[], int destinationCity, int source, ofstream& outPutFile);
void printAllPaths(const City adjacencyList[], int numberOfCities, ofstream& outPutFile);
int findTheSmallestUnforzenIndex(City adjacencyList[], int numberOfCities);

////
///    Main Program 
//
int main (void){

	//intialize local variables 
	int numberOfCities; 
	string tempCity;
	string tempDestCity;
	Node tempNode; 
	ifstream cityInput;
	ofstream cityOutput; 
	
	
	//open the data file
	cityInput.open("cities.txt");
	if(!cityInput){
		cout << "error: cannot open cities.txt for reading " << endl;
		exit(1);
	} 

	//open the output file 
	cityOutput.open("prog6.txt");
	if(!cityOutput){
		cout << "error: cannot open prog6.txt for writting "<< endl;
	}

	//read in the integer value of the amount of cities 
	cityInput >> numberOfCities;

	//create an array with the number of cities
	City adjacencyList[numberOfCities];

	//fill our array with city structures to create our adjacencey list
	for (int i = 0; i < numberOfCities; i++){
		cityInput >> adjacencyList[i].city;
	}

	
	//fill out the list of each adjacncey list. 
	while ((cityInput >> tempCity)){ 
		//iterate over the adj list to see what list we are putting the city in
		for (int i =0; i < numberOfCities; i++){
			if (tempCity == adjacencyList[i].city){
				//once we find a match for the city then we read in the desitination city and iterate over the list to find what int to store it as
				cityInput >> tempDestCity; 
				for(int j = 0; j < numberOfCities; j++){
					//once we find what int to store it as 
					 if (tempDestCity == adjacencyList[j].city){
					 	
					 	tempNode.destinationCity = j;
					 	cityInput >> tempNode.weight; 
					 	//add this temp node with all the data into the adjancey list 
					 	adjacencyList[i].adjNodes.push_back(tempNode);
						
					 }
					 
				}
				
			}
			
		}
		 		
		
	}

	//set all of the nodes to false for frozen and is infinte to true and distance to zero and crumb to no_node
	//excpet the fisrt node set that to something else
	for (int i = 0; i < numberOfCities; i++){
		if(i > STARTING_CITY){
			adjacencyList[i].isFrozen = false;
			adjacencyList[i].isIfinite = true;
		}
	}
	//set the starting node to the requited values 
	adjacencyList[STARTING_CITY].dist = 0;
	adjacencyList[STARTING_CITY].isIfinite = false; 
	adjacencyList[STARTING_CITY].isFrozen = false; 

	

	//bredth first search to find the shortest path to each node and store the crumb	
	
	shortestPathAlg(adjacencyList, numberOfCities);
	printAllPaths(adjacencyList, numberOfCities, cityOutput);
	

	
	//close the files 
	cityInput.close();
	cityOutput.close();

	cout << "done "<< endl; 

	return 0; 
}


void shortestPathAlg (City adjacencyList[], int numberOfCities){
	int indexOfSmallestSoFar = 0;
	
	while (indexOfSmallestSoFar != NO_NODE){		
		//find the index of the smalles unfrozen node
		indexOfSmallestSoFar = findTheSmallestUnforzenIndex(adjacencyList, numberOfCities);
		//freeze the smallest unfrozen node 
		adjacencyList[indexOfSmallestSoFar].isFrozen = true;
		
		//if the index is not -1 or the sentinal loop through its adjacencey list 
		if (indexOfSmallestSoFar != NO_NODE){
			//iterates over the adjcenecey list of the frozen node
			list<Node>::iterator it;
			//for loop to itterate over the list of nodese of the just frozen data
			for (it = adjacencyList[indexOfSmallestSoFar].adjNodes.begin(); it != adjacencyList[indexOfSmallestSoFar].adjNodes.end(); ++it) {
				// see if the city is frozen or not 

				if (!adjacencyList[it -> destinationCity].isFrozen){
					//cout << adjacencyList[it -> destinationCity].city << " crumb is " << adjacencyList[it -> destinationCity].crumb << endl;
					if ((adjacencyList[it -> destinationCity].isIfinite)||(it -> weight + adjacencyList[indexOfSmallestSoFar].dist) < adjacencyList[it -> destinationCity].dist){
						//set A distance to N dist + arc weight
						adjacencyList[it -> destinationCity].dist = it -> weight + adjacencyList[indexOfSmallestSoFar].dist;
						//set A isInfinte to false
						adjacencyList[it -> destinationCity].isIfinite = false;
						//set A crumb to N
						adjacencyList[it -> destinationCity].crumb = indexOfSmallestSoFar;
					}
				}
			}
		}
		
	}
}


bool unfronzenNodes(City adjacencyList[], int numberOfCities){
	//check if the there are any unofrozen nodes in the list 
	for (int i = 0; i < numberOfCities; ++i){
		//if it is not frozen return true 
		if (!adjacencyList[i].isFrozen){
			return true; 
		}
	}
	//if not return false 
	return false; 
}

void printShortestPath(const City adjacencyList[], int destinationCity, int source, ofstream& outPutFile){
	stack<int> pathStack;
	int currentCity = source; 
	

	//push all the destinations onto the stack
	while (currentCity != destinationCity){
		pathStack.push(currentCity);
		currentCity = adjacencyList[currentCity].crumb;
	}

	//print a title 
	outPutFile << "Shortest path from " << adjacencyList[source].city << " to " << adjacencyList[destinationCity].city << " is: " << endl; 
	outPutFile << "Lexington ";
	//while loop to pop stack and print the list out 
	while (!pathStack.empty()){
		outPutFile << " -> " << adjacencyList[pathStack.top()].city << " ";
		pathStack.pop();
	}

	//finally output the length of the arcs to get there 
	outPutFile << "lenght: " << adjacencyList[source].dist << endl << endl; 

	



}

void printAllPaths(const City adjacencyList[], int numberOfCities, ofstream& outPutFile){
	//for all the unfrozen nodes print out the path and the length if not print not reachable 
	for (int i = 0; i < numberOfCities; ++i ){
		//if the node is not infinte print out the shortest path to it
		if (!adjacencyList[i].isIfinite){
			printShortestPath(adjacencyList, STARTING_CITY, i, outPutFile);
		}
		//else print the node is unreachable 
		else {
			outPutFile << adjacencyList[i].city << " is unreachable" << endl << endl; 
		}
	}
}

int findTheSmallestUnforzenIndex(City adjacencyList[], int numberOfCities){
	//init two int variables to keep track of smallest so far and the index 
	int smallestSoFar = BIG_NUMBER;
	int index = -1;
	//for all the cities in the array loop through
	for (int i = 0; i < numberOfCities; ++i){
		//if the array is not frozen, not infinte, and smaller then the current smallest then proceded 
		if (!adjacencyList[i].isFrozen){
			if(!adjacencyList[i].isIfinite){
				if (adjacencyList[i].dist < smallestSoFar){
					//if all conditions are meet then replace the smallest so far and index with valid data
					smallestSoFar = adjacencyList[i].dist;
					index = i;
				} 
			}	
		}
	}
	//if data is still sentinal values return the sentail index
	if (smallestSoFar == BIG_NUMBER){
		return index;
	}
	//else return a valid index 
	return index;
}
