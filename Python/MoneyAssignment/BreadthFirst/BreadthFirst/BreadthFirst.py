import time as t
import pygame
import Nodes
import math as m
import queue as q
import Agent

from pygame import *
from Nodes import *
from queue import *
from math import *
from Agent import *

pathColor = (255,255,255)
exploredColor = (255,0,255)
visitedColor = (255,255,0)
defaultColor = (0,255,255)

def ResetGraph(nodeList, retToVisit, startNode, endNode, retPath,coinList, regenMap):
	retToVisit = [startNode]
	retPath = []
	if (regenMap):
		coinList = []
	for nodeRow in nodeList:
		for node in nodeRow:
			if (node != 0):
				node.visited = False
				node.backNode = 0
				node.costSoFar = 3000000
				node.col = defaultColor
				if (regenMap):
					node.isCoin = False
					node.isObstacle = False
					if (random.uniform(0, 100) > obstacleChance):
						node.isObstacle = False
						#if (random.uniform(0, 100) > coinChance):
						#	node.isCoin = False
						#else:
						#	node.isCoin = True
						#	coinList.append(node)
					else:
						node.isObstacle = True
			
	startNode.isObstacle = False
	endNode.isObstacle = False
	startNode.costSoFar = 0
	startNode.col = (0, 255, 0)
	endNode.col = (255, 0, 0)
	curr = startNode
	curr.backNode = 0
	return retPath, retToVisit, coinList

def BreadthFirst(nodeList, startNode, endNode, toVisit, path, bool):
	while (len(toVisit) > 0):
		curr = toVisit.pop(0)
		curr.col = exploredColor
		for neighbor in curr.neighbors:
			if(neighbor[0].isObstacle == False and neighbor[0].visited == False):
				if (neighbor[0] not in toVisit):
					toVisit.append(neighbor[0])
				neighbor[0].visited = True
				neighbor[0].backNode = curr
				neighbor[0].col = visitedColor
				if (neighbor[0] == endNode):
					print("Found end node")
					startNode.backNode = 0
					neighbor[0].col = (0,0,0)
					path.append(neighbor[0])
					while (curr != 0):
						curr.col = pathColor
						path.append(curr)
						curr = curr.backNode
						bool = False
					return path, toVisit, bool
		#return path, toVisit, bool
	bool = False
	print("Unsolvable!")
	return path, toVisit, bool

def DijkstrasSearch(nodeList, startNode, endNode, toVisit, path, bool):
	while (len(toVisit) > 0):
		toVisit.sort(key=lambda neighbor: neighbor.costSoFar)
		curr = toVisit.pop(0)
		curr.col = exploredColor
		if (curr != 0):
			for neighbor in curr.neighbors:
				if (neighbor[0] != 0):
					currDist = neighbor[1]
					if(neighbor[0].isObstacle == False and neighbor[0].visited == False):
						toVisit.append(neighbor[0])
						neighbor[0].visited = True
						neighbor[0].backNode = curr
						neighbor[0].costSoFar = currDist + curr.costSoFar
						neighbor[0].col = visitedColor
						if (neighbor[0] == endNode):
							total = neighbor[0].costSoFar
							neighbor[0].col = pathColor
							startNode.backNode = 0
							path.append(neighbor[0])
							while (curr != 0):
								curr.col = pathColor
								path.append(curr)
								curr = curr.backNode
							bool = False
							return path, toVisit, 0, bool
					elif(neighbor[0].isObstacle == False):
						if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
							neighbor[0].costSoFar = curr.costSoFar + currDist
							neighbor[0].backNode = curr
		#return path, toVisit, 0, bool
	bool = False
	print("Unsolvable!")
	return path, toVisit, 0, bool

