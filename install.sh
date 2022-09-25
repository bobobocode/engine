#!/usr/bin/env sh

#BoBoBo

installdir=/usr/local/nginx
system=centos
while getopts 'p:s:' opts
do
    case $opts in
    p)
        installdir=$OPTARG;;
    s)
        system=$OPTARG;;
    esac
done

if [ "centos" = ${system} ]; then
    yum install -y gcc gcc-c++\
        make \
        openssl-devel \
        pcre-devel \
        zlib-devel \
        python3-devel \
        wget \
        libtool \
        bzip2 \
        && yum clean all
fi

nginx_version=1.19.3
curdir=`pwd`
scriptdir=$(cd "$(dirname "$0")"; pwd)

pip3 install -r ${scriptdir}/requirements.txt

cd ${scriptdir}
if [ ! -d nginx-${nginx_version} ]; then
    wget 'http://nginx.org/download/nginx-'${nginx_version}'.tar.gz' \
        && tar -zxvf nginx-${nginx_version}.tar.gz \
        && rm -f nginx-${nginx_version}.tar.gz
fi

cd nginx-${nginx_version}

export C_INCLUDE_PATH=/usr/include/python3.6m
./configure --with-debug \
      --with-cc-opt='-g -O0' \
      --prefix=${installdir} \
      --add-module=${scriptdir}/engine \
      --with-ld-opt="`python3.6-config --ldflags`"
make
make install

cd ${curdir}
