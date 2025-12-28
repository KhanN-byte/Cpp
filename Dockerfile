FROM ubuntu:latest
LABEL authors="haris"

# Install C++ Compiler
RUN apt-get update && apt-get install -y \
    g++ \
    && rm -rf /var/lib/apt/lists/*
# Set working directory
WORKDIR /app

# Copy source code into container
COPY . .

#Compile arraysum.cpp
RUN g++ arraysum.cpp -o arraysum

#RUN the program
CMD ["./arraysum"]

