# Structure Light
3D reconstruction with Structure Light base on OpenCV  

[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/timbrist/structure-light/README.md)

[![中文](https://img.shields.io/badge/lang-中文-red.svg)](https://github.com/timbrist/structure-light/blob/main/README.cn.md)

### ABSTRACT
Congratulations for finding this repository, people as smart as you might notice that this repository has more clean structure, easy to use.  
We provide an noncontact 3D surface measurement techniques using OpenCV.  
<b>Note that this repository doesn't containt any pictures</b>   
If You are interested in taking picture from camera and project, You can have a look at my [hikvision-qt](https://github.com/timbrist/hikvision-qt).   
If You just want to have compile and run, I can send you my previous pictures.   

##### Projected Patterns
The Projected Patterns contains gray codes and phase shift  
![Project](https://github.com/timbrist/structure-light/blob/main/imgs/ProjectImg.png)

##### Calibration
The Calibration process uses Chessboard for canmera parameters.  
(It supposed to have Dot array calibration pattern... but I didn't have the board.)  
![ChessBoard](https://github.com/timbrist/structure-light/blob/main/imgs/ChessBoard.png)

### USAGE
If You are interested in this project, I assume that you will install OpenCV3 (only test in version 3.xx).
And Since you are using OpenCV, I guess you also using CMake;
1. `git clone https://github.com/timbrist/structure-light.git`
2. `cd structure-light`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make -j4`
7. `./main`

### EXPECTATION  
Put your images into the folder "../res/imgs/origin/",
Name the images as "./res/origin_images.xml" suggested.  
![INPUT](https://github.com/timbrist/structure-light/blob/main/imgs/Input.png)
  
This is the 3d cloud points:  
![OUTPUT](https://github.com/timbrist/structure-light/blob/main/imgs/Result3D.png)

