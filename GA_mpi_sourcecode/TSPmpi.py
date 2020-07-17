from mpi4py import MPI
import random
import numpy as np
from original_tsp import GA,TravelSalesPerson
import matplotlib.pyplot as plt
import time


start = time.clock()

comm = MPI.COMM_WORLD
comm_rank = comm.Get_rank()
comm_size = comm.Get_size()

#城市的数量
N_CITIES = 96  # DNA size
#交叉配对的比率
CROSS_RATE = 0.1
#变异的概率
MUTATE_RATE = 0.02
#种群的数量
POP_SIZE_ALL = 960
#变异的代数
N_GENERATIONS = 100


#交叉配对的比率
CROSS_RATE1 = 0.025
#变异的概率
MUTATE_RATE1 = 0.01


#交叉配对的比率
CROSS_RATE2 = 0.05
#变异的概率
MUTATE_RATE2 = 0.015


#交叉配对的比率
CROSS_RATE3 = 0.05
#变异的概率
MUTATE_RATE3 = 0.005

POP_SIZE = POP_SIZE_ALL // comm_size


if comm_rank == 0:
    #遗传算法的实体
    data0 = "data962400.txt"
    ga = GA(DNA_size=N_CITIES, cross_rate=CROSS_RATE, mutation_rate=MUTATE_RATE, pop_size=POP_SIZE, data = data0)
    #环境
    env = TravelSalesPerson(N_CITIES)
elif comm_rank == 1:
    #遗传算法的实体
    data1 = "data962401.txt"
    ga = GA(DNA_size=N_CITIES, cross_rate=CROSS_RATE1, mutation_rate=MUTATE_RATE1, pop_size=POP_SIZE, data = data1)
    #环境
    env = TravelSalesPerson(N_CITIES)
elif comm_rank == 2:
    #遗传算法的实体
    data2 = "data962402.txt"
    ga = GA(DNA_size=N_CITIES, cross_rate=CROSS_RATE2, mutation_rate=MUTATE_RATE2, pop_size=POP_SIZE, data = data2)
    #环境
    env = TravelSalesPerson(N_CITIES)
elif comm_rank == 3:
    #遗传算法的实体
    data3 = "data962403.txt"
    ga = GA(DNA_size=N_CITIES, cross_rate=CROSS_RATE3, mutation_rate=MUTATE_RATE3, pop_size=POP_SIZE, data = data3)
    #环境
    env = TravelSalesPerson(N_CITIES)
# else:
#     #遗传算法的实体
#     data1 = "data962400.txt"
#     ga = GA(DNA_size=N_CITIES, cross_rate=CROSS_RATE, mutation_rate=MUTATE_RATE, pop_size=POP_SIZE, data = data1)
#     #环境
#     env = TravelSalesPerson(N_CITIES)




for generation in range(N_GENERATIONS):
    #翻译DNA,env.city_position表示城市所在的位置，并打印其坐标
    lx, ly = ga.translateDNA(ga.pop, env.city_position)
    #整个路线的长度
    fitness, total_distance = ga.get_fitness(lx, ly)
    #进化DNA
    ga.evolve(fitness)
    #可视化
    best_idx = np.argmax(fitness)

    # print(comm_rank,' Gen:', generation, '| best fit: %.2f' % fitness[best_idx],)

    # env.plotting(lx[best_idx], ly[best_idx], total_distance[best_idx])

    # if generation % 5 == 0:
    best = comm.bcast(fitness[best_idx])


    worst_idx = np.argmin(fitness)

    fitness[worst_idx] = best

    if generation == (N_GENERATIONS - 1):
        end = time.clock()
        print('Rank: ', comm_rank, 'Time: ', end - start, 's | total_distance: ',total_distance[best_idx])

# plt.ioff()
# plt.show()

