import pygame
import Vector
import random
from pygame import *
from Vector import *

CONST_VAL = 200

class Enemy(object):
    """Character Object Base"""
    position = Vector(0,0)
    velocity = Vector(0,0)
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
    targetPosition = Vector(0,0)

    maxSpeed = 5
    maxPred = 3

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
        return ("<P: " + str(self.position) + ", V: " + str(self.velocity) + ", AngAccel: " + str(self.angAccel) + ", AngVel: " + str(self.angVel) + ", Type: " + str(type(self)) + ">")
        
    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.sprite.get_width(), self.sprite.get_height())
        self.sprite = pygame.transform.rotate(self.original_Sprite, -self.orientation)
        x,y = self.drawRect.center
        self.drawRect = self.sprite.get_rect()
        self.drawRect.center = (x,y)
        pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.normalize().x, self.velocity.normalize().y) * 60, 3)
        pygame.draw.line(screen, (255, 0, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.ghostRotation(self.orientation).normalize().x, self.velocity.ghostRotation(self.orientation).normalize().y) * 60, 3)
        
        if (type(self.targetPosition) == Vector):
            pygame.draw.line(screen, (0, 0, 255), self.drawRect.center, pygame.math.Vector2(self.targetPosition.x, self.targetPosition.y), 2)
        elif (isinstance(self.targetPosition, Enemy)):
            pygame.draw.line(screen, (0, 0, 255), self.drawRect.center, self.target.drawRect.center, 2)
        
        screen.blit(self.sprite, (self.position.x, self.position.y))

        
    def update(self, step):
        self.velocity.rotate(self.orientation)
        self.position += self.velocity.normalize().scale(step)

    def updateByCollision(self, step):
        self.target = Vector(0,0)
        self.position = Vector(random.uniform(self.size * 2, 800 - (self.size * 2)), random.uniform(self.size * 2, 600 - (self.size * 2)))
        self.behaviourState = "Wander"
        self.orientation = random.uniform(0, 359)

    def registerUnitInRange(self, unit, style):
        print("Enemy base class range method called")
        
    def flee(self, target, step):
        if (type(target) == Vector):
            if (self.position.distance(target) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step
        elif (isinstance(target, Enemy)):
            if (self.position.distance(target.position) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                #self.velocity.rotate(self.orientation)
                self.velocity = self.position - target.position
                self.position += self.velocity.normalize() * step
        else:
            print("Invalid Target")

    def seek(self, target, step):
        self.target = target
        if (isinstance(target, Enemy)):
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

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = self.position
        self.maxSpeed = 1
        Enemy.__init__(self, size, position, velocity, orientation, 0, 0)

    def update(self, step):
        if (self.target is not None and (type(self.target) == Vector or isinstance(self.target, Enemy))):
            if (self.behaviourState == "Wander"):
                self.wander(step)
            elif (self.behaviourState == "Seek"):
                self.seek(self.target, step)
            elif (self.behaviourState == "Pursue"):
                print("Pursue")
                self.pursue(self.target, step)
            elif (self.behaviourState == "Evade"):
                print("Evading")
            elif(self.behaviourState == "Flee"):
                #print("Flee")
                self.flee(self.target, step)
    
    def wander(self, step):
        if (self.selfRotDelay > 3):
            self.orientation = (self.orientation + random.uniform(-1.0, 1.0) * 6)
            self.selfRotDelay = 0
        else:
            self.selfRotDelay+=1
        self.velocity.rotate(self.orientation)
        self.target = self.position + (self.velocity.normalize())
        self.targetPosition = self.target
        self.seek(self.target, step)

    def seek(self, target, step):
        #self.target = target
        if (isinstance(target, Enemy)):
            if (self.position.distance(target.position) < 10):
                self.position = target.position
                self.behaviourState = "Wander"
            elif(self.position.distance(target.position) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                self.orientation = self.position.angleBetween(target.position)
                self.velocity = target.position - self.position
                self.position += self.velocity.normalize() * step * self.maxSpeed
        elif(type(target) == Vector):
            if (self.position.distance(target) < 10):
                self.position = target
                self.behaviourState = "Wander"
            elif(self.position.distance(target) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step * self.maxSpeed
        else:
            print("Invalid Target")

    def flee(self, target, step):
        if (type(target) == Vector):
            if (self.position.distance(target) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                self.velocity.rotate(self.orientation)
                self.position += self.velocity.normalize() * step * self.maxSpeed
        elif (isinstance(target, Enemy)):
            if (self.position.distance(target.position) > CONST_VAL):
                self.behaviourState = "Wander"
            else:
                #self.velocity.rotate(self.orientation)
                self.velocity = self.position - target.position
                self.position += self.velocity.normalize() * step * self.maxSpeed
                self.orientation = self.velocity.calculateVectorAngle()
        else:
            print("Invalid Target")

    def pursue(self, target, step):
        dir = target.position - self.position
        dst = dir.length()
        spd = self.maxSpeed
        prediction = 0
        if (spd <= dst/self.maxPred):
            prediction = self.maxPred
        else:
            prediction = dst/spd
        self.targetPosition = target.position + (target.velocity * prediction)
        self.seek(target.position + (target.velocity * prediction), step)

        if (self.position.distance(target.position) > 100):
            self.behaviourState = "Wander"

    def registerUnitInRange(self, unit, style):
        if (self.behaviourState == "Wander"):
            if (style is "Fight"):
                if (type(self.target) is Vector):
                    if (type(unit) == Vehicle):
                        self.target = unit
                        print("Humanoid: Vehicle in range, seeking")
                    elif (type(unit) == Gunship):
                        self.target = unit
                        print("Humanoid: Gunship in range, pursuing")
                    elif (type(unit) == Humanoid):
                        self.target = unit
                        if (random.uniform(0, 1) > 1.1):
                            self.behaviourState = "Seek"
                        else:
                            self.behaviourState = "Pursue"
            elif (style is "Flight"):
                if (type(self.target) is Vector): #and type(unit) is not Humanoid):
                    self.target = unit
                    self.behaviourState = "Flee"
            print(self.behaviourState)

class Vehicle(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Vehicle.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = Vector(0,0)
        Enemy.__init__(self, size, position, velocity, orientation, angAccel, angVel)

    def registerUnitInRange(self, unit, style):
        if (style is "Fight"):
            if (type(self.target) is Vector):
                if (type(unit) == Gunship):
                    self.target = unit
                    print("Vehicle: Gunship in range, seeking")
                elif (type(unit) == Humanoid):
                    self.target = unit
                    print("Vehicle: Humanoid in range, pursuing")
        elif (style is "Flight"):
             if (type(self.target) is Vector):
                 self.target = unit
                 self.behaviourState = "Flee"
                 print(str(type(self)) + " is fleeing from " + str(type(unit)))

    def update(self, step):
        if (self.target is not None and (type(self.target) == Vector or type(self.target) == Enemy)):
            #Wandering
            if (self.behaviourState == "Wander"):
                if (self.selfRotDelay > 1):
                    self.orientation += random.uniform(-1, 1) * random.uniform(-1, 1) * step
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
                #print("Flee")
                self.flee(self, self.target, step)

    #def update(self, step):
        #print("Vehicle Target: " + str(self.target))

class Gunship(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Gunship.png")
        self.original_Sprite = spr.convert_alpha()
        self.target = Vector(0,0)
        Enemy.__init__(self, size, position, velocity, orientation, angAccel, angVel)

    def registerUnitInRange(self, unit, style):
        if (style is "Fight"):
            if (type(self.target) is Vector):
                if (type(unit) == Humanoid):
                    self.target = unit
                    print("Gunship: Humanoid in range, seeking")
                elif (type(unit) == Vehicle):
                    self.target = unit
                    print("Gunship: Vehicle in range, pursuing")
        elif (style is "Flight"):
            if (type(self.target) is Vector):
                self.target = unit
                self.behaviourState = "Flee"
                print(str(type(self)) + " is fleeing from " + str(type(unit)))
                
    def update(self, step):
        if (self.target is not None and (type(self.target) == Vector or type(self.target) == Enemy)):
            if (self.behaviourState == "Wander"):
                if (self.selfRotDelay > 1):
                    self.orientation += random.uniform(-1, 1) * random.uniform(-1, 1) * step
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
                #print("Flee")
                self.flee(self, self.target, step)

    #def update(self, step):
        #print("Gunship Target: " + str(self.target))