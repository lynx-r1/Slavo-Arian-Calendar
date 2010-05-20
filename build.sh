#!/bin/bash

. ./install_prefix.sh
qmake
make && make install
