# CADprojectFen
This is the project for cad course of Teacher Gao of zju modified on the CADproject. Portal:http://www.cnblogs.com/tiny656/p/3537085.html?utm_source=tuicool&utm_medium=referral.

本项目在VS 2015 pro中编译运行。项目实现的是先通过欧拉操作实现一个带五个孔的平面图形：
![image](https://github.com/cians/CADprojectFen/blob/master/Fen.jpg)
 
然后通过扫成生成三维形体。
交互：按住鼠标右键拖动可以旋转观察角度，按住鼠标左键上下拖动可以放大缩小。
实现的方法，是先是用户定义目标体的顶点的数目和欧拉操作步骤数，然后依次输入点的坐标，再定义欧拉操作的流程，流程如下：
mvfs;
mev x 7;
mef ;//至此生成无内环的底面
mev x 3;
mef ;//生成三角面1
mev x 3;
mef ;//生成三角面2
mev x 3;
mef ;//生成三角面3
mev x 3;
mef ;//生成三角面4
mev x 4;
mef ;//生成中间面
kemr x 5 //依次生成五个内环
kfmrh x 5;//删除内环对应的面，形成孔
sweep;
并输出对应的BRP文件。然后程序生成线框表示的三维形体。输出的BRP文件经欧拉渲染框架验证得到的三维形体与目标一致。
