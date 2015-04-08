#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include "bam.h"

int main(){
   	const char fname[] = "read.bam";
   	bam1_t* b = bam_init1();
   	//bam_header_t *header;
   	//header = bam_header_read(fname);
   	bamFile in = bam_open(fname, "r");
   	bam_close(in);
}