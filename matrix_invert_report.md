### 《高性能计算实践》实验报告（摘要）  
#### 一、实验名称与内容
##### 实验名称：  方阵A求逆
##### 实验内容：  矩阵求逆。
一个n×n阶的非奇异方阵A[aij]的逆矩阵是一个满足A-1A=AA-1=I的n×n阶方阵，其中I为单位方阵。

#### 二、实验环境的配置参数  
CPU型号与参数（虚拟机中分配数量为4）：i7-8565U，四核心八线程，主频 1.8GHz，动态加速频率 4.6GHz。
内存容量：4G（虚拟机中分配4g）  
![处理器参数](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%84%E7%90%86%E5%99%A8%E5%8F%82%E6%95%B0.png)
#### 三、方案设计  

本实验采用初等行变换并行化计算矩阵的逆矩阵。  
![矩阵求逆流程图](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

#### 四、实现方法
实验环境为Virtual Box中的CentOS-7
其中按照实验指导书，系统编译环境配置须安装如下软件包：
openMPI（openmpi-3.1.0.tar.gz）
gcc(gcc version 4.8.5)
算法实现语言：C语言

#### 五、结果分析
![矩阵求逆结果分析](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E7%BB%93%E6%9E%9C%E5%88%86%E6%9E%90.png)
结果分析：
如上表可以看出加速比可以反映采用并行算法相对于串行算法所体现出的优势，加速比数值越大，表明并行程序执行的效率越高，要将加速比的数值提高，也许需要增加并行处理的核心数量（进程数量）。
从并行多机实验结果可以看到，程序执行时间比串行算法的执行时间还要慢许多。从下图可以注意到，在linux1和linux2中各有一个进程在运行invert。但因为两台虚拟机之间需要进行网络数据传输，这一部分时间使得程序总体时间增加。  
![矩阵求逆并行多机](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E5%B9%B6%E8%A1%8C%E5%A4%9A%E6%9C%BA.png) 
此外，在程序中，我将矩阵输出部分注释掉，以便于关注程序计算的时间。否则在矩阵阶数增大的情况下，在终端打印出结果将耗费大量时间。
 
### 《高性能计算实践》实验报告（正文）

#### 一、实验名称与内容
实验名称：方阵A求逆
实验内容：矩阵求逆。一个n×n阶的非奇异方阵A[aij]的逆矩阵是一个满足A-1A=AA-1=I的n×n阶方阵，其中I为单位方阵。

#### 二、实验环境的配置参数
CPU型号与参数（虚拟机中分配数量为4）：i7-8565U，四核心八线程，主频 1.8GHz，动态加速频率 4.6GHz。
内存容量：4G（虚拟机中分配4g）  
![处理器参数](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%84%E7%90%86%E5%99%A8%E5%8F%82%E6%95%B0.png)  

#### 三、实验题目问题分析
本题目是一个常用的矩阵运算。  

矩阵求逆，首先明确什么是逆矩阵。如果A为n阶方阵，若存在n阶方阵B，使得AB=BA=I，其中I为单位矩阵，则称矩阵B为A的逆矩阵，此处逆矩阵B我们记做A^(-1)，即逆矩阵是指满足A^(-1) A=AA^(-1)=I的n×n阶方阵，其中I为单位方阵。  

矩阵求逆被大量用于各种场景，常用的求逆算法有：代数余子式、初等行变换、最小二乘法、QR分解法、Jacobi数值法、LU分解法、极小多项式求逆等等。  

查阅参考文献后并行化方案有：基于CUDA的大型实对称矩阵并行求逆算法[1]，该算法在分块矩阵迭代求逆过程中，对向量相乘、矩阵相乘、向量数乘等运算进行并行化处理。大规模矩阵的MPI并行求逆算法设计与分析[2],该算法基于最小二乘法对高阶稠密对称正定矩阵并行求逆。一个矩阵求逆的并行算法[3]介绍了一个采用不选主元加列法求逆矩阵的并行算法。还有论文[4][5][6][7]介绍了几种不同的并行化计算方法。  


#### 四、方案设计  
本实验采用初等行变换并行化计算矩阵的逆矩阵。
##### 1.求方阵的逆的串行算法
![1.求方阵的逆的串行算法](https://github.com/Gongyihang/MPIGA/blob/master/pictures/1.%E6%B1%82%E6%96%B9%E9%98%B5%E7%9A%84%E9%80%86%E7%9A%84%E4%B8%B2%E8%A1%8C%E7%AE%97%E6%B3%95.png)
![1.1单处理器上的矩阵求逆算法](https://github.com/Gongyihang/MPIGA/blob/master/pictures/1.1%E5%8D%95%E5%A4%84%E7%90%86%E5%99%A8%E4%B8%8A%E7%9A%84%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E7%AE%97%E6%B3%95.png)
![2.方阵求逆的并行算法](https://github.com/Gongyihang/MPIGA/blob/master/pictures/2.%E6%96%B9%E9%98%B5%E6%B1%82%E9%80%86%E7%9A%84%E5%B9%B6%E8%A1%8C%E7%AE%97%E6%B3%95.png)
![2.1 矩阵求逆的并行算法1](https://github.com/Gongyihang/MPIGA/blob/master/pictures/2.1%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E7%9A%84%E5%B9%B6%E8%A1%8C%E7%AE%97%E6%B3%951.png)
![2.1矩阵求逆的并行算法2](https://github.com/Gongyihang/MPIGA/blob/master/pictures/2.1%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E7%9A%84%E5%B9%B6%E8%A1%8C%E7%AE%97%E6%B3%952.png)
若一次乘法和加法运算或一次除法运算时间为一个单位时间，则算法2.1所需的计算时间为 m n^2 ；又由于共有n行数据依次作为主行被广播，其通信时间为 n (ts+ n tw)logp，所以该算法并行计算时间为 Tp=m n2 +n (ts+ n tw)logp。

#### 五、实现方法
实验环境为Virtual Box中的CentOS-7
其中按照实验指导书，系统编译环境配置须安装如下软件包：
openMPI（openmpi-3.1.0.tar.gz）
gcc(gcc version 4.8.5)
算法实现语言：C语言
在虚拟机中整个程序运行步骤如下：
##### 1.使用data.cpp生成1200阶方阵保存在data.txt中。
本实验使用data.cpp生成一个1200阶的随机方阵，方阵元素使用rand函数随机生成。生成的矩阵保存在data.txt文件中。
c++文件编译生成命令，如：
g++ -o data data.cpp
./data
##### 2.源代码的编译命令为mpic++，如：
（其中invertc.c为串行算法，串行算法可以直接使用./invertc运行。invert.c为并行算法）
g++ -o invertc invertc.c
./invert
mpicc -o invert invert.c
mpirun -np 2 ./invert
##### 3.在运行可执行文件命令前面加上time可以获取程序的执行时间。如：
time mpirun -np 2 ./invert
结果的格式如下：
real 0m0.135s
user 0m0.889s
sys 0m0.005s
其中 real 为程序执行所用的实际时间， user 为所有线程在实际计算的时间总和， sys 为
程序执行中使用系统命令执行的时间。
我们只使用 real 一项。
##### 4.在配置好linux1，linux2免密登陆的情况下，使用多机多进程运行命令如下：
mpirun -host linux1,linux2 -np 2 ./test -mca btl_tcp_if_include enp0s3

上述命令中的-mca btl_tcp_if_include enp0s3是为了解决以下警告：
WARNING: Open MPI accepted a TCP connection from what appears to be a another Open MPI process but cannot find a corresponding process entry for that peer.
解决：使用ifconfig查找通信使用的网卡，我的机器网卡是enp0s3

#### 六、结果分析
本结果通过改变使用的核数来观察实验结果。测试程序运行时间，取十次运行的平均值。
##### 1.串行算法
示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E4%B8%B2%E8%A1%8Cinvert1.png)  

十次运行结果如下：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E4%B8%B2%E8%A1%8Cinvert_res.png)  

2.并行算法  

单核示例：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C%E5%8D%95%E6%A0%B8.png)  

十次运行结果如下： 

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C%E5%8D%95%E6%A0%B8invert_res.png)  

2核心示例：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C2%E6%A0%B8invert.png)  

十次运行结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C2%E6%A0%B8invert_res.png)

