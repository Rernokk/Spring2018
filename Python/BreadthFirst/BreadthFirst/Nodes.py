import pygame
import random
import math

class Vector(object):
    x = 0
    y = 0
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__ (self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__ (self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, other):
        return Vector(self.x * other, self.y * other)

    def length (self):
        return math.sqrt(self.x ** 2 + self.y ** 2)

    def normalized(self):
        return Vector(self.x / self.length(), self.y / self.length())

    def distance(self, other):
        return math.sqrt((self.x - other.x) ** 2 + (self.y - other.y) ** 2)

    def VecToPygame(self):
        return pygame.math.Vector2(self.x, self.y)

class Node (object):
    def __init__(self, pos):
        self.position = pos
        self.col = (random.uniform(0.0, 255.0),random.uniform(0.0, 255.0),random.uniform(0.0, 255.0))
        self.neighbors = []
        
    def draw(self, surf):
        for neighbor in self.neighbors:
            pygame.draw.line(surf, self.col, self.position.VecToPygame(), neighbor.position.VecToPygame(), 1)

    def shiftNode (self, dir, factor):
        self.position += dir.normalized() * factor

    def flareSelf(self, surf):
        pygame.draw.circle(surf, (0, 255, 255), (int(self.position.VecToPygame().x), int(self.position.VecToPygame().y)), 10, 0)