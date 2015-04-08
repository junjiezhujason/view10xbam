#include <stdio.h>
#include <stdlib.h>
#include "api/BamReader.h"
#include "api/BamWriter.h"
#include <string>
#include <iostream>
#include <map>

// gcc -I ~/Shannon/bamtools/include/ -L ~/Shannon/bamtools/lib/ -o wellMapGen wellMapGen.cpp -lz -lbamtools -lstdc++
// ./wellMapGen test_bamfiles/sim_reads_aligned.bam

typedef std::string barcode_str;
typedef std::string read_str;
typedef std::multimap<barcode_str, read_str> mapRead;

void bam_to_mapRead(const char* fName, mapRead& m, const char* tName) { 
    // create a multimap: m, based on tag: tName, from .bam file
    std::string bamFname(fName);
    BamTools::BamReader reader;
    if (!reader.Open(bamFname)) {
        std::cerr << "Could not open input BAM file." << std::endl;
        exit(1);
    }

    char tagTypeName;
    barcode_str tagData;
    read_str read;

    BamTools::BamAlignment al;
    while (reader.GetNextAlignment(al)) {           // each BAM entry is processed in this loop
        if (al.GetTagType(tName, tagTypeName)) {    // ensure that tagType matches
            if (tagTypeName == 'Z') {               // verify that type is correct
                read = al.QueryBases;               // extract the read from entry
                al.GetTag(tName, tagData);          // extract the tag value from entry
                m.insert(std::pair<barcode_str, read_str>(tagData, read)); // add entry to multimap
            } 
            else {
                printf("Warning: tagType is not Z - entry ignored \n");
            }  
        } 
    }
    reader.Close();
}

void printMap(mapRead m) { // print the entire multimap
    std::cout << "Print Map:"<< std::endl;
    int i = 0;
    for (mapRead::iterator it = m.begin(); it != m.end(); ++it) {
        printf("%d\t",i);
        std::cout << (*it).first << "\t\t" << (*it).second << std::endl;
        i++;
    }
}

void mapRead_to_file(const char* bFname, mapRead mRead, const char* mName){ // save multimap to a file
    std::string fname(bFname);   
    fname += std::string(mName); 
    std::ofstream file (fname.c_str());
    if (!file.is_open()) {
       printf("map_to_file: Cannot open the .bammap file %s!\n", fname.c_str());
       exit(1);
    }

    uint64_t map_size = mRead.size();
    file << "> Mapsize: " << map_size << "\n";
    for ( mapRead::iterator it = mRead.begin(); it != mRead.end(); ++it) {
        file << it->first << "\t" << it->second << "\n"; // [barcode, read]
    }
    file.close();
}




int main(int argc, char* argv[])
{
    char*  inputFileName;
    if ( argc == 2 ) {
            inputFileName  = argv[1] ;
    }
    else {
        std::cerr << "Wrong number of arguments." << std::endl;
        return 1;
    }

    mapRead MapLarge, MapSmall;
    const char MapLargeName[] = "map";
    const char MapSmallName[] = "maptiny"; // MAP with small number of wells
    const char barcodeName[]  = "MD";

    bam_to_mapRead(inputFileName, MapLarge, barcodeName);
    //printMap(MapLarge); 
    mapRead_to_file(inputFileName, MapLarge, MapLargeName);

    //extract_smallMap(MapLarge); /
    //mapRead_to_file(inputFileName, MapSmall, MapSmallName);

    //printMap(MapLarge);
    //printMap(MapSmall);
    return 0;
}