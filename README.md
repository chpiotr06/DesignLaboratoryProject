# DesignLaboratoryProject

# Building


To build the project execute the following commands: 

If you don't have pico-sdk added to environmental variable do the following:
```bash
git clone https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH="./pico-sdk"
```

Once you have PICO_SDK_PATH do:
```bash
git clone https://github.com/chpiotr06/DesignLaboratoryProject/tree/main
cd DesignLaboratoryProject
mkdir build && cd build 
wget https://raw.githubusercontent.com/raspberrypi/pico-sdk/master/external/pico_sdk_import.cmake
cmake ..
make

```

