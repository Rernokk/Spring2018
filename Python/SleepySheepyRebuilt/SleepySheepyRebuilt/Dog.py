import pygame
import random as r
import math as m
import QuickMaths
from pygame import *
from random import *
from math import *
from QuickMaths import *

class Dog(object):

	def __init__(self, size, position, velocity, orientation, angAccel, angVel):
		spr = pygame.image.load("Dog.png")
		spr = pygame.transform.rotate(spr, -90)
		self.original_Sprite = spr.convert_alpha()
		self.behaviourState = "Pursue"
		self.position = position
		self.maxSpeed = 100
		self.maxPred = 120
		#self.wanderTimer = 0
		self.targetCounter = 60
		self.velocity = QuickMaths.getNormalized((0,0))
		self.target = (0,0)
		self.drawTar = (0,0)

	def update(self, step, sheepList):
		self.orientation = math.degrees(math.atan2(self.velocity[0], self.velocity[1]))
		#if (self.behaviourState == "Wander"):
		#	self.wander(step)
		if (self.target == (0,0) or self.targetCounter >= 600):
			#self.target = QuickMaths.findClosest(self, sheepList)
			self.target = sheepList[r.randint(0, len(sheepList)-1)]
			self.targetCounter = 0
		else:
			self.targetCounter += 1

		if (self.target != (0,0)):
			if (self.behaviourState == "Seek"):
				self.seek(self.target, step)
			elif (self.behaviourState == "Pursue"):
				self.pursue(self.target, step)
	
	def wander(self, step):
		if (self.wanderTimer > 22):
			self.wanderTimer = 0
		self.wanderTimer += 1
		self.target = QuickMaths.addTuples(self.position, (QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), 30)))
		self.seek(self.target, step)

	def seek(self, target, step):
		self.velocity = QuickMaths.subTuples(self.drawTar, self.position)
		self.position = QuickMaths.addTuples(self.position, QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), step * self.maxSpeed))

	def pursue(self, target, step):
		dir = QuickMaths.addTuples((400,300), QuickMaths.subTuples(QuickMaths.subTuples(target.position, (400,300)), self.position))
		dist = QuickMaths.getMagnitudeIndividual(dir)
		spd = self.maxSpeed
		pred = 0
		if (spd <= dist / self.maxPred):
			pred = self.maxPred
		else :
			pred = dist / self.maxPred/4
		pos = QuickMaths.addTuples(target.position, QuickMaths.getScaled(QuickMaths.getNormalized(QuickMaths.subTuples(target.drawRect.center, (400,455))), pred * target.maxLinVel))
		self.drawTar = pos
		self.seek(pos, step)
			
	def draw(self, screen):
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())
		self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
		x,y = self.drawRect.center
		self.drawRect = self.sprite.get_rect()
		self.drawRect.center = (x,y)
		pygame.draw.line(screen, (255,0,0), self.drawRect.center, QuickMaths.addTuples(self.drawRect.center, QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), self.maxSpeed)), 2)
		pygame.draw.line(screen, (255,255,0), self.drawRect.center, self.drawTar, 2)
		pygame.draw.circle(screen, (255,255,255), (400, 455), 10, 1)
		pygame.draw.line(screen,(255,0,255), self.drawTar, (400,455))
		screen.blit(self.sprite, (self.position[0], self.position[1]))