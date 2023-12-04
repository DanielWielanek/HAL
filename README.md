# HAL
Heavy ion Analysis Libraries

Framework for analysis of the collisions of heavy ions. 

## installation
1. With FairRoot software
create build directory inside of HAL

<pre><code>
cmake -DCMAKE_INSTALL_PREFIX=&lt;place to install&gt;.. <br>
make & make install
</pre></code>
2. Without FairRoot
create build directory inside of HAL
<pre><code>
cmake -DCMAKE_INSTALL_PREFIX=&lt;place to install&gt; ..
</pre></code>
## additional parameters for cmake
CMAKE_CXX_STANDARD - set cmake standard  by hand (default is 17) - note: this standard should be compatbile with standard used for ROOT compilation. To check ROOT compilation flag type 
&lt;root-config --cflags&gt; e.g.: if output contains -std=c++17 it means that your ROOT was compiled with standard 17. 

JSROOT_DIR=[path] - sets path to custom JavaScript Root

FAIRROOT=OFF - disable compilation with FairRoot even if FairRoot is found in system

EXAMPLES=ON - enable examples

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