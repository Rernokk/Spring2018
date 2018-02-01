import myVector
import pygame
from myVector import *
from pygame import *

class Enemy(object):
    """description of class"""
    position = myVector(0,0)
    velocity = myVector(0,0)
    size = 30

    def __init__(self, position, velocity, size):
        self.position = position
        self.velocity = velocity
        self.size = size

    def draw (self, screen):
        pygame.draw.rect(screen, (0,128,255), pygame.Rect(self.position.x, self.position.y, self.size, self.size))
        pygame.draw.line(screen, (255,255,255), (self.position.x + self.size/2, self.position.y + self.size/2), (self.position.x + self.size/2 + self.velocity.normalize().scale(self.size).x, self.position.y + self.size/2 + self.velocity.normalize().scale(self.size).y), 3)

    def update(self):
        self.position += self.velocity.normalize()
