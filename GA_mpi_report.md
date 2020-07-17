### 《高性能计算实践》实验报告（摘要）  

#### 一、实验名称与内容
##### 实验名称：基于MPI并行遗传算法求解TSP问题
##### 实验内容：基于MPI并行遗传算法求解TSP问题  

#### 二、实验环境的配置参数
CPU型号与参数：i7-8565U，四核心八线程，主频 1.8GHz，动态加速频率 4.6GHz。  
内存容量：16G  
![处理器参数](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%84%E7%90%86%E5%99%A8%E5%8F%82%E6%95%B0.png)  

#### 三、方案设计
本实验采用种群划分来并行化遗传算法求解TSP问题。
种群划分操作是根据同时运行的进程数来划分，例如在串行算法中，种群数量为960，若进程数量为4，则将960个种群划分为4组子种群，每组种群数量为240。每组种群同时进行选择、交叉、变异操作，同时得到4个子种群的最优解。然后取4个子种群解中最短的路径作为全部解的最优解。在更新每一代得到的子种群最优解后，将当前最优解广播至所有进程进行交流更新。
其中每一组子种群可以拥有不同的选择、交叉和变异几率。
具体流程图如下：  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E6%B5%81%E7%A8%8B%E5%9B%BE.png)
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E6%B5%81%E7%A8%8B%E5%9B%BE2.png)

#### 四、实现方法
实验环境为windows10，vscode，python3.6.8，以及所需的numpy，matplotlib，mpi4py等库。
算法实现语言：python  

#### 五、结果分析  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E7%BB%93%E6%9E%9C%E5%88%86%E6%9E%90.png)
 
##### 结果分析：
如上表可以看出加速比可以反映采用并行算法相对于串行算法所体现出的优势，加速比数值越大，表明并行程序执行的效率越高，要将加速比的数值提高，也许需要增加并行处理的核心数量（进程数量）。
本次实验通过多进程方式实现，从结果看，将大种群划分为子种群同时进行遗传算法可以节约大量时间，并且将每一组子种群设置不同的选择、交叉和变异概率，能够在相同的遗传代数的情况下，使遗传算法更快地收敛到相比串行算法而言更好的结果。
因为代码中代码只进行了子种群的划分这个并行化，其实还可以将每组子种群收敛到的最优结果，再广播到所有子种群，进行“交流”。
 
### 《高性能计算实践》实验报告（正文）

#### 一、实验名称与内容
##### 实验名称：基于MPI并行遗传算法求解TSP问题
##### 实验内容：基于MPI并行遗传算法求解TSP问题  

#### 二、实验环境的配置参数
CPU型号与参数：i7-8565U，四核心八线程，主频 1.8GHz，动态加速频率 4.6GHz。
内存容量：16G
![处理器参数](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%A4%84%E7%90%86%E5%99%A8%E5%8F%82%E6%95%B0.png)  

#### 三、实验题目问题分析  
首先分析一下旅行商问题（Travelling salesman problem, TSP），TSP问题即给定一系列城市和每对城市之间的距离，求解访问每一座城市一次并回到起始城市的最短回路。它是组合优化中的一个NP-hard问题，在运筹学和理论计算机科学中非常重要。
其次分析一下遗传算法（Genetic Algorithm，GA），该算法是根据大自然中生物体进化规律而设计提出的。是模拟达尔文生物进化论的自然选择和遗传学机理的生物进化过程的计算模型，是一种通过模拟自然进化过程搜索最优解的方法。该算法通过数学的方式,利用计算机仿真运算,将问题的求解过程转换成类似生物进化中的染色体基因的交叉、变异等过程。在求解较为复杂的组合优化问题时,相对一些常规的优化算法,通常能够较快地获得较好的优化结果。遗传算法已被人们广泛地应用于组合优化、机器学习、信号处理、自适应控制和人工生命等领域。  

#### 四、方案设计
本实验采用python实现串行遗传算法框架，主要过程包含有：初始化种群、计算适应度、选择、交叉、变异。
当采用大种群的遗传算法时，我们需要利用适应度信息来产生下一代种群，这时候每一代繁殖的过程将会很耗时。但是值得注意的是，种群的选择、交叉、变异过程对于种群中的个体是相互独立的，我们可以将这一部分进行并行处理来加速遗传算法的迭代。  