def AStarSearch(nodeList, startNode, endNode, toVisit, path, bool):
	while (len(toVisit) > 0):
		toVisit.sort(key=lambda neighbor: neighbor.position.distance(endNode.position) + neighbor.costSoFar)
		curr = toVisit.pop(0)
		curr.col = exploredColor
		if (curr != 0):
			for neighbor in curr.neighbors:
				if (neighbor[0] != 0):
					currDist = neighbor[0].position.distance(endNode.position) + curr.position.distance(neighbor[0].position)
					#currDist = curr.position.distance(endNode.position) +
					#neighbor[0].costSoFar
					if(neighbor[0].isObstacle == False and neighbor[0].visited == False):
						toVisit.append(neighbor[0])
						neighbor[0].visited = True
						neighbor[0].backNode = curr
						neighbor[0].costSoFar = neighbor[0].position.distance(curr.position) + curr.costSoFar
						neighbor[0].col = visitedColor
						if (neighbor[0] == endNode):
							total = neighbor[0].costSoFar
							neighbor[0].col = pathColor
							startNode.backNode = 0
							path.append(neighbor[0])
							while (curr != 0):
								curr.col = pathColor
								path.append(curr)
								curr = curr.backNode
							bool = False
							return path, toVisit, 0, bool
					elif (neighbor[0].isObstacle == False):
						if (neighbor[0].position.distance(curr.position) + curr.costSoFar <= neighbor[0].costSoFar):
							neighbor[0].costSoFar = neighbor[0].position.distance(curr.position) + curr.costSoFar
							neighbor[0].backNode = curr
		#return path, toVisit, 0, bool
	bool = False
	print("Unsolvable!")
	return path, toVisit, 0, bool

def BestFirst(nodeList, startNode, endNode, toVisit, path, bool):
	while (len(toVisit) > 0):
		toVisit.sort(key=lambda neighbor: neighbor.position.distance(endNode.position))
		curr = toVisit.pop(0)
		curr.col = exploredColor
		if (curr != 0):
			for neighbor in curr.neighbors:
				if (neighbor[0] != 0):
					currDist = neighbor[0].position.distance(endNode.position)
					if(neighbor[0].isObstacle == False and neighbor[0].visited == False):
						toVisit.append(neighbor[0])
						neighbor[0].visited = True
						neighbor[0].backNode = curr
						neighbor[0].costSoFar = currDist + curr.costSoFar + neighbor[1]
						neighbor[0].col = visitedColor
						if (neighbor[0] == endNode):
							total = neighbor[0].costSoFar
							neighbor[0].col = pathColor
							startNode.backNode = 0
							path.append(neighbor[0])
							while (curr != 0):
								curr.col = pathColor
								path.append(curr)
								curr = curr.backNode
							bool = False
							return path, toVisit, 0, bool
					elif(neighbor[0].isObstacle == False):
						if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
							neighbor[0].costSoFar = curr.costSoFar + currDist
							neighbor[0].backNode = curr
		#return path, toVisit, 0, bool
	bool = False
	print("Unsolvable!")
	return path, toVisit, 0, bool

def drawPath(surf, pattern):
	i = 0
	while (i < len(pattern) - 1):
		pygame.draw.line(surf, (255,255,255), pattern[i].position.VecToPygame(), pattern[i + 1].position.VecToPygame(), 3)
		i+=1

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False

widthLim = 32
heightLim = 24
NodeList = [[]]
CoinList = []
obstacleChance = 30
coinChance = 10

i = 1
while (i <= widthLim):
	j = heightLim
	while (j > 0):
		if (False):
			NodeList[i - 1].append(0)
		else:
			NodeList[i - 1].append(Node(Vector((800 / widthLim) * i - (800 / (2 * widthLim)), (600 / heightLim) * j - (600 / (2 * heightLim))), i, j))
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
					#if (i+k == 0 or j+l == 0 or i+k == widthLim-1 or j+l == heightLim-1):
					#	edgeWeight = 1
					#else:
					if (NodeList[i][j] != 0):
						edgeWeight = m.sqrt(k ** 2 + l ** 2) * 25
						NodeList[i][j].neighbors.append([NodeList[i + k][j + l], edgeWeight])
				l+=1
			k+=1
		j+=1
	i+= 1
print("Time taken: " + str(t.time() - timeStart))

x = 25
y = 15
startX = 0
startY = 0
totalCost = 0
timeSinceCoin = 99

toVisit = []
path = []

runBreadth = False
runDijkstra = False
runAStar = False
runBest = False
drawNeighborLines = False
regenMap = False
lastSearch = BreadthFirst
agent = Agent(NodeList[startX][startY].position, screen)
NodeList[startX][startY].isOccupied = True

