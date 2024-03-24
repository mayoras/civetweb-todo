# ref: https://github.com/civetweb/civetweb/blob/master/Dockerfile
# ---------------------------------------------------
# Build Stage
# ---------------------------------------------------

# Use Alpine Linux 3.18 as the base image for the build stage
FROM alpine:3.19 AS build

# Update package list and install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base zlib-dev

# Set the working directory inside the container
WORKDIR /app

# build civetweb static library
RUN git clone https://github.com/civetweb/civetweb.git
RUN cd civetweb; make WITH_ALL=1 lib

# copy app project
COPY . .

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
FROM alpine:3.19

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

# Expose port 8080 for the application
EXPOSE 8080

# Set the entry point for the container
ENTRYPOINT  [ "/app/todo" ]
