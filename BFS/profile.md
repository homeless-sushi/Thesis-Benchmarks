# BFS Benchmark

## Profiling

To profile the BFS benchmark, `mosquitto` (a mqtt-broker) and `agora` (margot's profiling component) must be running.

### Run mosquitto

Run `mosquitto` with: 
 
```console
$ mosquitto
```

### Run agora

The easiest way to run agora is using `docker`:  

```console
$ sudo docker run \
  -i -t \
  --network="host" \
  --mount type=bind,source=/home/vivian/Documents/Thesis/apps/Thesis-Benchmarks/BFS,target=/usr/src/work/run \
  margotpolimi/agora:1.0 --workspace-directory /usr/src/work/run --plugin-directory /usr/src/margot/core/agora/plugins --models-directory /usr/src/work/run/models/ --qos 2 --storage-address /usr/src/work/run/ --min-log-level warning
```

### Builds and Run BFS Benchmark  

To build and run the BFS benchmark, simply use the scripts provided:  

```console
$ ./build.sh 
$ ./run.sh
```