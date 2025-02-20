#include <sliser.h>
#include "gnuplot-iostream.h"

int main(int argc, char* argv[]) {
    float cubeX = 20;
    float cubeY = 20;
    float cubeZ = 10;

    /*
    CubeSliser mySliser(cubeX, cubeY, cubeZ, 0.4, 10, 'g');
    mySliser.slice();
    mySliser.generateGcode("cube.gCode");
    */

    Gnuplot gp;
    std::vector<std::pair<double, double>> xy_pts;
    for (double x = -2; x <= 2; x += 0.1) {
        double y = x * x;  // Квадрат функции
        xy_pts.push_back(std::make_pair(x, y));
    }

    // Отправляем команды в Gnuplot
    gp << "set title 'График функции y = x^2'\n";
    gp << "set xlabel 'X'\n";
    gp << "set ylabel 'Y'\n";
    gp << "plot '-' with lines title 'y = x^2'\n";
    gp.send1d(xy_pts);  // Отправляем данные для построения

    return 0;
}