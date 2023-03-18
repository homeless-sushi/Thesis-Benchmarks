rm -rf build
mkdir build
if [ $1 = BENCHMARK ]
then
    cmake -DBUILD_IS=BENCHMARK \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBOOST_ROOT="~/Documents/Thesis/margot/boost_1_60_0/install" \
        -Declipse-paho-mqtt-c_DIR="~/Documents/Thesis/margot/paho.mqtt.c/install/lib/cmake/eclipse-paho-mqtt-c/" \
        -Dmargot_heel_generator_DIR="~/Documents/Thesis/margot/core/install/lib/cmake/margot" \
        -S . -B build
elif [ $1 = PROFILING ]
then
    cmake -DBUILD_IS=PROFILING \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBOOST_ROOT="~/Documents/Thesis/margot/boost_1_60_0/install" \
        -Declipse-paho-mqtt-c_DIR="~/Documents/Thesis/margot/paho.mqtt.c/install/lib/cmake/eclipse-paho-mqtt-c/" \
        -Dmargot_heel_generator_DIR="~/Documents/Thesis/margot/core/install/lib/cmake/margot" \
        -S . -B build
fi
cmake --build build