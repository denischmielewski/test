#! /bin/bash

# all deployments use rsync so if previously synced, nothing will be copied !

# deploy from Server1 Release build to dev-VM /home/train sub-folders
rsync -avz ../Codeblock/Server1/bin/Release/Server1 /home/train/programs/real/
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* /home/train/programs/lib/libprotobuf/
rsync -arzvhP ../lib/libboostlog/libboost*.so /home/train/programs/lib/libboostlog/

# deploy from train Release build to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
rsync -avz ../Codeblock/Train/bin/Release/Train /home/train/programs/real
rsync -avz ../Codeblock/Train/bin/Release/Train train@train191:/home/train/programs/real/
rsync -avz ../Codeblock/Train/bin/Release/Train train@train201:/home/train/programs/real/
rsync -avz ../Codeblock/Train/bin/Release/Train train@train211:/home/train/programs/real/
rsync -avz ../Codeblock/Train/bin/Release/Train train@train221:/home/train/programs/real/
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train191:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train191:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train201:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train201:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train211:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train211:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train221:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train221:/home/train/programs/lib/libboostlog/

# deploy config files to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
rsync -avz ../config/global/*.xml /home/train/config/global
rsync -avz ../config/local/*.xml /home/train/config/local
rsync -avz ../config/global/*.xml train@train191:/home/train/config/global
rsync -avz ../config/local/*.xml train@train191:/home/train/config/local
rsync -avz ../config/global/*.xml train@train201:/home/train/config/global
rsync -avz ../config/local/*.xml train@train201:/home/train/config/local
rsync -avz ../config/global/*.xml train@train211:/home/train/config/global
rsync -avz ../config/local/*.xml train@train211:/home/train/config/local
rsync -avz ../config/global/*.xml train@train221:/home/train/config/global
rsync -avz ../config/local/*.xml train@train221:/home/train/config/local

read -p  "Post-build deployment of Server1, Train and config files termnated !!! Check if errors ...and press a key to terminate !"
