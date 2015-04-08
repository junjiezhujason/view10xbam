#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

// Need to know length of read and length of bar code
typedef std::array<char, 15> barCode_str;
typedef std::array<char, 150> read_str;
typedef std::multimap<barCode_char, read_char> mapRead;


void file_to_mapRead(const char* fname, mapRead& mapBarRead){
	std::ifstream file;
    uint64_t map_size;
    barCode_str tagData;  
    read_str read;

	file.open(fname, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
       printf("file_to_map: Cannot open the bammap file %s!\n", fname);
       exit(1);
    }

    file.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));
    printf("MapSize: %d\n", (int)map_size);
    for (int i = 0; i < map_size; i++) {
    	file.read(reinterpret_cast<char*>(&tagData), sizeof(tagData));
        file.read(reinterpret_cast<char*>(&read), sizeof(read));
        mapBarRead.insert(std::pair<read_str, barCode_str>(tagData, read)); 
    }
	file.close();
}

void printMap(mapRead m) {
    std::cout << "Print Map:"<< std::endl;
    int i = 0;
    for (mapRead::iterator it = m.begin(); it != m.end(); ++it) {
        printf("%d\t",i);
        std::cout << (*it).first << "\t\t" << (*it).second << std::endl;
        i++;
    }
}


int main(){
	const char fName[] = "test_bamfiles/sim_reads_aligned.bammap";
	mapRead MapLarge;

	file_to_mapRead(fName, MapLarge); // load uniqueKmerMap
    printMap(MapLarge);
	// iterate through the BAM file 

}