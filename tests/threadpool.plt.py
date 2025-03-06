import matplotlib.pyplot as plt

tasks = [100, 200, 400, 800, 1600, 3200, 6400]
cost_4 = [0.393529, 0.784035, 1.60549, 3.17782, 6.28152, 12.7449, 25.4605]
cost_8 = [0.203002, 0.391424, 0.801944, 1.56535, 3.16497, 6.40223, 12.6028]
cost_15 = [0.10822, 0.218724, 0.429045, 0.839471, 1.68367, 3.40006, 6.79768]

plt.plot(tasks, cost_4, marker='o', label='Pool Size 4')
plt.plot(tasks, cost_8, marker='o', label='Pool Size 8')
plt.plot(tasks, cost_15, marker='o', label='Pool Size 15')
plt.xlabel('任务数量')
plt.ylabel('耗时')
plt.title('线程池性能对比')
plt.legend()
plt.show()