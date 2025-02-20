#include <sliser.h>

CubeSliser::CubeSliser(float dimX, float dimY, float dimZ, float layerHeight, int infillDensPntg, char infillPattern)
            : cubeX(dimX), cubeY(dimY), cubeZ(dimZ), layerHeight(layerHeight), infillDensity(infillDensPntg), infillPattern(infillPattern) {}

CubeSliser::~CubeSliser() {}

void CubeSliser::generateGcode(std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error when open the file with name:" << filename << std::endl;
        return;
    }
    file << gCode;
    file << "G28\n";
    file << "M84\n";

    file.close();
    std::cout << "G-code is saved in " << filename << std::endl;
}

void CubeSliser::slice() {
    int layersCount = (int)cubeZ/layerHeight;
    int bottomLayersCount = (int)cubeZ/bottomThinkness;
    int topLayersCount = (int)cubeZ/topThinkness;
    std::stringstream ss;
    ss << "G90 ;use absolute coordinates\n" <<
          "M82 ;absolute extrusion mode\n" <<
          "G28 ;home\n";
    bool inFillDir = false;

    std::vector<Eigen::Vector2f> wallTrajectory;
    std::vector<Eigen::Vector2f> inFillTrajectory;
    std::cout << "Slice Start \n";
    for(int i = 1; i <= layersCount; i++) {
        std::cout << "slice layer #" << i <<std::endl;

        float zCurrent = layerHeight*i;
        ss << ";LAYER_" << i << std::endl <<
              "G0 " << "X" << 10 << " Y" << 10 << " Z" << zCurrent << std::endl;
        gCode += this->makeWall(wallThinkness, nozzleDiameter, Eigen::Vector2f(cubeX, cubeY), wallTrajectory);
        std::cout << "wall - OK \n";
        if(i <= bottomLayersCount) {
            gCode += this->infillZigZag(wallThinkness, nozzleDiameter, Eigen::Vector2f(cubeX, cubeY), inFillTrajectory, inFillDir);
            inFillDir = !inFillDir;
        }
        else if(i >= layersCount - topLayersCount) {
            gCode += this->infillZigZag(wallThinkness, nozzleDiameter, Eigen::Vector2f(cubeX, cubeY), inFillTrajectory, inFillDir);
            inFillDir = !inFillDir;
        }
        std::cout << "infill - OK \n";
        std::cout << "layer #" << i << " is finished\n\n";
    }
}

std::string CubeSliser::makeWall(float wallThinkness, float nozzleD, Eigen::Vector2f cubeDims, std::vector<Eigen::Vector2f>& wallTrj) {
    std::stringstream ss;

    // Make inside wall
    ss << ";WALL_INNER\n";
    int N = (wallThinkness-nozzleD)/nozzleD;
    for(int i = 1; i < N; i++) {
        Eigen::Vector2f inner = cubeDims.array() - 2*wallThinkness + i*nozzleD;
        Eigen::Vector2f countur[] = {Eigen::Vector2f(0.0f, 0.0f), 
                                     Eigen::Vector2f(inner.x(), 0.0f),
                                     Eigen::Vector2f(inner.x(), inner.y()),
                                     Eigen::Vector2f(0.0f, inner.y()),
                                     Eigen::Vector2f(0.0f, nozzleD)};
        for(Eigen::Vector2f &num : countur) {
            num.array() -= i*nozzleD + nozzleD*N + nozzleD/2;
        }
        ss << "G0 " << "F6000 " << "X" << countur[0].x() << " Y" << countur[0].y() << std::endl;
        for(Eigen::Vector2f cord : countur) {
            ss << "G1 " << "X" << cord.x() << " Y" << cord.y() << std::endl;
            wallTrj.push_back(cord);
        }
    }
    // Make outside wall
    ss << ";WALL_OUTER\n";
    Eigen::Vector2f outer = cubeDims.array() - nozzleD;
    Eigen::Vector2f countur[] = {Eigen::Vector2f(0.0f, 0.0f), 
                                 Eigen::Vector2f(outer.x(), 0.0f),
                                 Eigen::Vector2f(outer.x(), outer.y()),
                                 Eigen::Vector2f(0.0f, outer.y()),
                                 Eigen::Vector2f(0.0f, nozzleD)};
    for(Eigen::Vector2f &num : countur) {
        num.array() += nozzleD/2;
    }
    ss << "G0 " << "F6000 " << "X" << countur[0].x() << " Y" << countur[0].y() << std::endl;
    for(Eigen::Vector2f cord : countur) {
        ss << "G1 " << "X" << cord.x() << " Y" << cord.y() << std::endl;
        wallTrj.push_back(cord);
    }
    std::string output = ss.str();
    return output;
}

