# 基于OpenCV的三维结构重建设计
  
### 摘要   
恭喜您发现这个宝藏仓库，聪明的您应该发现咱这代码结构比其他仓库干净好使。  
我们提供一种基于OpenCV非接触的3D表面测量技术。  
<b>仓库没有提供对应的照片</b>   
如果你对图片如何从相机和投影仪中获取，可以看看我这个项目[hikvision-qt](https://github.com/timbrist/hikvision-qt).   


##### 投影图案
格雷码和相移图
![Project](https://github.com/timbrist/structure-light/blob/main/imgs/ProjectImg.png)

##### 双目相机标定
标定过程使用的是棋盘格  
(我们本来有圆点标定的，但是我弄丢那个板子了，所以有一个Calib.h用来做父类)  
![ChessBoard](https://github.com/timbrist/structure-light/blob/main/imgs/ChessBoard.png)

### 用法
如果你看到了这个项目， 我猜你应该安装了OpenCV3(只在版本3.xx测试过)
如果你用OpenCV，我想你应该对CMake不陌生了。
1. `git clone https://github.com/timbrist/structure-light.git`
2. `cd structure-light`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make -j4`
7. `./main`

### EXPECTATION  
把你的图片按照配置文件"./res/origin_images.xml"放在对应的目录里"../res/imgs/origin/"。
![INPUT](https://github.com/timbrist/structure-light/blob/main/imgs/Input.png)
  
This is the 3d cloud points:  
![OUTPUT](https://github.com/timbrist/structure-light/blob/main/imgs/Result3D.png)

