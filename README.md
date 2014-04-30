rcpsp_simulated_annealing
=========================

Simulated Annealing implementation for the Resource Constrained Project Scheduling Problem

![asd sample](https://github.com/mrvaldes/rcpsp_simulated_annealing/raw/master/120.png)

Compile using make. The tree also contains a .pro file from QtCreator:
<pre>
$ cd sa-build-desktop/
$ make
</pre>

This will generate the binary executable 'sa'. If complilation produces errors, check/edit libs and flags. Choose a [PSP-LIB](http://129.187.106.231/psplib/) formatted input file in the sa-build-desktop/data/ folder and edit de generator script accordinglyand run it:
<pre>
$ nano generador.sh
$ bash generador.sh
</pre>

This will generate a launcher script for your tests, passing multiple parameters to the binary given the sets of the generator script:
<pre>
$ screen
$ bash output/longsearch60.sh
</pre>

At the end you will have files with solutions for the test in the output folder. For convenience parameters are indicated in the filename (like output/longsearch60/j601_2.sm_10_0.98_200_100_10000000_1) and in the file's header. Also system time usage is printed in the footer.
