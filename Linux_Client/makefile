SOURCE := ./src/*.cpp
#OBJ := ./src/*.o
TARGET := ./Linux_Client_App

LIBS := -Iopentok
LIBS_PATH := -L./libraries/libopentok_linux_llvm_x86_64/lib/libopentok.so
THIRD_PARTY_INCLUDE := -I~/oDocs/Linux_Client/./libs/common/src -I/usr/include/SDL2 -I./libraries/libopentok_linux_llvm_x86_64/include -I/serial -I/usr/include/opencv4 -I/usr/include/c++/12 -I/usr/include/x86_64-linux-gnu/c++/12 -I/usr/include/c++/12/backward -I/usr/lib/gcc/x86_64-linux-gnu/12/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include
EDITER_INCLUDE := -I./include

all: $(SOURCE)
	g++ $(THIRD_PARTY_INCLUDE) $(EDITER_INCLUDE) -o $(LIBS_PATH) $(LIBS) $(TARGET) 

#$(OBJ): $(SOURCE)
#	g++ -c $(THIRD_PARTY_INCLUDE) $(EDITER_INCLUDE) -o $(OBJ)