#!/bin/bash
cd
cd ..
cd /home/lak/new_kernel/linux-6.0.3
make -j$(nproc)
make modules
make modules_install
