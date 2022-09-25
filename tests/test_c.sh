#!/usr/bin/env sh

#BoBoBo#

echo "Start cleaning..."
docker-compose rm -sf
echo "Cleaned all."

docker-compose up -d test-c
docker logs -f engine.test.c

echo "Start cleaning..."
docker-compose rm -sf
echo "Cleaned all."