3核心示例：  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C3%E6%A0%B8invert.png)  

十次运行结果如下：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C3%E6%A0%B8invert_res.png)  

4核心示例：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C4%E6%A0%B8invert.png)  

十次运行结果如下：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C4%E6%A0%B8invert_res.png)  


3.多机并行  

示例：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%9A%E6%9C%BA%E5%B9%B6%E8%A1%8Cinvert.png)  

十次运行结果如下：  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%9A%E6%9C%BA%E5%B9%B6%E8%A1%8Cinvert_res.png)  


结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/invert%E7%BB%93%E6%9E%9C1.png)  

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/invert%E7%BB%93%E6%9E%9C2.png)  


结果分析：
如上表可以看出加速比可以反映采用并行算法相对于串行算法所体现出的优势，加速比数值越大，表明并行程序执行的效率越高，要将加速比的数值提高，也许需要增加并行处理的核心数量（进程数量）。
从并行多机实验结果可以看到，程序执行时间比串行算法的执行时间还要慢许多。从下图可以注意到，在linux1和linux2中各有一个进程在运行invert。但因为两台虚拟机之间需要进行网络数据传输，这一部分时间使得程序总体时间增加。

![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E7%9F%A9%E9%98%B5%E6%B1%82%E9%80%86%E5%B9%B6%E8%A1%8C%E5%A4%9A%E6%9C%BA.png)
 
