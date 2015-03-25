#! /bin/bash

# all deployments use rsync so if previously synced, nothing will be copied !

# deploy from train Release build to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
rsync -avz ./bin/Release/Train /home/train/programs/real
rsync -avz ./bin/Release/Train train@train191:/home/train/programs/real/
rsync -avz ./bin/Release/Train train@train201:/home/train/programs/real/
rsync -avz ./bin/Release/Train train@train211:/home/train/programs/real/
rsync -avz ./bin/Release/Train train@train221:/home/train/programs/real/
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train191:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train191:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train201:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train201:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train211:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train211:/home/train/programs/lib/libboostlog/
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train221:/home/train/programs/lib/libprotobuf/
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train221:/home/train/programs/lib/libboostlog/

# deploy config files to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
rsync -avz ../../config/global/*.xml /home/train/config/global
rsync -avz ../../config/global/*.xml train@train191:/home/train/config/global
rsync -avz ../../config/global/*.xml train@train201:/home/train/config/global
rsync -avz ../../config/global/*.xml train@train211:/home/train/config/global
rsync -avz ../../config/global/*.xml train@train221:/home/train/config/global


echo "Post-build deployment of Server1 from Codeblock termnated !!! Check if errors ..."
