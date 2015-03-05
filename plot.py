"""
plot the result of the traffic
"""
import matplotlib.pyplot as plt
import numpy as np
import random

def plot(filename):
	f = open(filename, 'r')
	cluster = f.readline()
	interval = f.readline()
	print cluster
	print interval
	delta = float(interval) / 1000
	clusterArray = []
	dataMatrix = [[]for i in xrange(int(cluster))]
	xArray = []
	xArray.append(0)
	formerData = [0 for i in xrange(int(cluster))]
	#print dataMatrix
	for i in xrange(int(cluster)):
		line = f.readline()
		res = line.split('\t')
		clusterArray.append(res[0]);
		formerData[i] = int(res[1][:-1]) - formerData[i]
		dataMatrix[i].append(0)
		#print i
	#print clusterArray
	#print formerData

	tag = 1
	index = 0
	while tag:
		for i in xrange(int(cluster)):
			line = f.readline()
			if line == '':
				tag = 0
				break
			res = line.split('\t')
			dataMatrix[i].append(int(res[1][:-1]) - formerData[i])
			formerData[i] = int(res[1][:-1])
		if tag == 0:
			break
		xArray.append(xArray[index] + delta)
		index = index + 1
	#print dataMatrix
	#print len(xArray)
	#print len(dataMatrix[1])
	
	#plot
	for i in xrange(int(cluster)):
		r = random.random()
		g = random.random()
		b = random.random()
		plt.plot(xArray, dataMatrix[i], label=clusterArray[i], color=(r, g, b), linewidth=1.5)
		#plt.text(10, (150 - 5 * i), clusterArray[i], color=(r, g, b))
	plt.xlabel("Time(ms)")
	plt.ylabel("Traffic")
	plt.title(filename)
	plt.legend()
	plt.show()


def main():
	plot('from')
	plot('to')



if __name__ == "__main__":
	main()