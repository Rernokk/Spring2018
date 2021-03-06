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
	CONST_WALL_RANGE = 40
	CONST_WALL_WEIGHT = 20
	CONST_PEN_RANGE = 30

	def __init__(self, screen, pos, vel):
		self.name = ""
		spr = pygame.image.load("Circle.png")
		self.original_Sprite = spr.convert_alpha()
		self.position = pos
		self.screen = screen
		self.orientation = r.uniform(-180, 180)
		self.velocity = QuickMaths.getNormalized((r.uniform(-1, 1), r.uniform(-1,1)))
		self.initVel = (self.velocity[0], self.velocity[1])
		self.maxLinVel = 65
		self.currVel = self.maxLinVel/4
		self.angVelocity = 15
		self.isInPen = False
		self.state = "Wander"
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())

	def draw(self):
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())
		self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
		x,y = self.drawRect.center
		self.drawRect = self.sprite.get_rect()
		self.drawRect.center = (x,y)
		self.screen.blit(self.sprite, (self.position[0], self.position[1]))
		pygame.draw.line(self.screen, (255,0,0), self.drawRect.center, (self.drawRect.center[0] + self.velocity[0] * self.currVel, self.drawRect.center[1] +  self.velocity[1] * self.currVel), 2)
		if (self.isInPen):
			pygame.draw.circle(self.screen, (255,0,0), self.drawRect.center, 10)

	def update(self, step, sheepList, penList, dogRef):
		if (QuickMaths.getMagnitude(self.dogRef.position, self.position) < Sheepy.CONST_SHEEP_LENGTH * 5 and self.isInPen == False):
			self.currVel += (self.maxLinVel/3)* step
			self.state = "Flee"
			if (self.currVel > self.maxLinVel):
				self.currVel = self.maxLinVel
		elif (QuickMaths.getMagnitude(self.dogRef.position, self.position) < Sheepy.CONST_SHEEP_LENGTH * 10 and self.isInPen == False):
			self.currVel += (self.maxLinVel/3) * step
			self.state = "Flee"
			if (self.currVel > self.maxLinVel/2):
				self.currVel = self.maxLinVel/2
		else:
			self.currVel -= (self.maxLinVel/3)* step
			self.state = "Wander"
			if (self.currVel < self.maxLinVel/4):
				self.currVel = self.maxLinVel/4
		
		#if (self.state == "Flee"):
		#	self.flee(dogRef, step)

		align = self.computeAlignment(step, sheepList)
		coheasion = self.computeCohesion(step, sheepList)
		separation = self.computeSeparation(step,sheepList, penList, dogRef)
		rot = QuickMaths.getNormalized((align[0] + coheasion[0] + separation[0]*20, align[1] + coheasion[1] + separation[1]*20))
		if (rot == (0,0)):
			rot = self.velocity
		deg = math.degrees(math.atan2(self.velocity[0], self.velocity[1]) - math.atan2(rot[0], rot[1]))
		if (math.fabs(deg) > self.angVelocity):
			deg = math.copysign(self.angVelocity, deg)
			self.rotateVelocity(deg)
		else:
			self.velocity = rot
		self.velocity = QuickMaths.getNormalized(self.velocity)
		self.position = QuickMaths.addTuples(self.position, QuickMaths.getScaled(self.velocity, self.currVel * step))
		
		if (self.position[0] < 600 and self.position[0] > 200 and self.position[1] < 450 and self.position[1] > 150):
			self.isInPen = True

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
			if (sheep != self and (self.getDistToSheep(sheep) < Sheepy.CONST_SHEEP_NEIGHBOR_RANGE*1.5 or len(closeSheep) < 6)):
				if (len(closeSheep) == 6):
					indexer = 0
					while (indexer < 6):
						if (QuickMaths.getMagnitude(closeSheep[indexer].position, self.position) > QuickMaths.getMagnitude(sheep.position, self.position)):
							closeSheep[indexer] = sheep
						indexer+=1
				else:
					closeSheep.append(sheep)

		for sheep in closeSheep:
			#pygame.draw.line(self.screen, (0,255,0), self.position, sheep.position)
			netMass = QuickMaths.addTuples(netMass, sheep.position)

		count = len(closeSheep)
		if (self.position[0] < 0 or self.position[0] > 800 or self.position[1] < 0 or self.position[1] > 600):
			QuickMaths.addTuples(netMass, (400,300))
		if (len(closeSheep) == 0):
			return netMass
		else:
			v = QuickMaths.getScaled(netMass, 1/count)
			v = QuickMaths.subTuples(v, self.position)
			return QuickMaths.getNormalized(v)

	def computeSeparation(self, step, sheepList, penList, dog):
		closeSheep = []
		netSeparation = (0,0)
		for sheep in sheepList:
			if (sheep != self and self.getDistToSheep(sheep) < Sheepy.CONST_SHEEP_NEIGHBOR_RANGE*.75):
				netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples(sheep.position, self.position)), 10))
				closeSheep.append(sheep)
		
		count = len(closeSheep)
		for wall in penList:
			if (QuickMaths.getMagnitude(self.position, wall[1]) < Sheepy.CONST_PEN_RANGE):
				pygame.draw.line(self.screen,(255,0,0), self.position, wall[1])
				netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples(wall[1], self.position)), Sheepy.CONST_PEN_RANGE - QuickMaths.getMagnitude(self.position, wall[1])))
				count+=1
		if (QuickMaths.getMagnitude(self.position, dog.position) < Sheepy.CONST_SHEEP_LENGTH * 10):
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples(dog.position, self.position)), 25))
			count+=1

		#Sheep Avoid Screen Boundaries (Or not apparently)
		if (self.position[1] < Sheepy.CONST_WALL_RANGE and self.position[1] > 0):
			#pygame.draw.line(self.screen,(255,0,0), self.position, (self.position[0], 0))
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples((self.position[0], 0), self.position)), 150))
			count+=1
		if (600 - (self.position[1]+self.original_Sprite.get_height()) < Sheepy.CONST_WALL_RANGE and self.position[1] < 600):
			#pygame.draw.line(self.screen,(255,255,0), self.position, ((self.position[0], 600)))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples((self.position[0], 600), self.position)), 150))
		if (self.position[0] < Sheepy.CONST_WALL_RANGE and self.position[0] > 0):
			#pygame.draw.line(self.screen,(255,0,0), self.position, (0, self.position[1]))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples((0, self.position[1]),self.position)), 150))
		if (800 - self.position[0] < Sheepy.CONST_WALL_RANGE and self.position[0] < 800):
			#pygame.draw.line(self.screen,(255,0,0), self.position, (800, self.position[1]))
			count+=1
			netSeparation = QuickMaths.addTuples(netSeparation, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples((800, self.position[1]), self.position)), 150))

		if (count == 0):
			return (0,0)
		else:
			return QuickMaths.getNormalized(QuickMaths.getScaled(QuickMaths.getScaled(netSeparation, 1/count), -1))

		
	def flee(self, target, step):
		self.velocity = QuickMaths.subTuples(self.position, target.position)
		self.position = QuickMaths.addTuples(self.position, QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), step * self.currVel))