#include "marchingcubes.hpp"

MarchingCubes::MarchingCubes() {}

void MarchingCubes::mc(const Grid &grid, float isolevel, QList<Vector3f> &vertices, QList<uint> &indices, int &speed)
{
    generateGridCells(grid);

#pragma omp parallel for
    for(auto &cell : gridCells){
        polygonise(cell,isolevel,vertices,indices);
        QThread::msleep(speed);
    }
}

void MarchingCubes::generateGridCells(const Grid &grid)
{
    gridCells.clear();
    gridCells.squeeze();
    int resX = grid.getRes().x();
    int resY = grid.getRes().y();
    int resZ = grid.getRes().z();
#pragma omp parallel
    {
        QList<GridCell> threadGridCells;
#pragma omp for nowait
        for (int i = 0; i < resX - 1; ++i) {
            for (int j = 0; j < resY - 1; ++j) {
                for (int k = 0; k < resZ - 1; ++k) {
                    GridCell cell;

                    cell.vertices[0] = grid.getPoints()[i][j+1][k];
                    cell.vertices[1] = grid.getPoints()[i+1][j+1][k];
                    cell.vertices[2] = grid.getPoints()[i+1][j][k];
                    cell.vertices[3] = grid.getPoints()[i][j][k];
                    cell.vertices[4] = grid.getPoints()[i][j+1][k+1];
                    cell.vertices[5] = grid.getPoints()[i+1][j+1][k+1];
                    cell.vertices[6] = grid.getPoints()[i+1][j][k+1];
                    cell.vertices[7] = grid.getPoints()[i][j][k+1];

                    cell.val[0] = grid.getSdf()[i][j+1][k];
                    cell.val[1] = grid.getSdf()[i+1][j+1][k];
                    cell.val[2] = grid.getSdf()[i+1][j][k];
                    cell.val[3] = grid.getSdf()[i][j][k];
                    cell.val[4] = grid.getSdf()[i][j+1][k+1];
                    cell.val[5] = grid.getSdf()[i+1][j+1][k+1];
                    cell.val[6] = grid.getSdf()[i+1][j][k+1];
                    cell.val[7] = grid.getSdf()[i][j][k+1];

                    threadGridCells.push_back(std::move(cell));
                }
            }
        }

#pragma omp critical(gridCells)
        gridCells << threadGridCells;
    }
}

void MarchingCubes::polygonise(GridCell &cell, float isolevel, QList<Vector3f> &vertices, QList<uint> &indices)
{
    QList<Vector3f> interpolatedVertices(12);
    uint cubeIndex = 0;

    if(cell.val.at(0) < isolevel) cubeIndex |= 1;
    if(cell.val.at(1) < isolevel) cubeIndex |= 2;
    if(cell.val.at(2) < isolevel) cubeIndex |= 4;
    if(cell.val.at(3) < isolevel) cubeIndex |= 8;
    if(cell.val.at(4) < isolevel) cubeIndex |= 16;
    if(cell.val.at(5) < isolevel) cubeIndex |= 32;
    if(cell.val.at(6) < isolevel) cubeIndex |= 64;
    if(cell.val.at(7) < isolevel) cubeIndex |= 128;

    if(edgeTable.at(cubeIndex) == 0)
        return;

    if(edgeTable[cubeIndex] & 1)
        interpolatedVertices[0] = LinearInterpolate(cell.vertices[0],cell.vertices[1],isolevel,cell.val[0],cell.val[1]);
    if(edgeTable[cubeIndex] & 2)
        interpolatedVertices[1] = LinearInterpolate(cell.vertices[1],cell.vertices[2],isolevel,cell.val[1],cell.val[2]);
    if(edgeTable[cubeIndex] & 4)
        interpolatedVertices[2] = LinearInterpolate(cell.vertices[2],cell.vertices[3],isolevel,cell.val[2],cell.val[3]);
    if(edgeTable[cubeIndex] & 8)
        interpolatedVertices[3] = LinearInterpolate(cell.vertices[3],cell.vertices[0],isolevel,cell.val[3],cell.val[0]);
    if(edgeTable[cubeIndex] & 16)
        interpolatedVertices[4] = LinearInterpolate(cell.vertices[4],cell.vertices[5],isolevel,cell.val[4],cell.val[5]);
    if(edgeTable[cubeIndex] & 32)
        interpolatedVertices[5] = LinearInterpolate(cell.vertices[5],cell.vertices[6],isolevel,cell.val[5],cell.val[6]);
    if(edgeTable[cubeIndex] & 64)
        interpolatedVertices[6] = LinearInterpolate(cell.vertices[6],cell.vertices[7],isolevel,cell.val[6],cell.val[7]);
    if(edgeTable[cubeIndex] & 128)
        interpolatedVertices[7] = LinearInterpolate(cell.vertices[7],cell.vertices[4],isolevel,cell.val[7],cell.val[4]);
    if(edgeTable[cubeIndex] & 256)
        interpolatedVertices[8] = LinearInterpolate(cell.vertices[0],cell.vertices[4],isolevel,cell.val[0],cell.val[4]);
    if(edgeTable[cubeIndex] & 512)
        interpolatedVertices[9] = LinearInterpolate(cell.vertices[1],cell.vertices[5],isolevel,cell.val[1],cell.val[5]);
    if(edgeTable[cubeIndex] & 1024)
        interpolatedVertices[10] = LinearInterpolate(cell.vertices[2],cell.vertices[6],isolevel,cell.val[2],cell.val[6]);
    if(edgeTable[cubeIndex] & 2048)
        interpolatedVertices[11] = LinearInterpolate(cell.vertices[3],cell.vertices[7],isolevel,cell.val[3],cell.val[7]);
#pragma omp critical(triTable)
    for(int i = 0; triTable.at(cubeIndex).at(i) != -1;i += 3){
        uint indexOffset = indices.count();
        vertices << interpolatedVertices[triTable.at(cubeIndex).at(i)]
                 << interpolatedVertices[triTable.at(cubeIndex).at(i + 1)]
                 << interpolatedVertices[triTable.at(cubeIndex).at(i + 2)];
        indices << indexOffset
                << indexOffset + 1
                << indexOffset + 2;
    }
}

Vector3f MarchingCubes::LinearInterpolate(Vector3f p1, Vector3f p2, float isolevel, float valp1, float valp2)
{
    if(p2.x() < p1.x() || p2.y() < p1.y() || p2.z() < p1.z()){
        std::swap(p1,p2);
        std::swap(valp1,valp2);
    }
    return std::abs(valp1-valp2) > 1e-6 ?
               p1 + (p2 - p1) / (valp2 - valp1)*(isolevel - valp1) : p1;
}
