#!/bin/bash
# 项目环境为WSL下的Almalinux 9 请根据实际情况修改
# 准备基本依赖
yum -y install gcc g++ cmake automake autoconf boost-devel libtool
# 准备fmt库
cd /usr/local || exit
wget https://github.com/fmtlib/fmt/archive/refs/tags/11.0.1.tar.gz
tar -zxvf 11.0.1.tar.gz
rm -rf 11.0.1.tar.gz
cd fmt-11.0.1 || exit
mkdir build
cd build || exit
cmake ..
make -j 4 && make install

# 准备grpc 参考 https://grpc.io/docs/languages/cpp/quickstart/
# git clone耗时较长 请耐心等待
git clone --recurse-submodules -b v1.64.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
# 执行这一步 否则构建失败
git submodule update --remote
cd grpc || exit
mkdir -p cmake/build
cd cmake/build || exit
cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/mnt/d/code/grpc-install -DABSL_PROPAGATE_CXX_STD=ON ../..
make -j 4 && make install

# 部署本项目
cd /usr/local || exit
git clone https://github.com/ZephyrZenn/simple-otel.git
cd simple-otel || exit
mkdir -p cmake/build
cd cmake/build || exit
cmake -DCMAKE_PREFIX_PATH=/mnt/d/code/grpc-install ../..
make -j 4