FROM ubuntu:18.04

# Install required packages, including openocd
RUN apt-get update && apt-get install -y --no-install-recommends \
        bsdmainutils \
        build-essential \
        cmake \
        file \
        gcc-arm-none-eabi binutils-arm-none-eabi \
        gdb \
        git \
        libpython2.7 \
        less \
        openocd \
        python \
        telnet \
        tio \
        usbutils \
        vim \
        wget \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p packages && cd packages && \
    wget -q http://mirrors.kernel.org/ubuntu/pool/universe/g/gdb-arm-none-eabi/gdb-arm-none-eabi_7.10-1ubuntu3+9_amd64.deb && \
    wget -q http://mirrors.kernel.org/ubuntu/pool/main/r/readline6/libreadline6_6.3-8ubuntu2_amd64.deb && \
    dpkg -i libreadline6_6.3-8ubuntu2_amd64.deb && \
    dpkg -i gdb-arm-none-eabi_7.10-1ubuntu3+9_amd64.deb && \
    cd ..

# Enable device rules for OpenOCD
RUN mkdir -p /etc/udev/rules.d/ \
    && cp /lib/udev/rules.d/60-openocd.rules /etc/udev/rules.d/60-openocd.rules

# Set workspace directory
ARG WORKDIR=/workspace
WORKDIR ${WORKDIR}

# Copy OpenOCD configuration files
COPY ocdconfig ${WORKDIR}/ocdconfig
COPY openocd.cfg ${WORKDIR}
COPY tioze.sh ${WORKDIR}

# Update OpenOCD probe config file to match actual USB identifiers
RUN sed -is "s/0x6014/0x6010/" /usr/share/openocd/scripts/interface/ftdi/digilent_jtag_smt2.cfg

COPY zybo_gdb.script ${WORKDIR}
#ENTRYPOINT ["openocd"]
