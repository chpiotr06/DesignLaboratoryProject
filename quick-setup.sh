DEFAULT_FOLDER=$(pwd)

cd /home/$USERNAME/Downloads/
git clone https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH="/home/$USERNAME/Downloads/pico-sdk"
echo -e "Pico-SDK saved in /home/$USERNAME/Downloads"
sleep 1

git clone https://github.com/chpiotr06/DesignLaboratoryProject/tree/main --branch dev
cd DesignLaboratoryProject
mkdir build && cd build 
wget https://raw.githubusercontent.com/raspberrypi/pico-sdk/master/external/pico_sdk_import.cmake
cmake ..
make



cd $DEFAULT_FOLDER
