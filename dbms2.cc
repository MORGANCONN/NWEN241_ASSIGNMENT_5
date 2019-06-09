//
// Created by Connor on 20/05/2019.
//

#include "vdb.hh"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

using namespace dbms2;
using namespace std;

/**
 * Processes a csv and adds the data to the database
 * @param infn a pointer to the path of the file to load
 * @return returns true if the data is successfully loaded and returns false if there was an error
 */
bool AbstractDbTable::loadCSV(const char *infn) {
    ifstream loadedFile(infn);
    string line;
    vector<string> buffer;
    char *stopString;
    int counter = 0;
    int id = 1;
    if (loadedFile.is_open()) {
        while (getline(loadedFile, line)) {
            stringstream unSplit(line);
            while (getline(unSplit, line, ',')) {
                buffer.push_back(line);
                counter++;
                if (counter == 5) {
                    movie toAdd;
                    id >> toAdd.id;
                    strcpy(toAdd.title, buffer.at(0).c_str());
                    strcpy(toAdd.director, buffer.at(1).c_str());
                    stringstream(buffer.at(2)) >> toAdd.year;
                    stringstream(buffer.at(3)) >> toAdd.numberOfOscars;
                    strcpy(toAdd.country, buffer.at(4).c_str());
                    add(toAdd);
                    buffer.clear();
                    counter = 0;
                }
            }
            id++;
        }
        loadedFile.close();
        return true;
    }
    return false;
}


/**
 * Saves the current database as a .csv file
 * @param outfn a pointer to the path of the file to save to
 * @return returns true if the saving was successful, returns false if an error was encountered
 */
bool AbstractDbTable::saveCSV(const char *outfn) {
    ofstream output;
    output.open(outfn);
    if(!output.is_open()){
        return false;
    }
    output.clear();
    for (int i = 0; i < rows(); i++) {
        output << get(i)->id << "," << get(i)->title << "," << get(i)->year << "," << get(i)->director << '\n';
    }
    output.close();
    return true;
}

