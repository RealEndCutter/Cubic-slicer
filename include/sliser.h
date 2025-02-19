#ifndef SLISER_H
#define SLISER_H

#include <Eigen/Geometry>
#include <vector>
#include <cmath>

class CubeSliser {
public:
    CubeSliser(float dimX, float dimY, float dimZ, float layerHeight, int infillDensPntg, char infillPattern);

    ~CubeSliser();

    void generateGcode(std::string filename); // Генератор GCode в txt
    void slice(); // Слайсер. Генерирует траекторию сопла

private:
    // Const Parameters:
    const float wallThinkness = 1.6f;
    const float topThinkness = 2.0f;;
    const float bottomThinkness = 2.2f;
    const float nozzleDiameter = 0.4f;

    // Slice Parameters:
    float layerHeight;
    int infillDensity;
    char infillPattern;
    Eigen::Vector2f startPose;
    std::vector<Eigen::Vector3f> nozzleTrj;

    // Object Parameters (Cube):
    float cubeX;
    float cubeY;
    float cubeZ;

    // Functions
    void generateFullInfill();
    void generateDenseInfill();
    void makeWall();
};

#endif // SLISER_H
