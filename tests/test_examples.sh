#!/usr/bin/env sh

#BoBoBo#

cur_dir=`pwd`
scrip_dir=$(cd "$(dirname "$0")"; pwd)
root_dir=${scrip_dir}/..
examples_dir=${root_dir}/examples
cd ${examples_dir}
echo "Start cleaning..."
docker-compose rm -sf
echo "Cleaned all."

docker-compose up -d engine-examples

while :
do
    result=`curl http://localhost:8080`
    if [[ $result =~ "Welcome" ]]; then
          echo "Service is open."
          break
    else
          echo "Service is not open now."
          sleep 3
    fi
done

pytest ${scrip_dir}/test_examples.py

echo "Start cleaning..."
docker-compose down
echo "Cleaned all."
cd ${cur_dir}
