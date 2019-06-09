//
// Created by Connor on 20/05/2019.
//
#include <iostream>
#include "vdb.hh"
#include <stdlib.h>

using namespace std;
using namespace dbms2;


/*
 * Returns the amount of rows
 */
int VectorDbTable::rows() const{
    return table.size();
}

/**
 * prints the requested entry if it exists and then returns an appropriate boolean
 * @param The index of the requested entry
 * @return Boolean indicating if it successfully printed an entry (True) or it was unable to find an entry (False)
 */
bool VectorDbTable::show(int index) const {
    if(index<0||index>table.size()){
        return false;
    }
    movie m = table.at(static_cast<unsigned int>(index));
    cout<<"Id:"<<m.id<<" Title:"<<m.title<<" Year:"<<m.year<<" Director:"<<m.director<<endl;
    return true;
}

/**
 * Adds an entry to the database
 * @param toAdd the movie to add
 * @return returns true if the movie was successfully added
 */
bool VectorDbTable::add(movie toAdd) {
    table.push_back(toAdd);
    return true;
}
/**
 * Removes an entry with the supplied id
 * @param id the id of the entry to be deleted
 * @return returns true if the removal was successful and false if it was not
 */
bool VectorDbTable::remove(unsigned long int id) {
    int index = 0;
    for(auto movies = table.begin();movies!=table.end();movies++){
        if(movies->id==id){
            table.erase(movies);
            return true;
        }
    }
    return false;
}
/**
 * returns a pointer to the movie at the provided index
 * @param index the index of the movie
 * @return a pointer pointing to the movie
 */
const movie* VectorDbTable::get(int index) const {
    if(index<0||index>table.size()){
        return nullptr;
    }
    return &table.at(static_cast<unsigned int>(index));
}


