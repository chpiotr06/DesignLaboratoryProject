# DesignLaboratoryProject

# Building


To build the project execute the following commands: 

```bash
git clone https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH="./pico-sdk"

git clone https://github.com/chpiotr06/DesignLaboratoryProject/tree/main
cd DesignLaboratoryProject
mkdir build && cd build 
wget https://raw.githubusercontent.com/raspberrypi/pico-sdk/master/external/pico_sdk_import.cmake
cmake ..
make

```

