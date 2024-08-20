## Docker basic usage
### Setup
We recommend to install Docker with binaries, if possible. Check https://docs.docker.com/engine/install/binaries/ for instructions.

Check the webpage's side bars for installation using command line in different distros. 

If you use VS Code, you'll probably find the Docker extension very useful. 

### Build an image
To build an image, the repo needs a `Dockerfile` to specify its composition. Typically, multi-stage builds is utilized, see below for more details. 

With a dockerfile, run the following to build an image with a name and a tag. Name is usually the project name and tag is used to identify the usage of this image. For instance, there can be a latest image, a stable image, a debug image, etc. 
```bash
$ docker build -t <name:tag> -f <path-to-Dockerfile>
```
The files that compose the built images, as well as containers, are located in `/var/lib/docker/`.

### Run an image
```bash
$ docker run <name:tag>
```

### Save an image
```bash
$ docker save <my_image:tag> > <my_image.tar>
```

### Load a saved tar into docker as an image
```bash
$ docker load < <saved_image.tar>
```

### See all images
```bash
$ docker image list
```

### Delete an image
```bash
$ docker image rm <name:tag>
```

### Clean unused docker images
```bash
$ docker image prune
```

Please see docker documentation for more commands, detailed description and options

## Multi-stage builds
https://docs.docker.com/develop/develop-images/multistage-build/
### What it does
- Keep image size down, 
- Optimize Dockerfile while keeping it easy to read and maintain.

### How it works
- Use multiple `FROM` in `dockerfile`. Each `FROM` begins a new stage of build. You can selectively copy artifacts from one stage to another, leaving behind the things you don't want. 

Notice that the `Dockerfile` in this repo can be a second example. 

### Simple example (from the doc)
```dockerfile
# first stage, to build the app
# select base image, name it as "builder"
FROM golang:1.7.3 AS builder

# set workdir
WORKDIR /go/src/github.com/alexellis/href-counter/

# get dependency
RUN go get -d -v golang.org/x/net/html  

# copy content to workdir
COPY app.go .

# execute command, which is to build the app in this case
RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .


# second stage, for deployment
# select base image
FROM alpine:latest

# get dependency
RUN apk --no-cache add ca-certificates

# set workdir
WORKDIR /root/

# copy files that are needed from builder stage to this stage's workdir
COPY --from=builder /go/src/github.com/alexellis/href-counter/app .

# command to be executed, which is to run the app in this case
CMD ["./app"]
```

### Stop at a specific build stage
```bash
$ docker build --target <stage_name> -t <name:tag> .
```


## Persistent files with volume
https://docs.docker.com/storage/

We recommend to persist files such as `settings.py` and the log file with volume.

https://docs.docker.com/storage/volumes/

Volumes are created and managed by Docker, and are isolated from the core functionality of the host machine. They are stored in `/var/lib/docker/volumes`.

### Create a volume
`docker volume create <name>`

### See all volumes
`docker volume ls`

### Remove a volume
`docker volume rm <name>`

### Start a container with a volume
```
$ docker run -d \
  --name monitor \
  --mount source=myvol,target=/app \
  monitor:latest
```
This mounts the volume to `/app` in container's filesystem. In the script, we can let the log filehandler to save log file to this directory, and the log file will persist.


## Links to more
To better understand how an image is stored locally:

https://www.freecodecamp.org/news/where-are-docker-images-stored-docker-container-paths-explained/

Docker config:

https://medium.com/@sujaypillai/docker-daemon-configuration-file-f577000da655

62      .
56      ./usr
	39      ./usr/local
		38      ./usr/local/lib
			35      ./usr/local/lib/python3.8
				16      ./usr/local/lib/python3.8/site-packages
					5       ./usr/local/lib/python3.8/site-packages/lxml
					3       /usr/local/lib/python3.8/site-packages/setuptools
					3       /usr/local/lib/python3.8/site-packages/cryptography/hazmat
					3       /usr/local/lib/python3.8/site-packages/cryptography
					2       /usr/local/lib/python3.8/site-packages/paramiko
				6       ./usr/local/lib/python3.8/lib-dynload
				2       /usr/local/lib/python3.8/idlelib
				2       /usr/local/lib/python3.8/encodings
				1       /usr/local/lib/python3.8/xmlrpc
				1       /usr/local/lib/python3.8/xml
				1       /usr/local/lib/python3.8/wsgiref
				1       /usr/local/lib/python3.8/venv
				1       /usr/local/lib/python3.8/urllib
	15      ./usr/lib

4       ./lib
2       ./etc                                                                                                           1       ./var                                                                                                           1       ./tmp                                                                                                           1       ./srv                                                                                                           1       ./sbin                                                                                                          1       ./run                                                                                                           1       ./root
