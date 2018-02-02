import pygame
import Vector
import random
from pygame import *
from Vector import *

class Enemy(object):
    """Character Object Base"""
    position = Vector(0,0)
    velocity = Vector(0,0)
    orientation = Vector(0,0)
    angAccel = 0
    angVel = 0
    size = 0
    drawRect = pygame.Rect(0,0,0,0)
    behaviourState = "Seek"
    target = Vector(0,0)
    selfRotDelay = 0
    sprite = pygame.Surface((100,100))

    def __init__(self, size, position, velocity, orientation, angAccel, angVel, spr):
        self.position = position
        self.velocity = velocity
        self.orientation = orientation
        self.angAccel = angAccel
        self.angVel = angVel
        self.size = size
        self.selfRotDelay = 0

    def __str__(self):
        print("Position: " + position + ", Velocity: " + velocity + ", Orientation: " + orientation + ", Angular Accel: " + angAccel + ", Angular Velocity: " + angVel)
        
    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.size, self.size)
        #pygame.draw.rect(screen, (0,128,255), self.drawRect)
        surf = pygame.Surface((self.size,self.size))
        surf = pygame.transform.rotate(surf, 45)
        screen.blit(surf, (self.position.x,self.position.y))
        pygame.draw.line(screen, (0,255,255), self.drawRect.center, (self.position.x + self.size / 2 + self.velocity.normalize().scale(self.size).x, self.position.y + self.size / 2 + self.velocity.normalize().scale(self.size).y), 3)
        pygame.draw.line(screen, (255,0,0), self.drawRect.center, (self.position.x + self.size / 2 + self.orientation.normalize().scale(self.size).x, self.position.y + self.size / 2 + self.orientation.normalize().scale(self.size).y), 3)
        
    def update(self):
        self.position += self.velocity.normalize()

    def updateByCollision(self):
        self.velocity = self.velocity.scale(-1)
        self.position += self.velocity

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        Enemy.__init__(self, size, position, velocity, orientation, 0, 0, pygame.Surface((100,100)))

    def update(self):
        if (self.orientation != self.velocity):
            self.velocity = self.orientation

        if (self.behaviourState == "Wander"):
            if (self.selfRotDelay > 3):
                self.orientation += Vector(random.triangular(-1, 1), random.triangular(-1,1)).normalize().scale(.1)
                self.selfRotDelay = 0
            else:
                self.selfRotDelay+=1
            self.seek(self.position + self.velocity)
        elif (self.behaviourState == "Seek"):
            self.seek(Vector(550, 150))
        elif (self.behaviourState == "Evade"):
            print("Evading")
        elif(self.behaviourState == "Flee"):
            print("Fleeing")
        
    def seek(self, target):
        if (self.position.distance(target) < 10):
            self.position = target
            self.behaviourState = "Wander"
        elif(self.position.distance(target) > 100):
            self.behaviourState = "Wander"
        else:
            self.orientation = (target - self.position).normalize()
            self.position += self.velocity