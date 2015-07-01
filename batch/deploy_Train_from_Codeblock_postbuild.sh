#! /bin/bash
# this file is used for post build in Codeblock
# all deployments use rsync so if previously synced, nothing will be copied !

# deploy train program to dev-VM and all trains
echo "==========sync Train program to local dev train"
rsync -avz ./bin/Release/Train /home/train/programs/real
echo "==========sync Train program to train 191"
rsync -avz ./bin/Release/Train train@Train191:/home/train/programs/real/
echo "==========sync Train program to train 201"
rsync -avz ./bin/Release/Train train@Train201:/home/train/programs/real/
echo "==========sync Train program to train 211"
rsync -avz ./bin/Release/Train train@Train211:/home/train/programs/real/
echo "==========sync Train program to train 221"
rsync -avz ./bin/Release/Train train@Train221:/home/train/programs/real/

# deploy libs to dev-VM and all trains
echo "==========sync protobuf lib to train 191"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@Train191:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 191"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@Train191:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 201"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@Train201:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 201"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@Train201:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 211"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@Train211:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 211"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@Train211:/home/train/programs/lib/libboostlog/
echo "==========sync protobuf lib to train 221"
rsync -arzvhP ../../lib/libprotobuf/libprotobuf.so* train@Train221:/home/train/programs/lib/libprotobuf/
echo "==========sync boost logs lib to train 221"
rsync -arzvhP ../../lib/libboostlog/libboost*.so train@Train221:/home/train/programs/lib/libboostlog/

# deploy from trainGUI program build to dev-VM and all trains
echo "==========sync TrainGUI files to local dev train"
rsync -avz ../../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI /home/train/programs/real
echo "==========sync TrainGUI files to train 191"
rsync -avz ../../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@Train191:/home/train/programs/real/
echo "==========sync TrainGUI files to train 201"
rsync -avz ../../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@Train201:/home/train/programs/real/
echo "==========sync TrainGUI files to train 211"
rsync -avz ../../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@Train211:/home/train/programs/real/
echo "==========sync TrainGUI files to train 221"
rsync -avz ../../Qt/build-TrainGUI-Desktop_Qt_5_4_1_GCC_64bit-Release/TrainGUI train@Train221:/home/train/programs/real/

# deploy config files to dev-VM and all trains /home/train sub-folders
echo "==========sync config files to local dev train"
rsync -avz ../../config/global/*.xml /home/train/config/global
#rsync -avz ../../config/local/*.xml /home/train/config/local
echo "==========sync config files to train 191"
rsync -avz ../../config/global/*.xml train@Train191:/home/train/config/global
#rsync -avz ../../config/local/*.xml train@Train191:/home/train/config/local
echo "==========sync config files to train 201"
rsync -avz ../../config/global/*.xml train@Train201:/home/train/config/global
#rsync -avz ../../config/local/*.xml train@Train201:/home/train/config/local
echo "==========sync config files to train 211"
rsync -avz ../../config/global/*.xml train@Train211:/home/train/config/global
#rsync -avz ../../config/local/*.xml train@train211:/home/train/config/local
echo "==========sync config files to train 221"
rsync -avz ../../config/global/*.xml train@Train221:/home/train/config/global
#rsync -avz ../../config/local/*.xml train@Train221:/home/train/config/local


echo "Post-build deployment of Train and TrainGUI from Codeblock termnated !!! Check if errors ..."
