#!/bin/bash
# build-simple-otel.sh 处理参数
if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
  echo "Usage: build-simple-otel.sh [OPTION*] [ARGV*], all options are optional"
  echo "Options:"
  echo "  --help, -h            Display help"
  echo "  --no-fmt              Do not install fmt library, default false"
  echo "  --fmt-version         Specify fmt version, default 11.0.1"
  echo "  --no-grpc             Do not install grpc library, default false"
  echo "  --local-grpc          Use local grpc library, specify the path of grpc library"
  echo "  --grpc-version        Specify grpc version, default v1.64.0"
  exit 0
fi

# 设置变量
NO_FMT=false
FMT_VERSION="11.0.1"
NO_GRPC=false
LOCAL_GRPC="/usr/local/grpc-install"
GRPC_VERSION="v1.64.0"
CPU_CORE_NUM=$(grep -c ^processor /proc/cpuinfo)

# 解析参数
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --no-fmt)
            NO_FMT="$2"
            echo "NO_FMT: $NO_FMT"
            shift 2
            ;;
        --fmt-version)
            FMT_VERSION="$2"
            echo "FMT_VERSION: $FMT_VERSION"
            shift 2
            ;;
        --no-grpc)
            NO_GRPC="$2"
            echo "NO_GRPC: $NO_GRPC"
            shift 2
            ;;
        --local-grpc)
            LOCAL_GRPC="$2"
            echo "LOCAL_GRPC: $LOCAL_GRPC"
            shift 2
            ;;
        --grpc-version)
            GRPC_VERSION="$2"
            echo "GRPC_VERSION: $GRPC_VERSION"
            shift 2
            ;;
        *)
            echo "Unknown parameter passed: $1"
            exit 1
            ;;
    esac
done

# 判断 是红帽还是ubuntu 准备基本依赖
if [ -f /etc/redhat-release ]; then
    # 项目环境为WSL下的Almalinux 9 请根据实际情况修改
    yum -y install gcc g++ cmake automake autoconf boost-devel libtool wget git
elif [ -f /etc/lsb-release ]; then
    sudo apt-get install -y git gcc g++ make cmake build-essential autoconf libtool pkg-config libboost-all-dev
else
  echo "Unknown linux distribution. Exiting..."
  exit 1
fi

# 如果/usr/bin/fmt存在且NO_FMT为false 则打印告警并需要手动Y确认
if [ -f /usr/bin/fmt ] && [ "$NO_FMT" == "false" ]; then
  # 通过fmt --version的首行输出获取版本号
  FMT_VERSION_INSTALLED=$(fmt --version | head -n 1 | awk '{print $4}')
  echo "fmt library already exists, version ${FMT_VERSION_INSTALLED}, do you want to install another version? [Y/n]"
  read -r REINSTALL
  if [ "$REINSTALL" == "n" ]; then
    NO_FMT="true"
  fi
fi

# 准备fmt库
if [ "$NO_FMT" == "false" ]; then
  echo "Prepare fmt library"
  cd /usr/local || exit
  # 带上版本
  wget "https://github.com/fmtlib/fmt/archive/refs/tags/${FMT_VERSION}.tar.gz"
  tar -zxvf "${FMT_VERSION}.tar.gz"
  rm -rf "${FMT_VERSION}.tar.gz"
  cd "fmt-${FMT_VERSION}" || exit
  mkdir build
  cd build || exit
  cmake ..
  make -j 4 && make install
else
  echo "Skip install fmt library"
fi

# 准备grpc 参考 https://grpc.io/docs/languages/cpp/quickstart/
if [ "$NO_GRPC" == "false" ]; then
  echo "Prepare grpc library"
  # git clone耗时较长 请耐心等待
  cd /usr/local || exit
  mkdir -p grpc-install
  git clone --recurse-submodules -b "${GRPC_VERSION}" --depth 1 --shallow-submodules https://github.com/grpc/grpc
  # 执行这一步 否则构建失败
  git submodule update --remote
  cd grpc || exit
  mkdir -p cmake/build
  cd cmake/build || exit
  cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX="${LOCAL_GRPC}" -DABSL_PROPAGATE_CXX_STD=ON ../..
  make -j "${CPU_CORE_NUM}" && make install
else
  echo "Skip install grpc library"
fi

# 部署本项目
echo "Prepare simple-otel project"
cd /usr/local || exit
git clone https://github.com/wangminan/simple-otel.git
cd simple-otel || exit
mkdir -p cmake/build
cd cmake/build || exit
cmake -DCMAKE_PREFIX_PATH="${LOCAL_GRPC}" ../..
make -j "${CPU_CORE_NUM}"

echo "Build simple-otel project successfully!"