# HAL
Heavy ion Analysis Libraries

Framework for analysis of the collisions of heavy ions. 

## prerequisites
This software requires:
 
 * ROOT 6
 * gls-lib (GNU scientific libraries)
 * cmake
 

## installation
1. enter your HAL directory, and make build directory, enter there:
<pre><code>
cd HAL
mkdir build
cd build
</pre></code>
2. Now call cmake to build software:
<pre><code>
cmake -DCMAKE_INSTALL_PREFIX=[place to install] .. <br>
make & make install
#optionally to use [N] cores
make -j[N] && make install
</pre></code>

Usually i install HAL in "inst" directory next to "build" directory:
<pre><code>
cmake -DCMAKE_INSTALL_PREFIX=../inst ..
</pre></code>

## usage
To use hal you should source the proper file:
<pre><code>
source [INSTALL_DIRECTORY]/bin/hal_config.sh
</pre></code>
## Additional parameters for cmake
Sometimes is required to set some additional parameters to HAL:

* CMAKE_CXX_STANDARD - sets cmake standard  by hand (default is 17) - note: this standard should be compatible with standard used for ROOT compilation. To check ROOT compilation flag type 
&lt;root-config --cflags&gt; e.g.: if output contains -std=c++17 it means that your ROOT was compiled with standard 17. In most cases it is not necessary to set this flag
* JSROOT_DIR=[path] - sets [path] to custom JS ROOT (can be used if HAL is not compatible with JSROOT installed with ROOT)
* FAIRROOT=OFF - disables compilation with FairRoot even if FairRoot is found in system
* EXAMPLES=ON - enable examples

Note: to use the option set -D[option] flag e.g., -DEXAMPLES=ON during calling cmake 


* Page @subpage femto_analysis
* Page @subpage subpage_2
* Page @subpage subpage_3

## Applications
This software contains not only libraries but also few applications:
* hal-report generates HTML report, unfortunatelly jsroot will not work directly you have to setup a sever or pretend that you
are setup the server (e.g., python3 -m http.server called inside of report directory)
* hal-merger for efficient merging of many HAL files
* hal-jobs to submit jobs on cluster without painfull BASH
* hal-cmake a helper for fast buidlign libraries/classes for cuts 


## To do
 * simplification of buffer class - using pointers instead of int maps?
 * std::fuction for custom class cuts?
 * generator of simple cuts on macro level