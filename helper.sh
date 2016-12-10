

path=/afs/zcu.cz/users/a/avrba/home/ups/jdk1.8.0_112/bin/java


"$path" -jar UPS-client/target/client-1.0-SNAPSHOT.jar



#! /bin/bash

cd UPS-server/Debug

./server2 config.txt


#!/bin/bash

rm -rf UPS-server
rm -rf UPS-client
rm

git clone https://github.com/Vrbikk/UPS-server.git
git clone https://github.com/Vrbikk/UPS-client.git

cd UPS-server
cmake ./
make
mv server2 Debug/server2
cd ..

path=/home/vrbik/Apps/jdk1.8.0_92/bin/
path_java=/home/vrbik/Apps/jdk1.8.0_92/bin/java

cd UPS-client
mvn -Dmaven.compiler.fork=true -Dmaven.compiler.executable="$path"javac package
cd ..

cd UPS-server/Debug
./server2 config.txt

"$path_java" -jar *.jar

export LD_PRELOAD=/home/vrbik/CloudStation/=FAV=/3rocnik/UPS/testing/socket_retarder/libsocket_retarder.so
export SOCKET_RETARDER_DEBUG=2
export SOCKET_RETARDER_DNS=0
export SOCKET_RETARDER_DISTRIBUTION=uniform
export SOCKET_RETARDER_UNIFORMDIST_A=250
export SOCKET_RETARDER_UNIFORMDIST_B=500

"$path_java" -jar -Djava.net.preferIPv4Stack=true *.jar



iptables -A INPUT -j REJECT
iptables -D INPUT 1
iptables -L --line-numbers
iptables -S