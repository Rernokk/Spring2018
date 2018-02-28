import time as t
import pygame
import Nodes
import queue as q
from pygame import *
from Nodes import *
from queue import *

def ResetGraph(nodeList, retToVisit, startNode, endNode, retPath):
	retToVisit = [startNode]
	retPath = []
	for nodeRow in nodeList:
		for node in nodeRow:
			if (node != 0):
				node.visited = False
				node.backNode = 0
				node.costSoFar = 3000000
				node.col = (255,0,0)
			
	startNode.costSoFar = 0
	curr = startNode
	curr.backNode = 0
	return retPath, retToVisit

def BreadthFirst(nodeList, startNode, endNode, toVisit, path, bool):
	curr = toVisit.pop(0)
	for neighbor in curr.neighbors:
		if(neighbor[0].visited==False):
			if (neighbor[0] not in toVisit):
				toVisit.append(neighbor[0])
			neighbor[0].visited = True
			neighbor[0].backNode = curr
			neighbor[0].col = (0,0,0)
			if (neighbor[0] == endNode):
				print("Found end node")
				startNode.backNode = 0
				path.append(neighbor[0])
				while (curr != 0):
					path.append(curr)
					curr = curr.backNode
					bool = False
	return path, toVisit, bool

def DijkstrasSearch(nodeList, startNode, endNode, toVisit, path, bool):
	curr = toVisit.pop(0)
	if (curr != 0):
		for neighbor in curr.neighbors:
			if (neighbor[0] != 0):
				currDist = neighbor[1]
				if(neighbor[0].visited == False):
					toVisit.append(neighbor[0])
					neighbor[0].visited = True
					neighbor[0].backNode = curr
					neighbor[0].costSoFar = currDist + curr.costSoFar
					neighbor[0].col = (0,0,0)
					if (neighbor[0] == endNode):
						total = neighbor[0].costSoFar
						startNode.backNode = 0
						path.append(neighbor[0])
						while (curr != 0):
							path.append(curr)
							curr = curr.backNode
						bool = False
				else:
					if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
						neighbor[0].costSoFar = curr.costSoFar + currDist
						neighbor[0].backNode = curr
	return path, toVisit, 0, bool

def AStarSearch(nodeList, startNode, endNode, toVisit, path, bool):
	curr = toVisit.pop(0)
	if (curr != 0):
		for neighbor in curr.neighbors:
			if (neighbor[0] != 0):
				currDist = neighbor[0].position.distance(endNode.position)
				if(neighbor[0].visited == False):
					toVisit.append(neighbor[0])
					neighbor[0].visited = True
					neighbor[0].backNode = curr
					neighbor[0].costSoFar = currDist + curr.costSoFar
					neighbor[0].col = (0,0,0)
					if (neighbor[0] == endNode):
						total = neighbor[0].costSoFar
						startNode.backNode = 0
						path.append(neighbor[0])
						while (curr != 0):
							path.append(curr)
							curr = curr.backNode
						bool = False
				else:
					if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
						neighbor[0].costSoFar = curr.costSoFar + currDist
						neighbor[0].backNode = curr
	return path, toVisit, 0, bool

def drawPath(surf, pattern):
	i = 0
	while (i < len(pattern)-1):
		pygame.draw.line(surf, (255,255,255), pattern[i].position.VecToPygame(), pattern[i+1].position.VecToPygame(), 3)
		i+=1

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False

widthLim = 32
heightLim = 24
NodeList = [[]]

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
						edgeWeight = math.sqrt(k**2 + l**2)
						NodeList[i][j].neighbors.append([NodeList[i + k][j + l], edgeWeight])
				l+=1
			k+=1
		j+=1
	i+= 1
print("Time taken: " + str(t.time() - timeStart))

x = 31
y = 23
timeStart = t.time()
totalCost = 0
toVisit = []
path = []
path, toVisit = ResetGraph(NodeList, toVisit, NodeList[0][0], NodeList[x][y], path)
print("Algorithm Computation: ", str(t.time() - timeStart))
runBreadth = False
runDijkstra = False
runAStar = False

i = 0
j = 0
stepVal = 0
clock = time.Clock()
while not done:
	clock.tick(144)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
		if event.type == pygame.KEYDOWN:
			if (event.key == pygame.K_DOWN):
				if (y > 0):
					y -=1
			if (event.key == pygame.K_UP):
				if (y < heightLim-1):
					y +=1
			if (event.key == pygame.K_LEFT):
				if (x > 0):
					x -=1
			if (event.key == pygame.K_RIGHT):
				if (x < widthLim-1):
					x +=1

			if (event.key == pygame.K_b):
				print("Calling Breadth")
				path, toVisit = ResetGraph(NodeList, toVisit, NodeList[0][0], NodeList[x][y], path)
				path, toVisit, runBreadth = BreadthFirst(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runBreadth)
				runBreadth = True

			if (event.key == pygame.K_d):
				print("Calling Dijkstras")
				path, toVisit = ResetGraph(NodeList, toVisit, NodeList[0][0], NodeList[x][y], path)
				path, toVisit, totalCost, runDijkstra = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runDijkstra)
				runDijkstra = True
				print("Total Cost: ", totalCost, ", Length: ", len(path))

			if (event.key == pygame.K_a):
				print("Calling A*")
				path, toVisit = ResetGraph(NodeList, toVisit, NodeList[0][0], NodeList[x][y], path)
				path, toVisit, totalCost, runAStar = AStarSearch(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runAStar)
				runAStar = True

			if (event.key == pygame.K_s):
				print("Not yet implemented Best-First Pathfinding.")

	
	#Update

	#Debug Coloring
	#Nodes[i][j].col = (255,0,0)
	#j+=1
	#if (j == heightLim):
	#    i += 1
	#    j = 0

	if (runBreadth):
		path, toVisit, runBreadth = BreadthFirst(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runBreadth)

	if (runDijkstra):
		path, toVisit, totalCost, runDijkstra = DijkstrasSearch(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runDijkstra)

	if (runAStar):
		path, toVisit, totalCost, runAStar = AStarSearch(NodeList, NodeList[0][0], NodeList[x][y], toVisit, path, runAStar)

	#Drawing
	screen.fill((0,200,255))
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
