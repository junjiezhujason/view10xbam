#include <stdio.h>
#include <stdlib.h>
#include "api/BamReader.h"
#include "api/BamWriter.h"
#include <string>
#include <iostream>
//git#include <map>

//using namespace std;
using namespace BamTools;

void maps_from_bam(std::string bamFname){
    BamReader reader;
    if (!reader.Open(bamFname)) {
        std::cerr << "Could not open input BAM file." << std::endl;
        exit(1);
    }

    char tagName[] = "MD"; // TO DO: Change the tagName to BX for 10x data
    char tagTypeName;
    std::string read;
    std::string tagData;  // if type_name is Z, then tagData should be a string
    
    int i = 0;
    BamAlignment al;
    while (reader.GetNextAlignment(al)) {
        printf("%d\t",i);
        read = al.QueryBases;
        al.GetTag(tagName, tagData);
        al.GetTagType(tagName, tagTypeName);
        // view
        printf("%s\t",read.c_str());
        printf("%s:", tagName);
        printf("%c:", tagTypeName); 
        printf("%s\n",tagData.c_str()); 
        i++;
    }
    reader.Close();
}

int main(int argc, char* argv[])
{
    std::string  inputFileName;
    if ( argc == 2 ) {
            inputFileName  = argv[1] ;
    }
    else {
        std::cerr << "Wrong number of arguments." << std::endl;
        return 1;
    }

//mapReads MapPartition2Read;
maps_from_bam(inputFileName);//, MapPartition2Read);
return 0;
}