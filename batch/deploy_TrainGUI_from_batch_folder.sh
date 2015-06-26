#! /bin/bash
# this file is used for post build in Qt Creator
# all deployments use rsync so if previously synced, nothing will be copied !

# deploy libs to dev-VM /home/train sub-folders
echo "==========sync protobuf lib to local dev train"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* /home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to local dev train"
rsync -arzvhP ../lib/libboostlog/libboost*.so /home/train/programs/lib/libboostlog/

# deploy from trainGUI Release build to dev-VM and all trains
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

# deploy config files to dev-VM and all trains
rsync -avz ../config/global/*.xml /home/train/config/global
rsync -avz ../config/local/*.xml /home/train/config/local
echo "==========sync config files to train 191"
rsync -avz ../config/global/*.xml train@Train191:/home/train/config/global
rsync -avz ../config/local/*.xml train@Train191:/home/train/config/local
echo "==========sync config files to train 201"
rsync -avz ../config/global/*.xml train@Train201:/home/train/config/global
rsync -avz ../config/local/*.xml train@Train201:/home/train/config/local
echo "==========sync config files to train 211"
rsync -avz ../config/global/*.xml train@Train211:/home/train/config/global
rsync -avz ../config/local/*.xml train@Train211:/home/train/config/local
echo "==========sync config files to train 221"
rsync -avz ../config/global/*.xml train@Train221:/home/train/config/global
rsync -avz ../config/local/*.xml train@Train221:/home/train/config/local

echo "Post-build deployment of Server1, Train and config files terminated !!! Check if errors !"