使用matplotlib可视化TSP问题求解过程。
使用mpi4py库实现多进程并行化遗传算法求解TSP问题。
##### 1.遗传算法求解TSP问题的串行算法
输入：
城市数量：96个
交叉配对比率：0.1
变异的概率：0.02
种群的数量：960
变异的代数：100
输出：优化100代后，所得到的最短路径长度。  

##### 2.遗传算法并行算法
在串行算法的基础上编写MPI多进程算法。
本实验采用种群划分来并行化遗传算法求解TSP问题。
种群划分操作是根据同时运行的进程数来划分，例如在串行算法中，种群数量为960，若进程数量为4，则将960个种群划分为4组子种群，每组种群数量为240。每组种群同时进行选择、交叉、变异操作，同时得到4个子种群的最优解。然后取4个子种群解中最短的路径作为全部解的最优解。在更新每一代得到的子种群最优解后，将当前最优解广播至所有进程进行交流更新。
其中每一组子种群可以拥有不同的选择、交叉和变异几率。
具体流程图如下：  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E6%B5%81%E7%A8%8B%E5%9B%BE.png)
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E6%B5%81%E7%A8%8B%E5%9B%BE2.png)  

#### 五、实现方法
实验环境为windows10，vscode，python3.6.8，以及所需的numpy，matplotlib，mpi4py等库。
算法实现语言：python
在Anaconda Prompt(py36)中整个程序运行步骤如下：  
##### 1.运行data96.py生成如下数据文件：
96个城市坐标：cities96.txt
960个初始种群：data96960.txt
240个不同的初始种群：data962400.txt、data962401.txt、data962402.txt、data962403.txt
##### 2.运行串行遗传算法TSPc.py
python TSPc.py
##### 3.运行并行遗传算法TSPmpi.py
使用anaconda prompt在源码所在目录下执行以下命令分别得到多进程实验结果。
mpiexec -n 4 python TSPmpi.py
其中4为进程数量
##### 4.运行的结果
使用python的time库对程序运行时间进行计时
start = time.clock()
func()
end = time.end()
最后打印运行时间和最短路径：
print('Time: ', end - start, 's | total_distance: ',total_distance[best_idx])
例：
Time:  157.22985229999998 s | total_distance:  6.462503133636991	

#### 六、结果分析
本结果通过改变使用的核数来观察实验结果。测试程序运行时间，取十次运行的平均值。  

##### 1.串行算法（100代）
示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E4%B8%B2%E8%A1%8C%EF%BC%88100%E4%BB%A3%EF%BC%89.png)  

十次运行结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E4%B8%B2%E8%A1%8C%EF%BC%88100%E4%BB%A3%EF%BC%89_res.png)  

##### 2.并行算法
单核示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C%E5%8D%95%E6%A0%B8GA.png)  

十次运行结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C%E5%8D%95%E6%A0%B8GA_res.png)

2核心示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C2%E6%A0%B8GA.png)  

十次运行结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C2%E6%A0%B8GA_res.png)

3核心示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C3%E6%A0%B8GA.png)  

十次运行结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C3%E6%A0%B8GA_res.png)

4核心示例：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C4%E6%A0%B8GA.png)  

十次运行结果如下：  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/%E5%B9%B6%E8%A1%8C4%E6%A0%B8GA_res.png)  

##### 结果如下：
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E7%BB%93%E6%9E%9C1.png)
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E7%BB%93%E6%9E%9C2.png)
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E7%BB%93%E6%9E%9C3.png)
 
##### 结果分析：  
如上表可以看出加速比可以反映采用并行算法相对于串行算法所体现出的优势，加速比数值越大，表明并行程序执行的效率越高，要将加速比的数值提高，也许需要增加并行处理的核心数量（进程数量）。
本次实验通过多进程方式实现，从结果看，将大种群划分为子种群同时进行遗传算法可以节约大量时间，并且将每一组子种群设置不同的选择、交叉和变异概率，能够在相同的遗传代数的情况下，使遗传算法更快地收敛到相比串行算法而言更好的结果。
因为代码中代码只进行了子种群的划分这个并行化，其实还可以将每组子种群收敛到的最优结果，再广播到所有子种群，进行“交流”。  

#### 七、个人总结  

通过这次实验，我了解了python中mpi4py库的使用。
学习了python可视化库matplotlib。
如将TSP问题可视化可得如下图片：  
![](https://github.com/Gongyihang/MPIGA/blob/master/pictures/GA%E7%BB%93%E6%9E%9C4.png)