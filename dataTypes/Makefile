##MAKEFILE
#compiles each of the required objects into one shared object that root can read

#set up command line arguments for the compilers
CXXFLAGS 	= -g -I$(ROOTSYS)/include -std=c++11 -fPIC
CFLAGS 		= -g -I$(ROOTSYS)/include

#specify what files need to be compiled and linked
HEADERS 	= DataSet.h Group.h
SOURCES 	= DataSet.cpp Group.cpp dict.cpp
OBJECTS 	= DataSet.o Group.o dict.o

#specify what output is desired
all: 		  dataTypes.so

#build process for the shared object containing all data types
dataTypes.so:	$(OBJECTS)
				@echo "Linking $(PROGRAM) ..."
				@/bin/rm -f $(PROGRAM)
				@gcc -shared -std=c++11 $(OBJECTS) -o dataTypes.so
				@chmod 555 dataTypes.so
				@echo "done"

#delete partial object files to start fresh
clean:;        @rm -f $(OBJECTS)

#update the dataset object if the header has changed
DataSet.o: DataSet.h
Group.o: Group.h

#generate the root dictionary file based on the headers.
dict.cpp: $(HEADERS)
		@echo "Generating dictionary ..."
		@rootcint dict.cpp -c $(HEADERS)