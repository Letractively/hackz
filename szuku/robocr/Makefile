ALL=robocr
OPT= -O2
CC := g++


all: robocr.o texture.o utils.o EasyBMP.o
	$(CC) -o robocr robocr.o utils.o texture.o EasyBMP.o

%.o: %.cpp
	$(CC) $(OPT) -c $<


clean:
	rm -f *.o *~ $(ALL)
