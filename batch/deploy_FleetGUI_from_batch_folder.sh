#! /bin/bash
# this file is used for post build in Qt Creator
# all deployments use rsync so if previously synced, nothing will be copied !

# deploy libs to dev-VM /home/train sub-folders and operator1
echo "==========sync protobuf lib to local dev train"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* /home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to local dev train"
rsync -arzvhP ../lib/libboostlog/libboost*.so /home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to Operator1"
rsync -arzvhP ../lib/libprotobuf/libprotobuf.so* train@Operator1:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to Operator1"
rsync -arzvhP ../lib/libboostlog/libboost*.so train@Operator1:/home/train/programs/lib/libboostlog/

# deploy from FleetGUI Release build to dev-VM and Operator1
echo "==========sync FleetGUI files to local train dev train"
rsync -avz ../Qt/build-FleetGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/FleetGUI /home/train/programs/real
echo "==========sync FleetGUI files to Operator1"
rsync -avz ../Qt/build-FleetGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/FleetGUI train@Operator1:/home/train/programs/real/


# deploy config files to dev-VM and Operator1
echo "==========sync config files to local train"
rsync -avz ../config/global/*.xml /home/train/config/global
rsync -avz ../config/local/*.xml /home/train/config/local
echo "==========sync config files to Operator1"
rsync -avz ../config/global/*.xml train@Operator1:/home/train/config/global
rsync -avz ../config/local/*.xml train@Operator1:/home/train/config/local

echo "Post-build deployment of FleetGUI and config files terminated !!! Check if errors !"
