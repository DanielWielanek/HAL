# HAL
Heavy ion Analysis Libraries

Framework for analysis of the collisions of heavy ions. 

## instalation
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

ROOT that you have on compuer
JSROOT_DIR - path to custom JavaScript Root
