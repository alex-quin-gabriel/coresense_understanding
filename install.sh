#!/bin/sh
export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install -y python3-pip

pip install pydantic pyoxigraph reasonable vcstool
vcs import --input dependencies.repos ../
