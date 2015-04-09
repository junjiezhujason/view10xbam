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

void mapRead_to_file(const char* bFname, mapRead mRead, const char* mName){ // save a multimap to a file
    std::string fname(bFname);   
    fname += std::string(mName); 
    std::ofstream file (fname.c_str());
    if (!file.is_open()) {
       printf("map_to_file: Cannot open the .bammap file %s!\n", fname.c_str());
       exit(1);
    }

    uint64_t map_size = mRead.size();
    file << "> Number of Reads: " << map_size << "\n";
    for ( mapRead::iterator it = mRead.begin(); it != mRead.end(); ++it) {
        file << it->first << "\t" << it->second << "\n"; // [barcode, read]
    }
    file.close();
}

void submaps_to_files(const char* bFname, mapRead mRead, const int numBc) { // 
    int countBc = 0;    // barcode counter
    int totalBc = 0;    // total # of barcodes
    int totalFiles = 0; // number of file to created
    bool newBc = false; // flag indicator if the barcode is new
    barcode_str bc;     // bar code string
    mapRead MapSmall;   // small map
    char subfName[10];
    char buffer[7];
    // use the fact that the keys are sorted in multimaps 
    for ( mapRead::iterator it = mRead.begin(); it != mRead.end(); ++it) {
        if (bc.compare(it->first)) { // if this barcode is different from the previous
            newBc = true;
        } 
        if (newBc) {         
            if (countBc == numBc) { // if already encountered numBc # of barcodes
                // mapread_to_file  // save map to file
                strcpy(subfName, "map"); std::string s = std::to_string(totalFiles);strcat(subfName, s.c_str());  
                mapRead_to_file(bFname, MapSmall, subfName);
                totalFiles++;
                countBc = 0;        // reset counter 
                MapSmall.clear();   // reset multimap
            } 
            countBc++;
            totalBc++;     
        } 
        // insert the [barcode, read] to the multimap
        MapSmall.insert(std::pair<barcode_str, read_str>(it->first, it->second));
        bc = it->first; // keep a copy of this barcode for future comparison
        newBc = false;
    }
    strcpy(subfName, "map"); std::string s = std::to_string(totalFiles);strcat(subfName, s.c_str());  
    mapRead_to_file(bFname, MapSmall, subfName); // save the leftover counts in the last file
    totalFiles++;
    printf("Total Number of Barcodes: %d\n",totalBc);
    printf("Number of Files: %d\n", totalFiles);
}


int main(int argc, char* argv[])
{
    char* inputFileName;
    char barcodeName[] = "MD";
    int bcPerFile = 100;
    mapRead MapLarge;

    if ( argc == 2 ) {
            inputFileName  = argv[1] ;
    } else {
        std::cerr << "Wrong number of arguments." << std::endl;
        return 1;
    }

    bam_to_mapRead(inputFileName, MapLarge, barcodeName);
    submaps_to_files(inputFileName, MapLarge, bcPerFile);   // output maps in separate files
    mapRead_to_file(inputFileName, MapLarge, "mapAll"); // output the entire map

    return 0;
}