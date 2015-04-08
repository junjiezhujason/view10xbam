# view10xbam

Test with do_bam.cpp (on my Mac):
gcc -I /[PATH]/bamtools/include/ -L ~/[PATH]/bamtools/lib/ -o do_bam do_bam.cpp -lz -lbamtools -lstdc++
./do_bam test_bamfiles/sim_reads_aligned.bam 