path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[0][0], NodeList[x][y], path, CoinList, True)

i = 0
j = 0
stepVal = 0
clock = pygame.time.Clock()
while not done:
	clock.tick(60)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
		if event.type == pygame.KEYDOWN:
			if (event.key == pygame.K_n):
				drawNeighborLines = not drawNeighborLines

			if (event.key == pygame.K_b):
				print("Calling Breadth")
				timeStart = t.time()
				#startX = random.randint(0, 31)
				#x = random.randint(0, 31)
				#startY = random.randint(0, 23)
				#y = random.randint(0, 23)
				path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
				tempCoinList = CoinList.copy()
				while (len(path) == 0 and len(tempCoinList) > 0):
					path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
					path, toVisit, runBreadth = BreadthFirst(NodeList, NodeList[startX][startY], NodeList[x][y], toVisit, path, runBreadth)
					NodeList[startX][startY].col = (0, 255, 0)
					NodeList[x][y].col = (255, 0, 0)
					lastSearch = BreadthFirst
					runBreadth = True
					runDijkstra = False
					runAStar = False
					runBest = False
					if (len(path) == 0 and len(tempCoinList) > 1):
						tempCoinList.pop(0)
						NodeList[x][y].col = (0,0,0)
						x = m.floor(tempCoinList[0].position.x/ 25)
						y = 24 - m.floor((tempCoinList[0].position.y + 25) / 25)
					else:
						print("Ran out of things to check.")
						break;
				agent.setPath(path, NodeList[x][y])
				print("Algorithm Computation: ", str(t.time() - timeStart))

			if (event.key == pygame.K_d):
				print("Calling Dijkstras")
				timeStart = t.time()
				#startX = random.randint(0, 31)
				#x = random.randint(0, 31)
				#startY = random.randint(0, 23)
				#y = random.randint(0, 23)
				path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
				tempCoinList = CoinList.copy()
				while (len(path) == 0 and len(tempCoinList) > 0):
					path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
					path, toVisit, totalCost, runDijkstra = DijkstrasSearch(NodeList, NodeList[startX][startY], NodeList[x][y], toVisit, path, runDijkstra)
					NodeList[startX][startY].col = (0, 255, 0)
					NodeList[x][y].col = (255, 0, 0)
					lastSearch = DijkstrasSearch
					runDijkstra = True
					runBreadth = False
					runAStar = False
					runBest = False
					if (len(path) == 0 and len(tempCoinList) > 1):
						tempCoinList.pop(0)
						NodeList[x][y].col = (0,0,0)
						x = m.floor(tempCoinList[0].position.x/ 25)
						y = 24 - m.floor((tempCoinList[0].position.y + 25) / 25)
					else:
						print("Ran out of things to check.")
						break;
				agent.setPath(path, NodeList[x][y])
				print("Total Cost: ", totalCost, ", Length: ", len(path))
				print("Algorithm Computation: ", str(t.time() - timeStart))

			if (event.key == pygame.K_a):
				print("Calling A*")
				timeStart = t.time()
				path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
				tempCoinList = CoinList.copy()
				while (len(path) == 0 and len(tempCoinList) > 0):
					path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
					path, toVisit, totalCost, runAStar = AStarSearch(NodeList, NodeList[startX][startY], NodeList[x][y], toVisit, path, runAStar)
					NodeList[startX][startY].col = (0, 255, 0)
					NodeList[x][y].col = (255, 0, 0)
					lastSearch = AStarSearch
					runAStar = True
					runBreadth = False
					runDijkstra = False
					runBest = False
					if (len(path) == 0 and len(tempCoinList) > 1):
						tempCoinList.pop(0)
						NodeList[x][y].col = (0,0,0)
						x = m.floor(tempCoinList[0].position.x/ 25)
						y = 24 - m.floor((tempCoinList[0].position.y + 25) / 25)
					else:
						print("Ran out of things to check.")
						break;
				agent.setPath(path, NodeList[x][y])
				print("Algorithm Computation: ", str(t.time() - timeStart))

			if (event.key == pygame.K_s):
				print("Calling Best")
				timeStart = t.time()
				path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
				tempCoinList = CoinList.copy()
				while (len(path) == 0 and len(tempCoinList) > 0):
					path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
					path, toVisit, totalCost, runBest = BestFirst(NodeList, NodeList[startX][startY], NodeList[x][y], toVisit, path, runBest)
					NodeList[startX][startY].col = (0, 255, 0)
					NodeList[x][y].col = (255, 0, 0)
					lastSearch = BestFirst
					runDijkstra = False
					runBreadth = False
					runAStar = False
					runBest = True
					if (len(path) == 0 and len(tempCoinList) > 1):
						tempCoinList.pop(0)
						NodeList[x][y].col = (0,0,0)
						x = m.floor(tempCoinList[0].position.x/ 25)
						y = 24 - m.floor((tempCoinList[0].position.y + 25) / 25)
					else:
						print("Ran out of things to check.")
						break;
				agent.setPath(path, NodeList[x][y])
				print("Best-First Computation.")

		if (event.type == pygame.MOUSEBUTTONDOWN):
			l,middle,r = pygame.mouse.get_pressed()
			clickedNode = NodeList[m.floor(pygame.mouse.get_pos()[0] / 25)][24 - m.floor((pygame.mouse.get_pos()[1] + 25) / 25)]
			if (l == 1):
				clickedNode.isObstacle = not clickedNode.isObstacle
				if (not clickedNode.isObstacle):
					for neighbor in clickedNode.neighbors:
						if (neighbor[0].visited and clickedNode not in toVisit):
							toVisit.append(clickedNode)
							break
				clickedNode.isCoin = False
				if (clickedNode in CoinList):
					CoinList.remove(clickedNode)
			elif (r == 1):
				clickedNode.isCoin = not clickedNode.isCoin
				if(clickedNode.isCoin):
					CoinList.append(clickedNode)
				elif (clickedNode in CoinList):
					CoinList.remove(clickedNode)
				clickedNode.isObstacle = False
				if (not clickedNode.isObstacle):
					for neighbor in clickedNode.neighbors:
						if (neighbor[0].visited and clickedNode not in toVisit):
							toVisit.append(clickedNode)
							break

	timeSinceCoin += clock.get_time()/1000
	if (timeSinceCoin > 1):
		timeSinceCoin = 0
		randX = random.randint(0, widthLim-1)
		randY = random.randint(0, heightLim-1)
		timeOut = 0
		while (NodeList[randX][randY].isObstacle or NodeList[randX][randY].isCoin or timeOut > widthLim * heightLim /1.25):
			randX = random.randint(0, widthLim-1)
			randY = random.randint(0, heightLim-1)
			timeOut += 1
		NodeList[randX][randY].isCoin = True
		CoinList.append(NodeList[randX][randY])

	#Updating Agent
	recalc = agent.update(NodeList, CoinList)
	startX = m.floor(agent.position.x/ 25)
	startY = 24 - m.floor((agent.position.y + 25) / 25)
	if (recalc):
		path, toVisit, CoinList = ResetGraph(NodeList, toVisit, NodeList[startX][startY], NodeList[x][y], path, CoinList, regenMap)
		path, toVisit, totalCost, runBest = lastSearch(NodeList, NodeList[startX][startY], NodeList[x][y], toVisit, path, runAStar)
		agent.setPath(path, NodeList[x][y])
	for row in NodeList:
		for node in row:
			node.update(clock.get_time()/1000)

	#Drawing
	screen.fill((0,200,255))
	for node in path:
		node.col = pathColor
	for row in NodeList:
		for node in row:
			if (node != 0):
				node.draw(screen, drawNeighborLines)
	for row in NodeList:
		for node in row:
			if (node != 0):
				node.drawObstacle(screen)

	if(len(CoinList) > 0 and agent.currentCoin == 0):
		CoinList.sort(key=lambda coin: coin.position.distance(NodeList[startX][startY].position))
		pygame.draw.line(screen, (255,0,0), (NodeList[startX][startY].position.x, NodeList[startX][startY].position.y), (CoinList[0].position.x, CoinList[0].position.y))
		x = m.floor(CoinList[0].position.x/ 25)
		y = 24 - m.floor((CoinList[0].position.y + 25) / 25)

	agent.draw()
	pygame.display.flip()
