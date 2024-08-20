# select a base image from Docker's archive
FROM python:3.8.5-alpine3.12 as base

FROM base as builder

# install dependencies for ncclient install
RUN apk add --no-cache \
  libxml2-dev \
  libxslt-dev \
  gcc \
  musl-dev \
  alpine-sdk \
  libffi-dev \
  openssl-dev
RUN pip install --prefix=/install --no-cache-dir ncclient

# second stage
FROM base as deploy

# install dynamically linked dependencies
RUN apk add --no-cache libxml2-dev libxslt-dev

# copy what we need from stage 1
COPY --from=builder /install /usr/local

# set work directory
WORKDIR /usr/src/monitor

# copy files from this repo to the image's FS
COPY . /usr/src/monitor

# clean up unnecessary files
RUN rm -rf \
  /sbin/apk \
  /etc/apk \
  /lib/apk \
  /usr/share/apk \
  /var/lib/apk \
  $HOME/.cache/pip \
  /usr/local/lib/python3.8/site-packages/pip \
  /usr/local/lib/python3.8/ensurepip \
  /usr/lib/cmake
  

# command to be executed as container runs
CMD [ "python", "monitor.py" ]
