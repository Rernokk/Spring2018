import pygame
import Sheepy
import random as r
from pygame import *
from Sheepy import *
from random import *

pygame.init()
screen = pygame.display.set_mode((800,600))

#Initializers
#Sheep = [Sheepy(screen,(400,300), (-1,0))]
Sheep = []
indexer = 1
while (indexer <= 15):
	tempSheep = Sheepy(screen, (400 + r.uniform(-400, 400), 300 + r.uniform(-250,250)), (0, r.uniform(0,1)))
	newX = tempSheep.position[0]
	newY = tempSheep.position[1]
	while (newX > 180 and newX < 620):
		newX = 400+r.uniform(-400, 400)
		while (newY < 130 and newY > 470):
			newY = 300 + r.uniform(-300, 300)
	tempSheep.position = (newX, newY)
	Sheep.append(tempSheep)
	indexer += 1

#Pen
Pen = []
i = -20
while (i <= 20):
	Pen.append([pygame.Surface((10,10)), (400+10*i, 150)])
	if (i <=-5 or i > 5):
		Pen.append([pygame.Surface((10,10)), (400+10*i, 450)])
	i += 1
i = 0
while (i < 30):
	Pen.append([pygame.Surface((10,10)), (200, 150 + 10*i)])
	Pen.append([pygame.Surface((10,10)), (600, 150 + 10*i)])
	i+=1

#Game Loop
done = False
clock = time.Clock()
while (not done):
	clock.tick(60)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	screen.fill((100,149,237))
	for sheep in Sheep:
		sheep.update(clock.get_time() * .001, Sheep, Pen)
	for sheep in Sheep:
		sheep.draw()
	for wall in Pen:
		screen.blit(wall[0], wall[1])
	pygame.display.flip()