# Car-Production-Simulation
A multithread program in C that stimulates a car production line with limited storage space and worksd and all parts being made when needed. It solves the producer consume problem.

In the producer-consumer problem, there is a Producer that creates a resource and a COnsumer that consumes the resource. The producers and consumers share the same memory buffer that is of fixed-size.

The job of the Producer is to generate the resource, put it into the buffer, and again start generating data. While the job of the Consumer is to consume the data from the buffer.

However, problem arises when the consumer attempts to consume a resource when the buffer is empty or when the producer attempts to create new resources even though the buffer is full. In this case the program will be stuck in a loop waiting for resource or waiting for space. 

This program solves this problem through the use of semaphores.
