FROM ubuntu:18.04

# Install required packages, including openocd
RUN apt-get update && apt-get install -y --no-install-recommends \
        binutils-arm-none-eabi \
        bsdmainutils \
        build-essential \
        cmake \
        file \
        gcc-arm-none-eabi \
        gdb \
        git \
        less \
        libpython2.7 \
        openocd \
        python \
        telnet \
        tio \
        u-boot-tools \
        usbutils \
        vim \
        wget \
    && rm -rf /var/lib/apt/lists/*

# Install ARM GDB, left out of Ubuntu 18 packages
RUN mkdir -p packages && cd packages && \
    wget -q http://mirrors.kernel.org/ubuntu/pool/universe/g/gdb-arm-none-eabi/gdb-arm-none-eabi_7.10-1ubuntu3+9_amd64.deb && \
    wget -q http://mirrors.kernel.org/ubuntu/pool/main/r/readline6/libreadline6_6.3-8ubuntu2_amd64.deb && \
    dpkg -i libreadline6_6.3-8ubuntu2_amd64.deb && \
    dpkg -i gdb-arm-none-eabi_7.10-1ubuntu3+9_amd64.deb && \
    cd ..

# Enable device rules for OpenOCD
RUN mkdir -p /etc/udev/rules.d/ \
    && cp /lib/udev/rules.d/60-openocd.rules /etc/udev/rules.d/60-openocd.rules

# Update OpenOCD probe config file to match actual USB identifiers of Zybo board
RUN sed -i "s/0x6014/0x6010/" /usr/share/openocd/scripts/interface/ftdi/digilent_jtag_smt2.cfg

# Set workspace directory
ARG WORKDIR=/workspace
WORKDIR ${WORKDIR}

# Copy source of the application
COPY app ${WORKDIR}/app

# Build the C hello world application to be debugged
# 1. cmake: configure the directory to build
# 2. make: compile source in objects and link objects in ELF application
# 3. objcopy: convert ELF application to binary application
# 4. mkimage: add a header to binary application to boot it with U-Boot
RUN mkdir app/build && \
    cd app/build/ && \
    cmake -DTOOLCHAIN_DIR=/usr -DCMAKE_BUILD_TYPE=Debug .. && \
    make VERBOSE=1 -j1 && \
    arm-none-eabi-objcopy \
        --enable-deterministic-archives \
        --output-target binary \
        embeddedhello \
        embeddedhello.bin && \
    mkimage -A arm \
            -O linux \
            -C none \
            -T kernel \
            -a 0x00100000 \
            -e `arm-none-eabi-readelf --headers embeddedhello | grep Entry | awk '{print $NF}'` \
            -d embeddedhello.bin \
            embeddedhello.img

# Copy OpenOCD configuration files
COPY ocdconfig ${WORKDIR}/ocdconfig

# Copy handy script files
COPY scripts ${WORKDIR}/scripts
COPY gdb ${WORKDIR}/gdb
