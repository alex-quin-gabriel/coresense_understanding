#!/bin/sh
export DEBIAN_FRONTEND=noninteractive
mkdir templates
sudo apt-get update
sudo apt-get install -y python3-pip 
pip install --user --break-system-packages vcstool
vcs import --recursive --input dependencies.repos ../
