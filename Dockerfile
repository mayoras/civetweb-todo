# ref: https://github.com/civetweb/civetweb/blob/master/Dockerfile
# ---------------------------------------------------
# Build Stage
# ---------------------------------------------------

# Use Alpine Linux 3.18 as the base image for the build stage
FROM alpine:3.18 AS build

# Update package list and install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base zlib-dev \
    git make clang curl sudo

# Set the working directory inside the container
WORKDIR /app

# build civetweb static library
RUN git clone https://github.com/civetweb/civetweb.git
RUN cd civetweb; make WITH_CPP=1 lib

# copy app project
COPY . .

# initialize database
# RUN echo "{}" > data/db.json

# copy the static library to project libs
RUN cp civetweb/libcivetweb.a lib/

# also copy the header files from build
RUN cp civetweb/include/* inc/

# compile source
RUN make

# ---------------------------------------------------
# Image Stage
# ---------------------------------------------------

# Use Alpine Linux 3.18 as the base image for the final stage
FROM alpine:3.18

# Update package list and install runtime dependencies
RUN apk update && \
    apk add --no-cache \
    libstdc++ zlib

# Create a non-root user and group for running Civetweb
RUN addgroup -S civetweb && adduser -S civetweb -G civetweb

# Switch to the non-root user
USER civetweb

# Copy the built application from the build stage into this stage
COPY --chown=civetweb:civetweb --from=build /app/ /app/

# Define Civetweb API env variables
ENV CIVETWEB_DATABASE_PATH /app/data/db.json
ENV CIVETWEB_DOCUMENT_ROOT /app/static
ENV CIVETWEB_PORT 8080

# Expose port 8080 for the application
EXPOSE 8080

# Set the entry point for the container
ENTRYPOINT  [ "/app/todo" ]
