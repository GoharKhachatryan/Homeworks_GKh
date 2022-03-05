all:			shm_producer shm_consumer


shm_producer:		shm_producer.o
			g++ shm_producer.o -o shm_producer -pthread -lrt

shm_consumer:		shm_consumer.o
			g++ shm_consumer.o -o shm_consumer -pthread -lrt 

shm_producer.o:		shm_producer.cpp
			g++ -c shm_producer.cpp

shm_consumer.o:		shm_consumer.cpp
			g++ -c shm_consumer.cpp

clean:
			rm *.o shm_producer shm_consumer
