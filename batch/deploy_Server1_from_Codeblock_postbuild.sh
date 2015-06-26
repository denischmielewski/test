#! /bin/bash
# this file is used for post build in Codeblock
# all deployments use rsync so if previously synced, nothing will be copied !

# deploy from Server1 Release build to dev-VM and Server1
echo "==========sync Server1 program to local dev train"
rsync -avz ./bin/Release/Server1 /home/train/programs/real/
rsync -avz ./bin/Release/Server1 train@Server1:/home/train/programs/real/
echo "==========sync protobuf lib to local dev and Server1"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* /home/train/programs/lib/libprotobuf/
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@Server1:/home/train/programs/lib/libprotobuf/
echo "==========sync Boost Logs lib to local dev and Server1"
rsync -arzvhP ../../lib/libboostlog/libboost*.so /home/train/programs/lib/libboostlog/
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@Server1:/home/train/programs/lib/libboostlog/

# note: we do not deploy server program on trains VMs !

# deploy config files to dev-VM and Server1
echo "==========sync config file to local dev and Server1"
rsync -avz ../../config/global/*.xml /home/train/config/global
rsync -avz ../../config/global/*.xml train@Server1:/home/train/config/global
echo "Post-build deployment of Server1 from Codeblock termnated !!! Check if errors ..."

