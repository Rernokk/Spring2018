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
    target = Vector(200,200)
    selfRotDelay = 0
    sprite = pygame.Surface((100,100))
    original_Sprite = 0
    orientation = 0

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
        pygame.draw.line(screen, (255, 0, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.ghostRotation(self.orientation).normalize().x, self.velocity.ghostRotation(self.orientation).normalize().y)*60, 3)
        pygame.draw.line(screen, (255, 255, 255), self.drawRect.topleft, self.drawRect.topright, 3)
        pygame.draw.line(screen, (255, 255, 255), self.drawRect.topleft, self.drawRect.bottomleft, 3)
        pygame.draw.line(screen, (255, 255, 255), self.drawRect.bottomright, self.drawRect.topright, 3)
        pygame.draw.line(screen, (255, 255, 255), self.drawRect.bottomleft, self.drawRect.bottomright, 3)
        
    def update(self, step):
        self.velocity.rotate(self.orientation)
        self.position += self.velocity.normalize().scale(step)

    def updateByCollision(self, step):
        self.orientation -= 180
        self.velocity.rotate(self.orientation)
        self.velocity = self.velocity.normalize().scale(-1 * step)
        self.position += self.velocity

    def registerUnitInRange(self, unit):
        if (typeof(unit) == Humanoid):
            print("Human is in range")

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = 0
        Enemy.__init__(self, size, position, velocity, orientation, 0, 0)

    def update(self, step):
        #print("Calculated: " + str(self.velocity.calculateVectorAngle()) + ", orientation = " + str(self.orientation))
        if (self.target is not None and (type(self.target) == Vector or type(self.target) == Enemy)):
            if (self.behaviourState == "Wander"):
                if (self.selfRotDelay > 1):
                    self.orientation += random.uniform(-1, 1) * random.uniform(-1, 1) * 2
                    self.selfRotDelay = 0
                else:
                    self.selfRotDelay+=1
                self.velocity.rotate(self.orientation)
                self.target = self.position + self.velocity.normalize()
                self.seek(self.target, step)
            elif (self.behaviourState == "Seek"):
                self.seek(self.target, step)
            elif (self.behaviourState == "Evade"):
                print("Evading")
            elif(self.behaviourState == "Flee"):
                print("Flee")
        
    def seek(self, target, step):
        self.target = target
        if (type(target) == Enemy):
            if (self.position.distance(target.position) < 10):
                self.position = target.position
                self.behaviourState = "Wander"
            elif(self.position.distance(target.position) > 10000):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step
        elif(type(target) == Vector):
            if (self.position.distance(target) < 10):
                self.position = target
                self.behaviourState = "Wander"
            elif(self.position.distance(target) > 10000):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step
        else:
            print("Invalid Target")

    def flee(self, target, step):
        if (type(target) == Vector):
            if (self.position.distance(target) > 100000):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step
        elif (type(target) == Enemy):
            if (self.position.distance(target.position) > 100000):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step
        else:
            print("Invalid Target")

    def registerUnitInRange(self, unit):
        if (type(unit) == Humanoid):
            print("Humanoid: Vehicle in range, seeking")
        elif (type(unit) == Gunship):
            print("Humanoid: Gunship in range, pursuing")

class Vehicle(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = 0
        Enemy.__init__(self, size, position, velocity, orientation, angAccel, angVel)

    def registerUnitInRange(self, unit):
        if (type(unit) == Gunship):
            print("Vehicle: Gunship in range, seeking")
        elif (type(unit) == Humanoid):
            print("Vehicle: Humanoid in range, pursuing")

class Gunship (Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = 0
        Enemy.__init__(self, size, position, velocity, orientation, angAccel, angVel)

    def registerUnitInRange(self, unit):
        if (typeof(unit) == Humanoid):
            print("Gunship: Humanoid in range, seeking")
        elif (typeof(unit) == Vehicle):
            print("Gunship: Vehicle in range, pursuing")