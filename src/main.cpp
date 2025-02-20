#include <sliser.h>

int main(int argc, char* argv[]) {
    float cubeX = 40;
    float cubeY = 60;
    float cubeZ = 20;

    CubeSliser mySliser(cubeX, cubeY, cubeZ, 0.4, 10, 'g');
    mySliser.slice();
    mySliser.generateGcode("cube.gCode");

    return 0;
}