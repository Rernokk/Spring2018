import pygame
from pygame import *

class Sheepy(object):
	"""description of class"""
	def __init__(self, screen, pos, vel):
		self.name = ""
		spr = pygame.image.load("Circle.png")
		self.original_Sprite = spr.convert_alpha()
		self.position = pos
		self.screen = screen
		self.orientation = 0
		self.velocity = vel
		self.maxLinVel = 30

	def draw(self):
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())
		self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
		x,y = self.drawRect.center
		self.drawRect = self.sprite.get_rect()
		self.drawRect.center = (x,y)
		self.screen.blit(self.sprite, (self.position[0], self.position[1]))
		pygame.draw.line(self.screen, (255,0,0), self.drawRect.center, (self.drawRect.center[0] + self.velocity[0] * self.maxLinVel, self.drawRect.center[1] +  self.velocity[1] * self.maxLinVel), 2)

	def update(self):
		self.orientation += 1

	def rotateVelocity(self):
		valX = self.x
		valY = self.y
		valAng = math.radians(angle)
		self.x = valX * math.cos(valAng) - valY * math.sin(valAng)
		self.y = valX * math.sin(valAng) + valY * math.cos(valAng)