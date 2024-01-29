SOURCE := ./src/*.cpp
#OBJ := ./src/*.o
TARGET := Linux_Client_App.out

LIBS := -lSDL2 -lSDL2-2.0 -lSDL2main -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_objdetect
LIBS_PATH := -L/usr/lib/x86_64-linux-gnu/libopencv_objdetect.so -L/usr/lib/x86_64-linux-gnu/libopencv_videoio.so -L/usr/lib/x86_64-linux-gnu/libopencv_highgui.so -L/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so -L/usr/lib/x86_64-linux-gnu/libSDL2.so -L/usr/lib/x86_64-linux-gnu/libSDL2main.so -L/usr/lib/x86_64-linux-gnu/libopencv_core.so
THIRD_PARTY_INCLUDE := -I~/oDocs/Linux_Client/./libs/common/src -I/usr/include/SDL2 -I./libraries/libopentok_linux_llvm_x86_64/include -I/serial -I/usr/include/opencv4 -I/usr/include/c++/12 -I/usr/include/x86_64-linux-gnu/c++/12 -I/usr/include/c++/12/backward -I/usr/lib/gcc/x86_64-linux-gnu/12/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include
EDITER_INCLUDE := -I./include

all: $(SOURCE)
	g++ $(SOURCE) $(EDITER_INCLUDE) $(THIRD_PARTY_INCLUDE) -o $(LIBS_PATH) $(LIBS) -o $(TARGET)

#$(OBJ): $(SOURCE) 
#	g++ -c $(THIRD_PARTY_INCLUDE) $(EDITER_INCLUDE) -o $(OBJ)  

#LIBS := -Iopentok -lSDL2 -lSDL2-2.0 -lSDL2main -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_objdetect
#LIBS_PATH := -L./libraries/libopentok_linux_llvm_x86_64/lib/libopentok.so -L/usr/lib/x86_64-linux-gnu/libopencv_objdetect.so -L/usr/lib/x86_64-linux-gnu/libopencv_videoio.so -L/usr/lib/x86_64-linux-gnu/libopencv_highgui.so -L/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so -L/usr/lib/x86_64-linux-gnu/libopentok.so -L/usr/lib/x86_64-linux-gnu/libSDL2.so -L/usr/lib/x86_64-linux-gnu/libSDL2main.so -L/usr/lib/x86_64-linux-gnu/libopencv_core.so