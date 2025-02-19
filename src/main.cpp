#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <Eigen/Geometry>

class GCodeGenerator {
private:
    double width, depth, height, layerHeight;
    double posX, posY;
    double infillPercentage;
    std::string infillPattern;
    std::string filename;

    Eigen::Vector3f toolPosition;

public:
    GCodeGenerator(double w, double d, double h, double lh, double x, double y, double infill, std::string pattern, std::string file)
        : width(w), depth(d), height(h), layerHeight(lh), posX(x), posY(y), infillPercentage(infill), infillPattern(pattern), filename(file) {}

    void generate() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла!" << std::endl;
            return;
        }

        file << std::fixed << std::setprecision(2);
        file << "G21 ; Установить единицы в миллиметры\n";
        file << "G90 ; Абсолютное позиционирование\n";
        file << "G28 ; Домой все оси\n";
        file << "G1 X" << posX << " Y" << posY << " F1500 ; Переместить в начальную позицию\n";

        double z = 0.2;
        for (; z <= height; z += layerHeight) {
            file << "; Новый слой Z = " << z << " мм\n";

            bool leftToRight = true;
            for (double y = 0; y <= depth; y += 1.0) { 
                if (leftToRight) {
                    file << "G1 X" << (posX + width) << " Y" << (posY + y) << " Z" << z << " F1500\n";
                } else {
                    file << "G1 X" << posX << " Y" << (posY + y) << " Z" << z << " F1500\n";
                }
                leftToRight = !leftToRight;
            }

            file << generateInfill(z);
        }

        file << "G28 ; Вернуть сопло домой\n";
        file << "M104 S0 ; Отключить нагрев сопла\n";
        file << "M140 S0 ; Отключить нагрев стола\n";
        file << "M84 ; Отключить моторы\n";

        file.close();
        std::cout << "G-code сохранен в " << filename << std::endl;
    }

private:
    std::string generateInfill(double z) {
        std::ostringstream infillGCode;
        infillGCode << "; Заполнение (" << infillPattern << ") - " << infillPercentage << "%\n";
        // Здесь можно добавить реализацию различных алгоритмов заполнения
        return infillGCode.str();
    }
};

int main() {
    double width, depth, height, posX, posY, infillPercentage;
    std::string infillPattern;
    
    std::cout << "Введите ширину, глубину и высоту объекта (мм): ";
    std::cin >> width >> depth >> height;
    
    std::cout << "Введите начальные координаты X и Y (мм): ";
    std::cin >> posX >> posY;
    
    std::cout << "Введите процент заполнения (0-100): ";
    std::cin >> infillPercentage;
    
    std::cout << "Выберите алгоритм заполнения (grid, lines, concentric): ";
    std::cin >> infillPattern;

    GCodeGenerator generator(width, depth, height, 0.2, posX, posY, infillPercentage, infillPattern, "output.gcode");
    generator.generate();
    
    return 0;
}