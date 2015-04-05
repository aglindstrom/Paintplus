g++ -c main.cpp 2> mainError.txt

g++ -c framework.cpp 2> FWError.txt

g++ -c inputDevice.cpp 2> IDError.txt

g++ -c ogl.cpp 2> oglError.txt

g++ -c output.cpp 2> outputError.txt

g++ *.o c:\MinGW\lib\libuser32.a c:\MinGW\lib\libgdi32.a c:\MinGW\lib\libopengl32.a 2> compileError.txt

dir *.txt