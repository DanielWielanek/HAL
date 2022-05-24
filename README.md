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
cmake -DCMAKE_INSTALL_PREFIX=&lt;place to install&gt; -DCMAKE_CXX_STANDARD=17 ..
</pre></code>
If there will be error Boost not found set path to boost by adding to cmake -DBOOST_ROOT=&lt;path to boost&gt; -DBOOST_DIR=&lt;path to boost headers&gt;