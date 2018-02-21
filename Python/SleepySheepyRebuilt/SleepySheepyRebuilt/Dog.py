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
		self.behaviourState = "Seek"
		self.position = position
		self.maxSpeed = 60
		#self.wanderTimer = 0
		self.targetCounter = 60
		self.velocity = QuickMaths.getNormalized((0,0))
		self.target = QuickMaths.addTuples(self.position, self.velocity)

	def update(self, step, sheepList):
		self.orientation = math.degrees(math.atan2(self.velocity[0], self.velocity[1]))
		#if (self.behaviourState == "Wander"):
		#	self.wander(step)
		if (self.targetCounter >= 60 or self.target == (0,0)):
			#self.target = sheepList[r.randint(0, len(sheepList)-1)]
			self.target = QuickMaths.findClosest(self, sheepList)
			self.targetCounter = 0
		else:
			self.targetCounter += 1
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
		self.velocity = QuickMaths.subTuples(target.position, self.position)
		self.position = QuickMaths.addTuples(self.position, QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), step * self.maxSpeed))
		if (self.behaviourState != "Pursue"):
			self.tarDest = target

	def pursue(self, target, step):
		if(isinstance(target, Enemy)):
			dir = QuickMaths.addTuples(Vector(400,300), QuickMaths.subTuples((QuickMaths.subTuples(target.position, Vector(400,300)), self.position)))
			dist = QuickMaths.getMagnitude(dir)
			spd = self.maxSpeed
			pred = 0
			if (spd <= dist / self.maxPred):
				pred = self.maxPred
			else :
				pred = dist / self.maxPred

			self.tarDest = QuickMaths.getScaled(QuickMaths.addTuples((target.drawRect.center[0], target.drawRect.center[1]), target.velocity), pred)
			self.seek(QuickMaths.getScaled(QuickMaths.addTuples(target.position, target.velocity), pred), step)
			
	def draw(self, screen):
		self.drawRect = pygame.Rect(self.position[0], self.position[1], self.original_Sprite.get_width(), self.original_Sprite.get_height())
		self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
		x,y = self.drawRect.center
		self.drawRect = self.sprite.get_rect()
		self.drawRect.center = (x,y)
		pygame.draw.line(screen, (255,0,0), self.drawRect.center, QuickMaths.addTuples(self.drawRect.center, QuickMaths.getScaled(QuickMaths.getNormalized(self.velocity), self.maxSpeed)), 2)
		screen.blit(self.sprite, (self.position[0], self.position[1]))