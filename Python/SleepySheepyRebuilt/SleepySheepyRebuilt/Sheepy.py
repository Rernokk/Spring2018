import pygame
import random as r
import math as m
import QuickMaths
from pygame import *
from random import *
from math import *
from QuickMaths import *

class Sheepy(object):
	"""description of class"""
	CONST_SHEEP_LENGTH = 20
	CONST_SHEEP_SPRING = .9
	CONST_SHEEP_MIN = 2 * CONST_SHEEP_LENGTH
	CONST_SHEEP_MAX = 5 * CONST_SHEEP_LENGTH
	CONST_SHEEP_NEIGHBOR_RANGE = 50
	CONST_WALL_RANGE = 30
	CONST_WALL_WEIGHT = 10
	CONST_PEN_RANGE = 60

	def __init__(self, screen, pos, vel):
		self.name = ""
		spr = pygame.image.load("Circle.png")
		self.original_Sprite = spr.convert_alpha()
		self.position = pos
		self.screen = screen
		self.orientation = r.uniform(-180, 180)
		self.velocity = QuickMaths.getNormalized((r.uniform(-1, 1), r.uniform(-1,1)))
		self.initVel = (self.velocity[0], self.velocity[1])
		self.maxLinVel = 20
		self.angVelocity = 3

	def draw(self):
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())
		self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
		x,y = self.drawRect.center
		self.drawRect = self.sprite.get_rect()
		self.drawRect.center = (x,y)
		self.screen.blit(self.sprite, (self.position[0], self.position[1]))
		pygame.draw.line(self.screen, (255,0,0), self.drawRect.center, (self.drawRect.center[0] + self.velocity[0] * self.maxLinVel, self.drawRect.center[1] +  self.velocity[1] * self.maxLinVel), 2)
		if (self.drawRect.center[0] < 0 or self.drawRect.center[0] > 800 or self.drawRect.center[1] < 0 or self.drawRect.center[1] > 600):
			pygame.draw.line(self.screen, (255,255,255), self.drawRect.center, (400, 300))

	def update(self, step, sheepList, penList):
		align = self.computeAlignment(step, sheepList)
		coheasion = self.computeCohesion(step, sheepList)
		separation = self.computeSeparation(step,sheepList, penList)
		rot = QuickMaths.getNormalized((align[0] + coheasion[0] + separation[0] * 1.1, align[1] + coheasion[1] + separation[1] * 1.1))
		if (rot == (0,0)):
			rot = self.velocity
		deg = math.degrees(math.atan2(self.velocity[0], self.velocity[1]) - math.atan2(rot[0], rot[1]))
		if (math.fabs(deg) > self.angVelocity):
			deg = math.copysign(self.angVelocity, deg)
			self.rotateVelocity(deg)
		else:
			self.velocity = rot
		self.velocity = QuickMaths.getNormalized(self.velocity)
		self.position = QuickMaths.addTuples(self.position, QuickMaths.getScaled(self.velocity, self.maxLinVel * step))

	def rotateVelocity(self, angle):
		valX = self.velocity[0]
		valY = self.velocity[1]
		valAng = m.radians(angle)
		self.velocity = (valX * m.cos(valAng) - valY * m.sin(valAng), valX * m.sin(valAng) + valY * m.cos(valAng))

	def getDistToSheep(self, sheep):
		return m.sqrt((self.position[0] - sheep.position[0])**2 + (self.position[1] - sheep.position[1])**2)

	def computeAlignment(self, step, sheepList):
		closeSheep = []
		netAlignment = (0,0)
		for sheep in sheepList:
			if (sheep != self and self.getDistToSheep(sheep) < Sheepy.CONST_SHEEP_NEIGHBOR_RANGE):
				netAlignment = QuickMaths.addTuples(netAlignment, sheep.velocity)
				closeSheep.append(sheep)
		if (len(closeSheep) == 0):
			return (0,0)
		else:
			return QuickMaths.getNormalized(QuickMaths.getScaled(netAlignment,1/len(closeSheep)))

	def computeCohesion(self, step, sheepList):
		closeSheep = []
		netMass = (0,0)
		for sheep in sheepList:
			if (sheep != self and self.getDistToSheep(sheep) < Sheepy.CONST_SHEEP_NEIGHBOR_RANGE*1.5):
				netMass = QuickMaths.addTuples(netMass, sheep.position)
				closeSheep.append(sheep)
		if (len(closeSheep) == 0):
			return netMass
		else:
			v = QuickMaths.getScaled(netMass, 1/len(closeSheep))
			v = QuickMaths.subTuples(v, self.position)
			return QuickMaths.getNormalized(v)

	def computeSeparation(self, step, sheepList, penList):
		closeSheep = []
		netSeparation = (0,0)
		for sheep in sheepList:
			if (sheep != self and self.getDistToSheep(sheep) < Sheepy.CONST_SHEEP_NEIGHBOR_RANGE*.75):
				netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples(sheep.position, self.position)))
				closeSheep.append(sheep)
		
		count = len(closeSheep)
		for wall in penList:
			if (QuickMaths.getMagnitude(self.position, wall[1]) < Sheepy.CONST_PEN_RANGE):
				pygame.draw.line(self.screen,(255,0,0), self.position, wall[1])
				netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples(wall[1], self.position)) * (Sheepy.CONST_WALL_WEIGHT * 3))
				count+=1
		if (QuickMaths.getMagnitude(self.position, (self.position[0], 0)) < Sheepy.CONST_WALL_RANGE):
			pygame.draw.line(self.screen,(255,0,0), self.position, (self.position[0], 0))
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples((self.position[0], 0), self.position)) * Sheepy.CONST_WALL_WEIGHT*10)
			count+=1
		if (QuickMaths.getMagnitude(self.position, (self.position[0], 600)) < Sheepy.CONST_WALL_RANGE):
			pygame.draw.line(self.screen,(255,0,0), self.position, ((self.position[0], 600)))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples((self.position[0], 600), self.position)) * Sheepy.CONST_WALL_WEIGHT*10)
		if (QuickMaths.getMagnitude(self.position, (0, self.position[1])) < Sheepy.CONST_WALL_RANGE):
			pygame.draw.line(self.screen,(255,0,0), self.position, (0, self.position[1]))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples((0, self.position[1]),self.position)) * Sheepy.CONST_WALL_WEIGHT*10)
		if (QuickMaths.getMagnitude(self.position, (800, self.position[1])) < Sheepy.CONST_WALL_RANGE):
			pygame.draw.line(self.screen,(255,0,0), self.position, (800, self.position[1]))
			pygame.draw.line(self.screen,(255,0,0), self.position, (self.position[0], 0))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getNormalized(QuickMaths.subTuples((800, self.position[1]), self.position)) * Sheepy.CONST_WALL_WEIGHT*10)

		if (count == 0):
			return (0,0)
		else:
			return QuickMaths.getNormalized(QuickMaths.getScaled(QuickMaths.getScaled(netSeparation, 1/count), -1))