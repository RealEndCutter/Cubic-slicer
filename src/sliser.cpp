#include <sliser.h>

CubeSliser::CubeSliser(float dimX, float dimY, float dimZ, float layerHeight, int infillDensPntg, char infillPattern)
            : cubeX(dimX), cubeY(dimY), cubeZ(dimZ), layerHeight(layerHeight), infillDensity(infillDensPntg), infillPattern(infillPattern) {}

CubeSliser::~CubeSliser() {}

void CubeSliser::makeWall() {
    int cylceCount = static_cast<int>(std::ceil(wallThinkness/nozzleDiameter));
    for(int i = 0; i < cylceCount; i++) {
        nozzleTrj.push_back(Eigen::Vector3f(cubeX - nozzleDiameter/2 - i*nozzleDiameter, nozzleDiameter/2 + i*nozzleDiameter, 0.0f));
        nozzleTrj.push_back(Eigen::Vector3f(cubeX - nozzleDiameter/2 - i*nozzleDiameter, cubeY - nozzleDiameter/2 - i*nozzleDiameter, 0.0f));
        nozzleTrj.push_back(Eigen::Vector3f(nozzleDiameter/2 + i*nozzleDiameter, cubeY - nozzleDiameter/2 - i*nozzleDiameter, 0.0f));
        nozzleTrj.push_back(Eigen::Vector3f(nozzleDiameter/2 + i*nozzleDiameter, nozzleDiameter/2 + (i+1)*nozzleDiameter, 0.0f));
    }
}

void CubeSliser::generateFullInfill() {
    if(CubeSliser::nozzleTrj.empty()) {
        float startX = startPose.x() - cubeX/2;
        float startY = startPose.y() - cubeY/2;
        float startZ = 0.0f;
        nozzleTrj.push_back(Eigen::Vector3f(startX, startY, startZ));
        makeWall();

    }


}