#! /bin/bash

# all deployments use rsync so if previously synced, nothing will be copied !

# deploy from train Release build to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
echo "==========sync Train program to local dev train"
rsync -avz ./bin/Release/Train /home/train/programs/real
echo "==========sync Train program to train191"
rsync -avz ./bin/Release/Train train@train191:/home/train/programs/real/
echo "==========sync Train program to train201"
rsync -avz ./bin/Release/Train train@train201:/home/train/programs/real/
echo "==========sync Train program to train211"
rsync -avz ./bin/Release/Train train@train211:/home/train/programs/real/
echo "==========sync Train program to train221"
rsync -avz ./bin/Release/Train train@train221:/home/train/programs/real/
echo "==========sync protobuf lib to train191"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train191:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train191"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train191:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train201"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train201:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train201"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train201:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train211"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train211:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train211"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train211:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train221"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@train221:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train221"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@train221:/home/train/programs/lib/libboostlog/

# deploy config files to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
echo "==========sync config files to local dev train"
rsync -avz ../../config/global/*.xml /home/train/config/global
rsync -avz ../../config/local/*.xml /home/train/config/local
echo "==========sync config files to train 191"
rsync -avz ../../config/global/*.xml train@train191:/home/train/config/global
rsync -avz ../../config/local/*.xml train@train191:/home/train/config/local
echo "==========sync config files to train 201"
rsync -avz ../../config/global/*.xml train@train201:/home/train/config/global
rsync -avz ../../config/local/*.xml train@train201:/home/train/config/local
echo "==========sync config files to train 211"
rsync -avz ../../config/global/*.xml train@train211:/home/train/config/global
rsync -avz ../../config/local/*.xml train@train211:/home/train/config/local
echo "==========sync config files to train 221"
rsync -avz ../../config/global/*.xml train@train221:/home/train/config/global
rsync -avz ../../config/local/*.xml train@train221:/home/train/config/local


echo "Post-build deployment of Server1 from Codeblock termnated !!! Check if errors ..."
