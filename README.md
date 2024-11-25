# MarchingCubesQT

A Marching Cubes implementation using QT and Eigen

## Plans for the future

- rewrite
- cuda and general optimisation of SDF calculation
- inclusion of dual marching cubes and marching tetrahedra


## Example Output
![Screenshot from 2024-06-20 15-02-33](https://github.com/user-attachments/assets/89852ba6-92e5-4054-a8b1-f4705756de61)

## Dependencies

[vcpkg](https://vcpkg.io/en/)

[QT](https://www.qt.io/download-open-source)

Eigen

```bash
  vcpkg install eigen3
```
## Build the Project

Clone the project

```bash
  git clone https://github.com/Salzaufsauge/MarchingCubesQT
```

Enable the VCPKG Plugin in QTCreator

Add the VCPKG path to preferences/CMake/Vcpkg

Open the project in QTCreator

Choose any kit

Wait for project to setup

Click on the green play button or use CTRL+R





## Acknowledgements

 - [Lookup-tables and base implementation](https://paulbourke.net/geometry/polygonise/)
 - [Building a BVH](https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/)


## License

[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)


