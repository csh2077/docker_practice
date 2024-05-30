#!/bin/bash

PLATFORM=${1:-linux_arm}

DOCKER_FILE="buildScript/Dockerfile"
DOCKER_FILE_HASH="buildScript/dockerfile_hash.txt"
DOCKER_FILE_HASH_PREV="buildScript/dockerfile_hash_prev.txt"
IMAGE_NAME="docker_env"
CONTAINER_NAME="running_docker_env_container"

if ! command -v docker &> /dev/null; then
    echo "Docker could not be found, please install Docker first."
    exit 1
fi

sha256sum "$DOCKER_FILE" > "$DOCKER_FILE_HASH"

if [[ -f "$DOCKER_FILE_HASH_PREV" ]] && cmp --silent "$DOCKER_FILE_HASH" "$DOCKER_FILE_HASH_PREV"; then
    echo "Docker configuration has not changed."
else
    echo "Docker configuration has changed or has not been deployed. Rebuilding..."
    docker build -t "$IMAGE_NAME" ./buildScript || {
        echo "ERROR: Docker build failed for image $IMAGE_NAME" >&2
        exit 1
    }
fi

cp "$DOCKER_FILE_HASH" "$DOCKER_FILE_HASH_PREV"

if [ "$(docker ps -q -f name=^${CONTAINER_NAME}$)" ]; then
    echo "already running docker env..."
    docker exec -it $CONTAINER_NAME /bin/bash -c "cd /app && ./buildScript/compile_inside_docker_env.sh $PLATFORM" || {
        echo "ERROR: Failed to execute compile script in running parts." >&2
        exit 1
    }
else
    echo "docker env will run..."
    docker rm $CONTAINER_NAME
    docker run -it --name "$CONTAINER_NAME" -v "$(pwd)":/app "$IMAGE_NAME" /bin/bash -c "cd /app && ./buildScript/compile_inside_docker_env.sh $PLATFORM" || {
        echo "ERROR: Failed to run compile script in new container." >&2
        exit 1
    }
fi
