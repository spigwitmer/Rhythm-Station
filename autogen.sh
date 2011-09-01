#!/bin/bash
autoreconf --install
git submodule init
git submodule update
(cd extern/glfw3 && cmake .)
(cd extern/gl3w && python gl3w_gen.py)
