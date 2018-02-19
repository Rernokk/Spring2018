import time as t
import pygame
import Nodes
import queue as q
from pygame import *
from Nodes import *
from queue import *

def BreadthFirst(nodeList, startNode, endNode, path, toVisit, pause):
    #path = []
    #toVisit = [startNode]
    for nodeRow in Nodes:
        for node in nodeRow:
            node.visited = False
            node.backPath = 0

    while (len(toVisit) > 0):
        if (pause):
            print(len(toVisit))
            return path
        curr = toVisit.pop(0)
        curr.visited = True
        path.append(curr)
        if (len(toVisit) == 0):
            print("Finished")
        for neighbor in curr.neighbors:
            if(not neighbor.visited):
                if (neighbor not in toVisit):
                    toVisit.append(neighbor)
                neighbor.visited = True
                neighbor.backNode = curr
                if (neighbor == endNode):
                    path.append(neighbor)
                    return path
        pause = True
    return path

def drawPath(surf, pattern, stepVal):
    i = 0
    while (i < stepVal-1):
        pygame.draw.line(surf, (255,0,0), pattern[i].position.VecToPygame(), pattern[i+1].position.VecToPygame(), 3)
        i+=1

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False

widthLim = 20
heightLim = 15
Nodes = [[]]
#Nodes[i][j] = (Node(Vector((800 / widthLim) * i - (800 / (2 * widthLim)), (600 / heightLim) * j - (600 / (2 * heightLim)))))

i = 1
while (i <= widthLim):
    j = heightLim
    while (j > 0):
        Nodes[i-1].append(Node(Vector((800 / widthLim) * i - (800 / (2 * widthLim)), (600 / heightLim) * j - (600 / (2 * heightLim))), i, j))
        j-=1
    i+= 1
    Nodes.append([])

#Nodes[0][0].col = (0,0,0)

leftX = (800 / widthLim)
rightX = (800 - (800 / (widthLim)))
topY = (600 / heightLim)
bottomY = (600 - (600 / heightLim))

timeStart = t.time()
i = 0
while (i < widthLim):
    j = 0
    while (j < heightLim):
        k = -1
        while (k <= 1):
            l = -1
            while (l <= 1):
                if ((k != 0 or l != 0) and i + k >= 0 and i + k < widthLim and j + l >= 0 and j + l < heightLim):
                    Nodes[i][j].neighbors.append(Nodes[i + k][j + l])
                l+=1
            k+=1
        j+=1
    i+= 1
print("Time taken: " + str(t.time() - timeStart))

#path = BreadthFirst(Nodes, Nodes[2][2], Nodes[0][7])
#print(len(path))
i = 0
j = 0
stepVal = 0
path = []
toVisit = [Nodes[10][9]]
clock = time.Clock()
while not done:
    clock.tick(144)
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
    for row in Nodes:
        for node in row:
            if (node.visited == True):
                node.col = (255, 0,0)
            node.draw(screen)

    pause = False
    path = BreadthFirst(Nodes, Nodes[10][9], Nodes[0][7], path, toVisit, pause)

    drawPath(screen, path, stepVal)
    stepVal += 1
    if (stepVal > len(path)):
        stepVal = len(path)
    #rootNode.flareSelf(screen)
    pygame.display.flip()
