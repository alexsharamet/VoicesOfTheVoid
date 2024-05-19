#!/bin/bash

conan install . -of=conan-deps-release --profile=release --build=missing 
conan install . -of=conan-deps-debug --profile=debug --build=missing
