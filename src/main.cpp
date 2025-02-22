#include <sliser.h>

int main(int argc, char* argv[]) {
    float cubeX = 20;
    float cubeY = 20;
    float cubeZ = 10;

    CubeSliser mySliser(cubeX, cubeY, cubeZ, 0.4, 10, 'g');
    mySliser.slice();
    mySliser.generateGcode("cube.gCode");

    return 0;
}