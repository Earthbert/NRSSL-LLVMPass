FROM debian:12.10

RUN apt update && \
    apt install -y \
        build-essential \
        cmake \
        llvm-19 \
        libclang-19-dev \
        clang-19 \
        openjdk-17-jdk

COPY ./src /src
COPY ./update_alternatives.sh /src

WORKDIR /src

ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64

RUN ./update_alternatives.sh
RUN mkdir build_all