std::string CubeSliser::infillZigZag(float wallThinkness, float nozzleD, Eigen::Vector2f cubeDims, std::vector<Eigen::Vector2f>& infillTrj, bool dir) {
    std::stringstream ss; 
    // Make infill contour
    ss << ";INFILL_WALL\n";  
    Eigen::Vector2f infill_wall_coord = cubeDims.array() - nozzleD;
    Eigen::Vector2f countur[] = {Eigen::Vector2f(0.0f, 0.0f), 
                                 Eigen::Vector2f(infill_wall_coord.x(), 0.0f),
                                 Eigen::Vector2f(infill_wall_coord.x(), infill_wall_coord.y()),
                                 Eigen::Vector2f(0.0f, infill_wall_coord.y()),
                                 Eigen::Vector2f(0.0f, nozzleD)};
    for(Eigen::Vector2f &num : countur) {
        num.array() += nozzleD/2 + wallThinkness;
    }
    ss << "G0 " << "F6000 " << "X" << countur[0].x() << " Y" << countur[0].y() << std::endl;
    for(Eigen::Vector2f cord : countur) {
        ss << "G1 " << "X" << cord.x() << " Y" << cord.y() << std::endl;
        infillTrj.push_back(cord);
    }
    std::cout << "infill countur - OK\n";
 
    // Make infill zig-zag contour
    /*  __   __   __   __   _
       / /  / /  / /  / /  /
      / /  / /  / /  / /  /
    _/ /__/ /__/ /__/ /__/
        */
    ss << ";INFILL_ZIGZAG\n";  
    Eigen::Vector2f infillDim = cubeDims.array() - 2*wallThinkness - 2*nozzleD;
    bool fillFlag = true;
    Eigen::Vector2f xUp(nozzleD, 0.0f);
    Eigen::Vector2f yUp;
    Eigen::Vector2f start;
    Eigen::Vector2f end;
    if(dir) {
            yUp << 0.0f, -nozzleD;
            start << 0.0f, infillDim.y();
            end << infillDim.x() + infillTrj.back().x(), infillTrj.back().y(); 
    }
    else {
            yUp << 0.0f, nozzleD;
            start << 0.0f, 0.0f;
            end << infillTrj.back().x() + infillDim.x(), infillTrj.back().y() - nozzleD + infillDim.y(); 
    }
    start += infillTrj.back() + Eigen::Vector2f(nozzleD, -nozzleD);

    Eigen::Vector2f p1Inc = xUp;
    Eigen::Vector2f p2Inc = yUp;
    Eigen::Vector2f p1 = start, p2 = start;

    while(!(p1.x() > end.x() && p2.x() > end.x())) {
        p1 += p1Inc;
        p2 += p2Inc;

        if(p1.x() > end.x())
            p1Inc = yUp;
        if((p2.y() > end.y() && !dir) || (p2.y() < end.y() && dir))
            p2Inc = xUp;
        
        if(fillFlag) {
            infillTrj.push_back(p1);
            infillTrj.push_back(p2);
            fillFlag = false;
            ss << "G0 " << "F6000 " << "X " << p1.x() << " Y" << p1.y() << std::endl;
            ss << "G1 " << "F1200 " << "X " << p2.x() << " Y" << p2.y() << std::endl;
        }
        else {
            infillTrj.push_back(p2);
            infillTrj.push_back(p1);
            fillFlag = true;
            ss << "G0 " << "F6000 " << "X" << p2.x() << " Y" << p2.y() << std::endl;
            ss << "G1 " << "F1200 " << "X" << p1.x() << " Y" << p1.y() << std::endl;
        }
    }
    std::string output = ss.str();
    return output; 
}