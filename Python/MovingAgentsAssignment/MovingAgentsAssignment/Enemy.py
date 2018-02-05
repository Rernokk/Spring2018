import pygame
import Vector
import random
from pygame import *
from Vector import *

class Enemy(object):
    """Character Object Base"""
    position = Vector(0,0)
    velocity = Vector(1,0)
    angAccel = 0
    angVel = 0
    size = 0
    drawRect = pygame.Rect(0,0,0,0)
    behaviourState = "Wander"
    target = 0
    selfRotDelay = 0
    sprite = pygame.Surface((100,100))
    original_Sprite = 0
    orientation = 0
    dir = 1

    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        self.position = position
        self.velocity = velocity
        self.orientation = orientation
        self.angAccel = angAccel
        self.angVel = angVel
        self.size = size
        self.selfRotDelay = 0
        self.angle = 0

    def __str__(self):
        print("Position: " + position + ", Velocity: " + velocity +  ", Angular Accel: " + angAccel + ", Angular Velocity: " + angVel)
        
    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.sprite.get_width(), self.sprite.get_height())
        #pygame.draw.rect(screen, (0,128,255), self.drawRect)
        #self.angle += 1 % 360
        self.sprite = pygame.transform.rotate(self.original_Sprite, -self.orientation)
        x,y = self.drawRect.center
        self.drawRect = self.sprite.get_rect()
        self.drawRect.center = (x,y)
        screen.blit(self.sprite, (self.position.x, self.position.y))
        pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.normalize().x, self.velocity.normalize().y)*60, 3)
        pygame.draw.line(screen, (255, 0, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.normalize().x, self.velocity.normalize().y)*60, 3)
        
    def update(self):
        self.position += self.velocity.rotate(self.orientation).normalize()

    def updateByCollision(self):
        self.velocity = self.velocity.scale(-1)
        self.position += self.velocity

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        self.original_Sprite = spr.convert_alpha()
        Enemy.__init__(self, size, position, velocity, orientation, 0, 0)

    def update(self):
        #self.angle = math.atan2(-self.velocity.y, self.velocity.x)
        if (self.behaviourState == "Wander"):
            if (self.selfRotDelay > 1):
                if (self.orientation > 45 or self.orientation < -45):
                    self.dir *= -1
                self.orientation += self.dir
                #self.angle += random.uniform(-1, 1) * random.uniform(-1, 1) * 5
                self.selfRotDelay = 0
            else:
                self.selfRotDelay+=1
            self.velocity.rotate(self.orientation)
            self.seek(self.position + self.velocity.normalize())
        elif (self.behaviourState == "Seek"):
            self.seek(Vector(550, 150))
        elif (self.behaviourState == "Evade"):
            print("Evading")
        elif(self.behaviourState == "Flee"):
            print("Flee")
        
    def seek(self, target):
        if (self.position.distance(target.position) < 10):
            self.position = target.position
            self.behaviourState = "Wander"
        elif(self.position.distance(target.position) > 100):
            self.behaviourState = "Wander"
        else:
            self.velocity += self.velocity.rotate(self.orientation)
            self.position += self.velocity.normalize();

    def flee(self, point):
        if (self.position.distance(point) > 100):
            self.behaviourState = "Wander"
        else:
            self.velocity += self.velocity.rotate(self.orientation)
            self.position += self.velocity.normalize()