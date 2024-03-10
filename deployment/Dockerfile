FROM ubuntu:24.04

RUN apt-get update && apt-get install -y libssl-dev zlib1g-dev ca-certificates libsodium-dev libopus-dev libfreetype6-dev libpng-dev \
     cmake \
     pkg-config \
     g++ \
     gcc \
     git \
     fontconfig \
     wget \
     curl \
     protobuf-compiler=3.21.12-8ubuntu5 \
     make

RUN git config --global --add safe.directory /Discland
RUN curl -fL https://install-cli.jfrog.io | sh
RUN jf c add local --url=http://192.168.10.3:8081 --user=admin --password=Jfrog_1234 --interactive=false

RUN jf rt dl mongo-binaries/mongo-c-driver-r1-26/* /dependencies/ && \
     jf rt dl catch2-binaries/catch2-3.5.3/ /dependencies/ && \
     jf rt dl dpp-binaries/dpp-10.0.29/ /dependencies/ && \
     jf rt dl libgd-binaries/libgd-master/ /dependencies/ && \
     jf rt dl mongo-binaries/mongo-cxx-driver-1.10/ /dependencies/

COPY install_dependencies.sh /root/

# WORKDIR /app/DPP/build

# RUN cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_STANDARD=20 -DDPP_BUILD_TEST=OFF -DBUILD_VOICE_SUPPORT=OFF -DCMAKE_INSTALL_PREFIX=/artifact .. && \
#      make && \
#      make install

# WORKDIR /artifact
# RUN jf c add local --url=http://172.17.0.3:8081 --user=admin --password=Artifactory_1234 --interactive=false
# RUN jf rt upload "include/*" dpp

# docker build -t docker-discland -f ./deployment/Dockerfile .
# docker run --name docker-discland -v .:/Discland -w /Discland -it docker-discland:latest
# jf c add local --url=http://172.17.0.3:8081 --user=admin --password=Artifactory_1234 --interactive=false
# jf rt upload "include/*" dpp
