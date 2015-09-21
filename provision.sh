#!/bin/bash

sudo add-apt-repository ppa:xorg-edgers/ppa
sudo apt-get update

sudo apt-get -y install make
sudo apt-get -y install freeglut3 freeglut3-dev
sudo apt-get -y install g++
sudo apt-get -y install nvidia-331 nvidia-331-updates
