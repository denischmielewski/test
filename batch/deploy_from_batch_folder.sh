#! /bin/bash

# all deployments use rsync so if previously synced, nothing will be copied !

# deploy from Server1 Release build to dev-VM /home/train sub-folders
echo "==========sync Server1 program to local dev train"
rsync -avz ../Codeblock/Server1/bin/Release/Server1 /home/train/programs/real/
echo "==========sync protobuf lib to local dev train"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* /home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to local dev train"
rsync -arzvhP ../lib/libboostlog/libboost*.so /home/train/programs/lib/libboostlog/

# deploy from train Release build to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
echo "==========sync Train program to local dev train"
rsync -avz ../Codeblock/Train/bin/Release/Train /home/train/programs/real
echo "==========sync Train program to train 191"
rsync -avz ../Codeblock/Train/bin/Release/Train train@train191:/home/train/programs/real/
echo "==========sync Train program to train 201"
rsync -avz ../Codeblock/Train/bin/Release/Train train@train201:/home/train/programs/real/
echo "==========sync Train program to train 211"
rsync -avz ../Codeblock/Train/bin/Release/Train train@train211:/home/train/programs/real/
echo "==========sync Train program to train 221"
rsync -avz ../Codeblock/Train/bin/Release/Train train@train221:/home/train/programs/real/
echo "==========sync protobuf lib to train191"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train191:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train191"
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train191:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 201"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train201:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 201"
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train201:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 211"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train211:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 211"
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train211:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 221"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@train221:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 221"
rsync -arzvhP ../lib/libboostlog/libboost*.so train@train221:/home/train/programs/lib/libboostlog/

# deploy from trainGUI Release build to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
echo "==========sync TrainGUI files to local train dev train"
rsync -avz ../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI /home/train/programs/real
echo "==========sync TrainGUI files to train 191"
rsync -avz ../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@train191:/home/train/programs/real/
echo "==========sync TrainGUI files to train 201"
rsync -avz ../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@train201:/home/train/programs/real/
echo "==========sync TrainGUI files to train 211"
rsync -avz ../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@train211:/home/train/programs/real/
echo "==========sync TrainGUI files to train 221"
rsync -avz ../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@train221:/home/train/programs/real/

# deploy config files to:
#	dev-VM /home/train sub-folders
#	all trains /home/train sub-folders
echo "==========sync config files to local dev train"
rsync -avz ../config/global/*.xml /home/train/config/global
rsync -avz ../config/local/*.xml /home/train/config/local
echo "==========sync config files to train 191"
rsync -avz ../config/global/*.xml train@train191:/home/train/config/global
rsync -avz ../config/local/*.xml train@train191:/home/train/config/local
echo "==========sync config files to train 201"
rsync -avz ../config/global/*.xml train@train201:/home/train/config/global
rsync -avz ../config/local/*.xml train@train201:/home/train/config/local
echo "==========sync config files to train 211"
rsync -avz ../config/global/*.xml train@train211:/home/train/config/global
rsync -avz ../config/local/*.xml train@train211:/home/train/config/local
echo "==========sync config files to train 221"
rsync -avz ../config/global/*.xml train@train221:/home/train/config/global
rsync -avz ../config/local/*.xml train@train221:/home/train/config/local

read -p  "Post-build deployment of Server1, Train and config files termnated !!! Check if errors ...and press a key to terminate !"
