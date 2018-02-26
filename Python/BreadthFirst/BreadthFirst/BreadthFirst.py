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
			if (node != 0):
				node.visited = False
				node.backNode = 0
				node.costSoFar = 3000000
				node.col = (255,255,255)
			
	startNode.costSoFar = 0
	curr = startNode
	curr.backNode = 0

	while (len(toVisit) > 0):
		curr = toVisit.pop(0)
		if (curr != 0):
			for neighbor in curr.neighbors:
				if (neighbor[0] != 0):
					currDist = neighbor[1]#curr.position.distance(neighbor[0].position)
					if(neighbor[0].visited == False):
						#if (neighbor[0] not in toVisit):
						toVisit.append(neighbor[0])
						neighbor[0].visited = True
						neighbor[0].backNode = curr
						neighbor[0].costSoFar = currDist + curr.costSoFar
						if (neighbor[0] == endNode):
							total = neighbor[0].costSoFar
							startNode.backNode = 0
							path.append(neighbor[0])
							while (curr != 0):
								path.append(curr)
								curr = curr.backNode
							startNode.col = (255,0,0)
							endNode.col = (0,0,0)
							return path, total
					else:
						#print("Found Visited, Current: ",curr.costSoFar + currDist, ", neighbor: ", neighbor[0].costSoFar)
						if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
							neighbor[0].costSoFar = curr.costSoFar + currDist
							neighbor[0].backNode = curr
	return path, 0

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
		if (False):
			NodeList[i-1].append(0)
		else:
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
edgeWeight = random.uniform(0, 1000)
while (i < widthLim):
	j = 0
	while (j < heightLim):
		k = -1
		while (k <= 1):
			l = -1
			while (l <= 1):
				if ((k != 0 or l != 0) and i + k >= 0 and i + k < widthLim and j + l >= 0 and j + l < heightLim):
					#if (i+k == 0 or j+l == 0 or i+k == widthLim-1 or j+l  == heightLim-1):
					#	edgeWeight = 1
					#else:
					if (NodeList[i][j] != 0):
						edgeWeight = random.uniform(1, 256)
						NodeList[i][j].neighbors.append([NodeList[i + k][j + l], edgeWeight])
				l+=1
			k+=1
		j+=1
	i+= 1
print("Time taken: " + str(t.time() - timeStart))

x = 15
y = 14
timeStart = t.time()
path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y])
print("Dijkstra's Computation: ", str(t.time() - timeStart))
print("Found end node, Cost: ", totalCost)

for n in path:
	print(n.indX, ", ", n.indY ,", ", n.costSoFar)

i = 0
j = 0
stepVal = 0
clock = time.Clock()
while not done:
	clock.tick(10)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
		if event.type == pygame.KEYDOWN:
			if (event.key == pygame.K_s):
				if (y > 0):
					y -=1
					path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y])
			if (event.key == pygame.K_w):
				if (y < heightLim-1):
					y +=1
					path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y])
			if (event.key == pygame.K_a):
				if (x > 0):
					x -=1
					path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y])
			if (event.key == pygame.K_d):
				if (x < widthLim-1):
					x +=1
					path,totalCost = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y])
	
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
			#if (node.visited == False):
			#	node.col = (255, 255, 0)
			#elif (totalCost >= node.costSoFar):
			#	node.col = ((node.indX/widthLim)*255.0, (node.indY/heightLim)*255.0, 255*(float(node.costSoFar/totalCost)))
			#else:
			#	node.col = (0,0,0)
			if (node != 0):
				node.draw(screen)

	drawPath(screen, path)
	pygame.display.flip()
