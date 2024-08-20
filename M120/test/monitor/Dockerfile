# select a base image from Docker's archive
FROM python:3.8.5-slim-buster as base

# name stage 1 as builder
FROM base as builder

# install dependencies
RUN pip install --prefix=/install --no-cache-dir ncclient

FROM base

# copy what we need from stage 1
COPY --from=builder /install /usr/local

# set work directory
WORKDIR /usr/src/monitor

# copy files from this repo to the image's FS
COPY . /usr/src/monitor

# command to be executed as container runs
CMD [ "python", "monitor.py" ]
