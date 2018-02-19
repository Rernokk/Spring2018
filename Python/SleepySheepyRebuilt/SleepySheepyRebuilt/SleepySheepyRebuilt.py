import pygame
import Sheepy
from pygame import *
from Sheepy import *
pygame.init()
screen = pygame.display.set_mode((800,600))

#Initializers
Sheep = Sheepy(screen,(400,300), (1,0))

#Game Loop
done = False
clock = time.Clock()
while (not done):
	clock.tick(144)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	Sheep.update()
	screen.fill((100,149,237))
	Sheep.draw()
	pygame.display.flip()