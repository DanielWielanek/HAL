# Użyj Ubuntu jako bazowego obrazu
FROM ubuntu:24.10

LABEL maintainer="twoj.email@example.com"

# Ustaw zmienne środowiskowe, aby uniknąć interakcji podczas instalacji
ENV DEBIAN_FRONTEND=noninteractive

# Instalacja wymaganych pakietów systemowych i ROOT-a CERN
RUN apt-get update && apt-get install -y \
    dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev python3 libssl-dev \
    gcc pkg-config libwayland-dev libx11-dev libx11-xcb-dev libxkbcommon-x11-dev libgles2-mesa-dev \
    libegl1-mesa-dev libffi-dev libxcursor-dev libvulkan-dev xvfb xdotool libgsl-dev \
    libvdt-dev libgif-dev libtbb-dev gfortran vim\
    wget tar && \
    apt-get clean

# Pobranie i rozpakowanie ROOT-a CERN
RUN wget --quiet https://root.cern/download/root_v6.34.02.Linux-ubuntu24.10-x86_64-gcc14.2.tar.gz && \
    tar zxf root_v6.34.02.Linux-ubuntu24.10-x86_64-gcc14.2.tar.gz --directory /opt

# Ustawienie zmiennych środowiskowych dla ROOT-a
ENV ROOTSYS=/opt/root
ENV PATH=$ROOTSYS/bin:$PATH
ENV LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH

# Sprawdzenie instalacji ROOT-a
RUN root-config --features

# Ustawienie katalogu roboczego
WORKDIR /app

# Skopiowanie kodu źródłowego do kontenera
COPY . /app/hal_src
WORKDIR /app/hal_src

# Kompilacja kodu
RUN bash  -c "source /opt/root/bin/thisroot.sh"
RUN bash  -c "export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH"
RUN bash  -c "mkdir build"
RUN bash  -c "cd build "
WORKDIR /app/hal_src/build
RUN bash  -c "cmake /app/hal_src -DCMAKE_INSTALL_PREFIX=/app/hal_inst -DEXAMPLES=ON"
RUN bash  -c "make"
RUN bash  -c "make install"


# Ustawienie domyślnego polecenia
CMD ["bash"]
ENTRYPOINT ["/bin/bash", "-c", "source /opt/root/bin/thisroot.sh && source /app/hal_inst/bin/hal_config.sh && exec bash"]


#CMD ["/bin/bash", "-c", "source /opt/root/bin/thisroot.shh"]
#CMD ["/bin/bash", "-c", "source /app/hal_inst/bin/hal_config.sh"]



