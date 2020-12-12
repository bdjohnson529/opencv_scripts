git clone https://github.com/opencv/opencv
git clone https://github.com/opencv/opencv_contrib

cd opencv
mkdir build
cd build
cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules/
make -j
sudo make install