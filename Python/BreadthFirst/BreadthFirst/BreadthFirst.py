import time as t
import pygame
import Nodes
import queue as q
from pygame import *
from Nodes import *
from queue import *

def BreadthFirst(nodeList, startNode, endNode):
	toVisit = [startNode]
	curr = startNode
	path = []
	curr.backNode = 0
	startNode.col = (255,0,0)
	endNode.col = (0,0,0)
	for nodeRow in nodeList:
		for node in nodeRow:
			node.visited = False
			node.backNode = 0

	while (len(toVisit) > 0):
		curr = toVisit.pop(0)
		for neighbor in curr.neighbors:
			if(neighbor[0].visited==False):
				if (neighbor[0] not in toVisit):
					toVisit.append(neighbor[0])
				neighbor[0].visited = True
				neighbor[0].backNode = curr
				if (neighbor[0] == endNode):
					print("Found end node")
					startNode.backNode = 0
					path.append(neighbor[0])
					while (curr != 0):
						path.append(curr)
						curr = curr.backNode
					return path
	return path

def DijkstrasSearch(nodeList, startNode, endNode):
	toVisit = [startNode]
	path = []
	for nodeRow in nodeList:
		for node in nodeRow:
			node.visited = False
			node.backNode = 0
			node.costSoFar = 2000
			
	startNode.col = (255,0,0)
	endNode.col = (0,0,0)
	startNode.costSoFar = 0
	curr = startNode
	curr.backNode = 0

	while (len(toVisit) > 0):
		curr = toVisit.pop(0)
		for neighbor in curr.neighbors:
			currDist = curr.position.distance(neighbor[0].position)
			if(neighbor[0].visited==False):
				if (neighbor[0] not in toVisit):
					toVisit.append(neighbor[0])
				neighbor[0].visited = True
				neighbor[0].backNode = curr
				neighbor[0].costSoFar = currDist + curr.costSoFar
				if (neighbor[0] == endNode):
					print("Found end node, Cost: ", curr.costSoFar)
					total = curr.costSoFar
					startNode.backNode = 0
					path.append(neighbor[0])
					while (curr != 0):
						path.append(curr)
						curr = curr.backNode
					return path, total
		else:
			if (curr.costSoFar + currDist < neighbor[0].costSoFar):
				neighbor[0].costSoFar = curr.costSoFar + currDist
				neighbor[0].backNode = curr
	return path

def drawPath(surf, pattern):
	i = 0
	while (i < len(pattern)-1):
		pygame.draw.line(surf, (255,0,0), pattern[i].position.VecToPygame(), pattern[i+1].position.VecToPygame(), 3)
		i+=1

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False

widthLim = 20
heightLim = 15
NodeList = [[]]
#Nodes[i][j] = (Node(Vector((800 / widthLim) * i - (800 / (2 * widthLim)), (600 / heightLim) * j - (600 / (2 * heightLim)))))

i = 1
while (i <= widthLim):
	j = heightLim
	while (j > 0):
		NodeList[i-1].append(Node(Vector((800 / widthLim) * i - (800 / (2 * widthLim)), (600 / heightLim) * j - (600 / (2 * heightLim))), i, j))
		j-=1
	i+= 1
	NodeList.append([])

#Nodes[0][0].col = (0,0,0)

leftX = (800 / widthLim)
rightX = (800 - (800 / (widthLim)))
topY = (600 / heightLim)
bottomY = (600 - (600 / heightLim))

timeStart = t.time()
i = 0
edgeWeight = 1
while (i < widthLim):
	j = 0
	while (j < heightLim):
		k = -1
		while (k <= 1):
			l = -1
			while (l <= 1):
				if ((k != 0 or l != 0) and i + k >= 0 and i + k < widthLim and j + l >= 0 and j + l < heightLim):
					r = random.randint(0, 5120)
					NodeList[i][j].neighbors.append([NodeList[i + k][j + l], r])
				l+=1
			k+=1
		j+=1
	i+= 1
print("Time taken: " + str(t.time() - timeStart))

timeStart = t.time()
path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[14][14])
print("Dijkstra's Computation: ", str(t.time() - timeStart))

for n in path:
	print(n.indX, ", ", n.indY ,", ",n.costSoFar)
i = 0
j = 0
stepVal = 0
clock = time.Clock()
while not done:
	clock.tick(10)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	
	#Update

	#Debug Coloring
	#Nodes[i][j].col = (255,0,0)
	#j+=1
	#if (j == heightLim):
	#    i += 1
	#    j = 0


	#Drawing
	screen.fill((100, 149, 237))
	for row in NodeList:
		for node in row:
			if (node.visited == False):
				node.col = (0, 255, 255)
			elif (totalCost >= node.costSoFar):
				node.col = ((node.indX/widthLim)*255.0, (node.indY/heightLim)*255.0, 255*(float(node.costSoFar/totalCost)))
			else:
				node.col = (0,0,0)
			node.draw(screen)

	drawPath(screen, path)
	pygame.display.flip()
