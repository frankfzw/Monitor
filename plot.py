"""
plot the result of the traffic
"""
import matplotlib.pyplot as plt
import numpy as np
import random
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--configure', '-c', dest='confPath', type=str, help='Path of Configure File of Cluster')
parser.add_argument('--data', '-d', dest='dataPath', type=str, help='Path of Traffic Data Directory')
args = parser.parse_args()


def plot(filename, title):
	f = open(filename, 'r')
	cluster = f.readline()
	interval = f.readline()
	print cluster
	print interval
	delta = float(interval) / 1000
	clusterArray = []
	dataMatrix = [[]for i in xrange(int(cluster))]

	#number of data set
	#setNum = 1
	#xArray = []
	#xArray.append(0)
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
	#index = 0
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
		#xArray.append(xArray[index] + delta)
		#index = index + 1
		#setNum = setNum + 1
	#print dataMatrix
	#print len(xArray)
	#print len(dataMatrix[1])
	
	#plot
	barWidth = 0.35
	for i in xrange(int(cluster)):
		r = random.random()
		g = random.random()
		b = random.random()
		index = np.arange(len(dataMatrix[i]))
		print str(i) + ': ' + str(len(index))
		#plt.bar(index + (i * barWidth), dataMatrix[i], barWidth,
		#	color=(r, g, b),
		#	label=clusterArray[i])
		plt.plot(index, dataMatrix[i], label=clusterArray[i], color=(r, g, b), linewidth=1.5)
		#plt.text(10, (150 - 5 * i), clusterArray[i], color=(r, g, b))
	plt.xlabel("Time(ms)")
	plt.ylabel("Traffic")
	plt.title(title)
	plt.legend()
	plt.show()


def main():
	confPath = args.confPath
	f = open(confPath, 'r')
	clusterNum = 0
	for line in f:
		if ':' in line:
			continue
		plot(args.dataPath + '/' + line[:-1] + '_in', line[:-1] + ' in')
		plot(args.dataPath + '/' + line[:-1] + '_out', line[:-1] + ' out')
		
	#get cluster ip
	"""
	for i in xrange(clusterNum):
		slave = f.readline()
		plot(args.dataPath + '/' + slave + '_in', slave + '\tin')
		plot(args.dataPath + '/' + slave + '_out', slave + '\tout')
	#plot('/home/franklab/hadoop-src/data/192.168.3.103_in')
	#plot('/home/franklab/hadoop-src/data/192.168.3.103_out')
	"""



if __name__ == "__main__":
	main()