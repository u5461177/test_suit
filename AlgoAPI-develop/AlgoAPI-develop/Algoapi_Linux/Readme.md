 　　完整版

## **服务器地址**

## 算法交易客户阿里云环境

smart.qilu-dev.com 22225 

## 算法交易生产环境

    smartsz.ztqft.com  28085 
    10.36.53.195


## **运行环境**

## docker 环境

    docker pull gcc:7.4
    docker run -it -v /opt/AlgoApi:/opt/AlgoApi gcc:7.4 bash  

**1. download**

    cd /opt
    # 1. websocket download
    git clone https://github.com/zaphoyd/websocketpp.git
    # 2. curl download
    wget https://curl.haxx.se/download/curl-7.63.0.tar.gz
    # 3. curlpp download
    git clone https://github.com/jpbarrette/curlpp.git
    # 4. cmake download: version>=3.10
    wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2.tar.gz
    # 5. boost download: version>=1.62.0    
    apt-get update && apt-get install libboost-all-dev

**2. boost install**

    cd /opt
    tar -xzvf boost_1_69_0.tar.gz
    cd /opt/boost_1_69_0
    ./bootstrap.sh
    ./b2 install

**3. cmake install**  

    cd /opt
    tar -xzvf cmake-3.13.2.tar.gz
    cd /opt/cmake-3.13.2
    ./bootstrap
    make
    make install  

**4. curl install**

    cd /opt
    tar -xzvf curl-7.63.0.tar.gz
    cd /opt/curl-7.63.0
    ./configure --with-ssl
    make
    make install
    
**5. curlppl install**

    cd /opt/curlpp
    mkdir build
    cd build
    cmake ..
    make
    make install  
**6. websocketpp install**

    cd /opt/websocketpp
    mkdir build
    cd build
    cmake ..
    make
    make install
      
**7. set env path**

    export LD_LIBRARY_PATH=/usr/local/lib

## Build
    cd /opt/AlgoApi/userdemo
    mkdir build
    cd /opt/AlgoApi/userdemo/build
    cmake..
    make
    
## Run
    ./main 

