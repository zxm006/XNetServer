#!/bin/bash

export_env_ws1508()
{
    ROOT="/opt"
    export STAGING_DIR=$ROOT/staging_dir/target-arm_cortex-a5+neon_eglibc-2.19_eabi
    CROSS_TOOLCHAINS_DIR=$ROOT/staging_dir/toolchain-arm_cortex-a5+neon_gcc-4.8-linaro_eglibc-2.19_eabi

    if [ ! -x "$STAGING_DIR" ];then
        sudo tar xjvf ../toolchain/staging_dir.tar.bz2 -C /opt/
    fi

    export PATH=$PATH:${CROSS_TOOLCHAINS_DIR}/bin
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CROSS_TOOLCHAINS_DIR/usr/lib:$ROOT/build/lib
    export CROSS_COMPILE=arm-openwrt-linux-gnueabi-
}

export_env_mips()
{
    CROSS_TOOLCHAINS_DIR="/opt/trendchip/mips-linux-uclibc"
    export PATH=$PATH:${CROSS_TOOLCHAINS_DIR}/usr/bin
    export CROSS_COMPILE=mips-linux-
    echo "end export env mips"
}


init_env()
{
    ENV_DIR=$(dirname .)
    echo "path: $ENV_DIR"
    rm ${ENV_DIR}/default.cmake
    ln -s ${ENV_DIR}/${ARCH_MAKE} ${ENV_DIR}/default.cmake
    currdir=`pwd`
    updir=`dirname $currdir`
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${updir}/3rd/bin
    export PATH=$PATH:${updir}/3rd/bin
}

choose_info()
{
    echo
    echo "Please choose the arch you what build:"
    echo "   1.x86"
    echo "   2.armv7l"
    echo "   3.android"
    echo "   4.mips"
    echo ""
}

choose_arch()
{
    choose_info
    local DEFAULT_NUM DEFAULT_VALUE
    DEFAULT_NUM=1
    echo -n "Which one would you like ? ["$DEFAULT_NUM"] "
    if [ -z "$1" ]; then
        read ANSWER
    else
        echo $1
        ANSWER=$1
    fi

    case $ANSWER in
    1)
        curdir=`pwd`
        cd ${curdir}/../3rd
        rm lib
        ln -s libx86 lib
        cd ${curdir}
        export ARCH_MAKE="x86.cmake"
        export THIRD_LIB_PATH="x86_lib"
        ;;
    2)
        curdir=`pwd`
        cd ${curdir}/../3rd
        rm lib
        ln -s libarm lib
        cd ${curdir}
        export ARCH_MAKE="cross.cmake"
        export THIRD_LIB_PATH="boxv2_lib"
        export_env_ws1508
        ;;
    3)
        export ARCH_MAKE="android.cmake"
        ;;
    4)
        export ARCH_MAKE="mips.cmake"
        export_env_mips
        ;; 
    *)
        echo "Default x86 arch"
        export ARCH_MAKE="x86.cmake"
        export THIRD_LIB_PATH="x86_lib"
        ;;
    esac
}

choose_arch
init_env
