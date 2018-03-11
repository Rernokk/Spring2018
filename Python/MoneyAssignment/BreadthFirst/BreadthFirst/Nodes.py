import pygame
import random
import math

class Vector(object):
	x = 0
	y = 0
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def __add__(self, other):
		return Vector(self.x + other.x, self.y + other.y)

	def __sub__(self, other):
		return Vector(self.x - other.x, self.y - other.y)

	def __mul__(self, other):
		return Vector(self.x * other, self.y * other)

	def __str__(self):
		return ("<Vector: " + str(self.x) + ", " + str(self.y) + ">")

	def length(self):
		return math.sqrt(self.x ** 2 + self.y ** 2)

	def normalized(self):
		return Vector(self.x / self.length(), self.y / self.length())

	def distance(self, other):
		return math.sqrt((self.x - other.x) ** 2 + (self.y - other.y) ** 2)

	def VecToPygame(self):
		return pygame.math.Vector2(self.x, self.y)

class Node(object):
	def __init__(self, pos, indX, indY):
		self.position = pos
		#self.col = (random.uniform(0.0, 255.0),random.uniform(0.0,
		#255.0),random.uniform(0.0, 255.0))
		self.col = (255,0,0)
		self.neighbors = []
		self.visited = False
		self.indX = indX
		self.indY = indY
		self.backNode = 0
		self.isObstacle = False
		self.isCoin = False
		self.obstacleColor = (60,0,0)
		self.obstacleOffSetX = 0
		self.obstacleOffSetY = 0
		self.wallShifting = False
		self.obstacleTargetTile = 0
		self.timeSinceCheckMove = random.uniform(0, .5)
		self.isOccupied = False

	def __lt__(self, other):
		return self.costSoFar < other.costSoFar

	def draw(self, surf, drawNeighbors):
		pygame.draw.rect(surf, (0,0,0), pygame.Rect(self.position.x - 13, self.position.y - 13, 26, 26))
		pygame.draw.rect(surf, self.col, pygame.Rect(self.position.x - 11, self.position.y - 11, 22, 22))
		if (drawNeighbors):
			for neighbor in self.neighbors:
				if (not neighbor[0].isObstacle):
					pygame.draw.line(surf, (255,0,0), self.position.VecToPygame(), neighbor[0].position.VecToPygame(), 1)
		if (self.isCoin):
			pygame.draw.circle(surf, (0,0,255), (int(self.position.x), int(self.position.y)), 8)

	def drawObstacle(self, surf):
		if (self.isObstacle):
			pygame.draw.rect(surf, self.obstacleColor, pygame.Rect(self.position.x - 13 + self.obstacleOffSetX, self.position.y - 13 + self.obstacleOffSetY, 26, 26))

	def shiftNode(self, dir, factor):
		self.position += dir.normalized() * factor
		if (self.position.x < 0):
			self.position.x = 0
		if (self.position.x > 800):
			self.position.x = 800
		if (self.position.y > 600):
			self.position.y = 600
		if (self.position.y < 0):
			self.position.y = 0

	def flareSelf(self, surf):
		pygame.draw.circle(surf, (0, 255, 255), (int(self.position.VecToPygame().x), int(self.position.VecToPygame().y)), 10, 0)

	def update(self, time):
		self.timeSinceCheckMove += time
		if (self.timeSinceCheckMove >= 1):
			self.timeSinceCheckMove = 0
			if (self.isObstacle):
				open = []
				for node in self.neighbors:
					if (node[0] != 0 and not node[0].isObstacle and not node[0].isCoin and not node[0].isObstacle):
						open.append(node[0])

				if (len(open) > 0):
					if (not self.wallShifting):
						if (random.uniform(0, 1) < .02):
							self.wallShifting = True
							self.obstacleTargetTile = open[random.randint(0, len(open) - 1)]
					
		if (self.obstacleTargetTile != 0):
			offset = (self.obstacleTargetTile.position - self.position).normalized()
			self.obstacleOffSetX += offset.x
			self.obstacleOffSetY += offset.y
			if ((abs(self.obstacleOffSetX) > abs(self.obstacleTargetTile.position.x - self.position.x)) or (abs(self.obstacleOffSetY) > abs(self.obstacleTargetTile.position.y - self.position.y))):
				self.obstacleOffSetX = 0
				self.obstacleOffSetY = 0
				self.obstacleTargetTile.isObstacle = True
				self.isObstacle = False
				self.obstacleTargetTile = 0