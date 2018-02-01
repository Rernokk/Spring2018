import pygame
import Vector
from pygame import *
from Vector import *

class Enemy(object):
    """Character Object Base"""
    position = Vector(0,0)
    velocity = Vector(0,0)
    orientation = 0
    angAccel = 0
    angVel = 0
    size = 0
    drawRect = pygame.Rect(0,0,0,0)
    behaviourState = "Seek"
    target = Vector(0,0)

    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        self.position = position
        self.velocity = velocity
        self.orientation = orientation
        self.angAccel = angAccel
        self.angVel = angVel
        self.size = size

    def __str__(self):
        print("Position: " + position + ", Velocity: " + velocity + ", Orientation: " + orientation + ", Angular Accel: " + angAccel + ", Angular Velocity: " + angVel)
        
    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.size, self.size)
        pygame.draw.rect(screen, (0,128,255), self.drawRect)
        pygame.draw.line(screen, (255,255,255), self.drawRect.center, (self.position.x + self.size / 2 + self.velocity.normalize().scale(self.size).x, self.position.y + self.size / 2 + self.velocity.normalize().scale(self.size).y), 3)
        
    def update(self):
        self.position += self.velocity.normalize()

    def updateByCollision(self):
        self.velocity = self.velocity.scale(-1)
        self.position += self.velocity

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        Enemy.__init__(self, size, position, velocity, orientation, 0, 0)

    def update(self):
        if (self.behaviourState == "Wander"):
            print("Wandering")
        elif (self.behaviourState == "Seek"):
            self.seek(Vector(150, 150))
        
    def seek(self, target):
        if (self.position.distance(target) < 10):
            self.position = target
            self.behaviourState = "Wander"
        else:
            self.position += (target - self.position).normalize()