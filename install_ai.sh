#!/bin/bash

#install ai dependency 
apt-get update
apt -y install build-essential
apt -y install make
apt -y install unzip
apt -y install bzip2
apt -y install curl
apt -y install libopenblas-dev

#install server dependency
apt install cmake
apt install pip
pip install conan
conan profile detect

#install utils
apt install net-tools
apt -y install tmux

#download engine and network
wget https://github.com/LostRuins/koboldcpp/archive/refs/tags/v1.66.zip
wget https://huggingface.co/TheBloke/Llama-2-13B-chat-GGUF/resolve/main/llama-2-13b-chat.Q3_K_S.gguf

#install cuda
sudo ubuntu-drivers devices
sudo apt install gcc
sudo apt install nvidia-cuda-toolkit
nvcc --version

#install engine
unzip v1.66.zip
cd koboldcpp-1.66
make -j 3 LLAMA_OPENBLAS=1 
#make -j 7 LLAMA_CUBLAS=1
chmod +x koboldcpp.py

#run 
./koboldcpp.py ../llama-2-13b-chat.Q3_K_S.gguf 5001
#./koboldcpp.py ../llama-2-13b-chat.Q3_K_S.gguf 5001 --usecublas normal 0 --gpulayers 41

