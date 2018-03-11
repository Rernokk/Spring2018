import pygame
import math as m
from pygame import *
from math import *

class Agent (object):
	def __init__(self, position, screen):
		self.position = position
		self.screen = screen
		self.path = []

	def setPath(self, path):
		self.path = path
		self.path.reverse()

	def draw(self):
		pygame.draw.circle(self.screen, (200,200,200), (int(self.position.x), int(self.position.y)), 5)
		pygame.draw.circle(self.screen, (0,220,0), (int(self.position.x), int(self.position.y)), 5, 3)

	def update(self, nodeList, coinList):
		if (len(self.path) > 0):
			if (self.path[0].position.distance(self.position) < 2):
				self.position = self.path.pop(0).position
			else:
				self.position += (self.path[0].position - self.position).normalized() * 2
		myGridX = m.floor(self.position.x/ 25)
		myGridY = 24 - m.floor((self.position.y + 25) / 25)
		if (nodeList[myGridX][myGridY].isCoin):
			nodeList[myGridX][myGridY].isCoin = False
			coinList.remove(nodeList[myGridX][myGridY])
