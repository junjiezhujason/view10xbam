# view10xbam



do_bam: read and print entries of a bam file
wellMapGen: read entries of a bam file and output the map of data in a barcode 

`gcc -I /[PATH]/bamtools/include/ -L ~/[PATH]/bamtools/lib/ -o do_bam do_bam.cpp -lz -lbamtools -lstdc++`

`./do_bam test_bamfiles/sim_reads_aligned.bam`

