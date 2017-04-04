
OBJECTBIN:=main.o lock.o thread.o ./tools/nreentry.o #./algorithms/sort.o msgqueue.o

OBJECTSO:=lock.o thread.o ./tools/nreentry.o #./algorithms/sort.o  msgqueue.o

main:$(OBJECTBIN)
	g++ -g  -o main $(OBJECTBIN) -lpthread -lrt
#common.so:$(OBJECTSO)
#	g++ -shared -o common.so $(OBJECTSO)


main.o : main.cpp thread.h ./tools/nreentry.h lock.h ./algorithms/sort.h
	g++ -c -g main.cpp

thread.o : thread.cpp thread.h
	g++ -c -g thread.cpp

lock.o : lock.cpp lock.h
	g++ -c -g lock.cpp 

#msgqueue.o : msgqueue.h 
#	g++ -c msgqueue.h

#sort.o : ./algorithms/sort.h
#	g++ -c ./algorithms/sort.h

nreentry.o : ./tools/nreentry.cpp ./tools/nreentry.h
	g++ -c -g ./tools/nreentry.cpp

#common.so:$(OBJECTTSO)
#	g++ -shared -o common.so $(OBJECTSO)


clean:
	rm -f *.o
	rm -f ./tools/*.o
	rm -f ./algorithms/*.o
	
