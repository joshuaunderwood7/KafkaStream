# KafkaStream
Basic framework for building Kafka Streaming Microservices

## This is the most basic, boilerplate framework
The purpose of this project is to give you the absolutely minimal framework for
making a function process a string from a Kafka Source and echo a string to a
Kafka Sink.

In order to accomplish this, you only have to implement a single method of the
MyMap class that is defined in the src/program.cpp file and modify the
config.cfg file to connect to your Kafka cluster.

Type `make` then `./program config.cfg` and you're program will be running.
<Ctl-C> to kill the program will signal MyMap to stop running and disconnect
from the Kafka after attempting to stop the consumer and producer..

