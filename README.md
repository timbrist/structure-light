# structure-light
3D reconstruction with Structure Light

[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/timbrist/structure-light/README.md)

[![中文](https://img.shields.io/badge/lang-中文-red.svg)](https://github.com/timbrist/structure-light/blob/main/README.cn.md)

<b>abstract </b>
In the three-dimensional reconstruction, in order to quickly and accurately obtain the surface depth information of the object to be measured in space, this paper proposes a three-dimensional measurement system based on the structured light coding pattern based on OpenCV and 6 gray coding +4 phase shift maps.  
In order to reduce the repetitiveness of the research on the 3D reconstruction of the object surface, this paper combines the software design principles to implement the specific algorithm with C++ code.  
the mono-linear interpolation algorithm is used to improve the algorithm module of stereo matching. In the structured light coding pattern, the coding structure of 6 Gray codes +4 phase shift maps is used, and the error rate of the 6 Gray codes is low. Using this structured light coding pattern, the stripes can be decoded without loss.  
From the optical path formula of the Young's double-slit experiment to explain why the four phase shift maps can measure the distance of an object. For the reliability of the project, the entire system will use C++, OpenCV to process the image matrix. This makes the system running speed tend to be acceptable.  
<b>Note that this repository doesn't containt any picture, compile and run doesn't work here. 
But I believe if you have base background knowledge, you will easily use the code.</b>