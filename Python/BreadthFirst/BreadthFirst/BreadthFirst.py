import pygame
import Nodes
from pygame import *
from Nodes import *

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
Nodes = []

i = 120
while (i > 0):
    Nodes.append(Node(Vector(random.uniform(25, 725), random.uniform(25, 525))))
    i-=1

for aNode in Nodes:
    for anotherNode in Nodes:
        if (aNode != anotherNode):
            if (anotherNode.position.distance(aNode.position) < 20):
                aNode.shiftNode(aNode.position - anotherNode.position, 120)
            if (anotherNode.position.distance(aNode.position) < 125 and aNode not in anotherNode.neighbors):
                aNode.neighbors.append(anotherNode)

rootNode = Nodes[random.randint(0, 120)]

clock = time.Clock()
while not done:
    clock.tick(144)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
    
    #Update 

    #Drawing
    screen.fill((100, 149, 237))
    for node in Nodes:
        node.draw(screen)
    rootNode.flareSelf(screen)
    pygame.display.flip()