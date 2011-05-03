#!/bin/bash
autoreconf --install
git submodule init
git submodule update
(cd extern/glfw3 && cmake . && make -j5)