此外，在程序中，我将矩阵输出部分注释掉，以便于关注程序计算的时间。否则在矩阵阶数增大的情况下，在终端打印出结果将耗费大量时间。

#### 七、个人总结
通过这次实验，我明白现实中并行计算中需要考虑的因素很多，主要的是并行计算的算法和处理器性能，其次还有网络传输方式、程序输入输出方式等等。假设所有计算机处理能力相同，并且多台机器间网络传输速率也基本相同，之所以做这样的假设，是因为先不考虑硬件问题，主要考虑算法本身节点数目是否越多，加速比的值就会越大，当输入设备输出设备效率很差，即输入输出的时间在并行计算中占很大比例，无论节点数目再多，计算能力再强，网络处理速度再快，它与输入输出所占时间相比，也微乎其微，所以并不是节点数目越多，加速比的值就会越大，要提高并行程序执行效率，必须考虑多方面的因素。
总之，在设计一个问题的并行算法时，讨论这个并行算法的好坏时，必须从多方面进行分析比对，比如要考虑节点机的数目，网络状况以及输入输出效率，进而对算法的执行时间，加速比，处理器的被利用的程度以及并行算法所需的开销等去分析，综合评判一个并行程序的性能指标。  

参考文献：  

[1]霍迎秋,王武星,彭楚风,方勇.基于CUDA的大型实对称矩阵并行求逆算法[J].计算机工程与设计,2015,36(08):2133-2137.  
[2]周浩,罗志才,钟波.大规模矩阵的MPI并行求逆算法设计与分析[J].大地测量与地球动力学,2014,34(05):120-124.  
[3]刘桂清. 一个矩阵求逆的并行算法[C]. 中国数学力学物理学高新技术交叉研究学会.数学·力学·物理学·高新技术交叉研究进展——2010（13）卷.中国数学力学物理学高新技术交叉研究学会:中国数学力学物理学高新技术交叉研究学会,2010:835-839.  
[4]魏琼.矩阵求逆算法在Cell上的并行[J].程序员,2008(08):90-92.  
[5]曾庆华,孙世新,陈天麒.基于LogP简化模型的矩阵求逆并行算法研究[J].计算机科学,2003(08):176-177+184.  
[6]陈军,王正华,李晓梅.高阶稠密矩阵的并行求逆与应用[J].国防科技大学学报,1997(04):3-6.  
[7]莫则尧,李晓梅.树网结构上一种新的矩阵迭代求逆并行算法[J].国防科技大学学报,1994(01):60-65.  


1.防火墙关闭后才能使用多机并行
使用命令：systemctl status firewalld.service
查看防火墙状态
执行后可以看到绿色字样标注的“active（running）”，说明防火墙是开启状态
使用命令：systemctl stop firewalld.service    
关闭运行的防火墙
关闭后，使用命令systemctl status firewalld.service
查看防火墙状态
可以看到，disavtive（dead）的字样，说明防火墙已经关闭
参考：https://jingyan.baidu.com/article/ff42efa9fd8c1cc19e2202bb.html


2.创建文件和删除文件  创建文件夹  
touch data.txt  
rm data.txt  
mkdir dir  

3.查看隐藏文件  
ctrl+h  

4.拷贝文件  
scp 路径：file1 file2 路径：file3 file4  

5.多机免密登陆中，在一台机器生成密钥。  
ssh -keygen -t rsa  
cd .ssh  
ls  
其中id_rsa.pub改名为authorized_keys传到其他机器的.ssh目录下，即可免密登陆  
可能用到cat id_rsa.pub >> 路径：authorized_keys  

6.MPI学习  
在C++中使用MPI库的编译和运行命令(linux)。  
mpicxx 文件 -o 可执行文件(可以使用Makefile)  
单机上运行：  
mpirun -np 进程个数./可执行文件  
在集群上运行：  
mpirun -f./hostfile-np 进程个数./可执行文件  
hostfile文件的格式：  
ip地址：进程个数  
ip地址：进程个数  
ip地址：进程个数  


MPICH2简单的安装配置总结  
https://blog.csdn.net/cleverysm/article/details/1638651
Linux虚拟机之间免密登录的实现  
https://blog.csdn.net/thor027/article/details/76635051
MPI程序中的多进程写冲突问题的解决与遗留问题  
https://blog.csdn.net/king_lu/article/details/4008667
MPI单机多机方法  
https://blog.csdn.net/chseanch/article/details/26697765?locationNum=11  


