import math
from math import *

class QuickMaths(object):
	@staticmethod
	def getNormalized(vector):
		magnitude = math.sqrt(vector[0]**2 + vector[1]**2)
		if (magnitude == 0):
			return (0,0)
		return (vector[0]/magnitude, vector[1]/magnitude)

	@staticmethod
	def getMagnitude(vecA, vecB):
		return math.sqrt((vecA[0] - vecB[0])**2 + (vecA[1] - vecB[1])**2)

	@staticmethod
	def getScaled(vector, val):
		return (vector[0] * val, vector[1] * val)

	@staticmethod
	def addTuples(vecA, vecB):
		return(vecA[0] + vecB[0], vecA[1] + vecB[1])

	@staticmethod
	def subTuples(vecA, vecB):
		return(vecA[0] - vecB[0], vecA[1] - vecB[1])

	@staticmethod
	def findClosest(tar, collection):
		if (len(collection) > 0):
			temp = collection[0]
			i = 1
			while (i < len(collection)-1):
				print(collection[i].position)
				if (QuickMaths.getMagnitude(tar.position, temp.position) > QuickMaths.getMagnitude(tar.position, collection[i].position)):
					temp = collection
				i+=1
			return temp
		return tar