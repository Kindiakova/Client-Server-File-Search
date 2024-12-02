VPATH = headers

ifeq ($(OS),Windows_NT) 
SEARCH = WinFileSearch.cpp 
CONN = WinConnect.cpp
CXX_LIBS = -lpthread -lws2_32
else
SEARCH = LinuxFileSearch.cpp 
CONN = LinuxConnect.cpp	
CXX_LIBS = -lpthread 
endif

client:	
	g++ -o client client.cpp -I. $(SEARCH) $(CONN)  $(CXX_LIBS)
	
server: 
	 g++ -o server server.cpp -I. $(CONN)  $(CXX_LIBS)

all: client server

