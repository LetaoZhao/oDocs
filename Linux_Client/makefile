THIRD_PARTY_LIB := -I~/oDocs/Linux_Client/./libs/common/src -I/usr/include/SDL2 -I./libraries/libopentok_linux_llvm_x86_64/include -I./libraries/libopentok_linux_llvm_x86_64/docs -I./libraries/libopentok_linux_llvm_x86_64/lib -I/serial -I/usr/include/opencv4 -I/usr/include/c++/12 -I/usr/include/x86_64-linux-gnu/c++/12 -I/usr/include/c++/12/backward -I/usr/lib/gcc/x86_64-linux-gnu/12/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include
EDITER_LIB := -I./include

all: ~/oDocs/Linux_Client/src/*.cpp
	g++ $(EDITER_LIB) $(THIRD_PARTY_LIB) ~/oDocs/Linux_Client/src/*.cpp -o ~/oDocs/Linux_Client/Linux_Client_App