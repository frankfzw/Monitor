"""
plot the result of the traffic
"""


def main():
	f = open('from', 'r')
	cluster = f.readline()
	interval = f.readline()
	print cluster
	print interval
	
	while True:
		line = f.readline()
		if line == '':
			break




if __name__ == "__main__":
	main()