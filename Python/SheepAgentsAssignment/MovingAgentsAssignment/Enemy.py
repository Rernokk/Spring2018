import pygame
import Vector
import random
from pygame import *
from Vector import *

CHASE_LIM = 200

class Enemy(object):
    """Character Object Base"""
    ######################## Variables Data
    position = Vector(0,0)
    velocity = Vector(0,0)
    angAccel = 0
    angVel = 0
    size = 0
    drawRect = pygame.Rect(0,0,0,0)
    behaviourState = "Wander"
    target = Vector(200,200)
    sprite = pygame.Surface((100,100))
    original_Sprite = 0
    orientation = 0
    wanderTimer = 0
    maxSpeed = 5
    maxPred = 1200
    tarDest = Vector(0,0)

    ########################
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        self.position = position
        self.velocity = velocity
        self.orientation = orientation
        self.angAccel = angAccel
        self.angVel = angVel
        self.size = size
        self.selfRotDelay = 0
        self.angle = 0

    ########################
    def __str__(self):
        return ("<P: " + str(self.position) + ", V: " + str(self.velocity) + ", AngAccel: " + str(self.angAccel) + ", AngVel: " + str(self.angVel) + ", Type: " + str(type(self)) + ">")
        
    ########################
    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.sprite.get_width(), self.sprite.get_height())
        self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
        x,y = self.drawRect.center
        self.drawRect = self.sprite.get_rect()
        self.drawRect.center = (x,y)
        pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.normalize().x, self.velocity.normalize().y) * 20, 3)
        #pygame.draw.line(screen, (255, 0, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.ghostRotation(self.orientation).normalize().x, self.velocity.ghostRotation(self.orientation).normalize().y) * 60, 3)
        
        if (type(self.target) == Vector and self.target.distance(self.position) < CHASE_LIM):
            if (self.behaviourState == "Wander"):
                pygame.draw.line(screen, (0, 255, 255), self.drawRect.center, pygame.math.Vector2((self.velocity.normalize().x * 30) + self.drawRect.center[0], (self.velocity.normalize().y * 30) + self.drawRect.center[1]), 2)
            if (self.behaviourState == "Seek"):
                pygame.draw.line(screen, (255, 255, 0), self.drawRect.center, pygame.math.Vector2((self.velocity.normalize().x * 30) + self.drawRect.center[0], (self.velocity.normalize().y * 30) + self.drawRect.center[1]), 2)
            elif (self.behaviourState == "Pursue"):
                pygame.draw.line(screen, (255, 0, 255), self.drawRect.center, pygame.math.Vector2((self.velocity.normalize().x * 30) + self.drawRect.center[0], (self.velocity.normalize().y * 30) + self.drawRect.center[1]), 2)
            elif (self.behaviourState == "Evade"):
                pygame.draw.line(screen, (0, 0, 255), self.drawRect.center, pygame.math.Vector2((self.velocity.normalize().x * 30) + self.drawRect.center[0], (self.velocity.normalize().y * 30) + self.drawRect.center[1]), 2)
            elif (self.behaviourState == "Flee"):
                pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, pygame.math.Vector2((self.velocity.normalize().x * 30) + self.drawRect.center[0], (self.velocity.normalize().y * 30) + self.drawRect.center[1]), 2)
        elif (isinstance(self.target, Enemy)and self.target.position.distance(self.position) < CHASE_LIM):
            if (self.behaviourState == "Seek"):
                pygame.draw.line(screen, (255, 255, 0), self.drawRect.center, (self.tarDest.x, self.tarDest.y), 2)
            elif (self.behaviourState == "Pursue"):
                pygame.draw.line(screen, (255, 0, 255), self.drawRect.center, (self.tarDest.x, self.tarDest.y), 2)
            elif (self.behaviourState == "Flee"):
                pygame.draw.line(screen, (0, 0, 255), self.drawRect.center, (self.tarDest.x, self.tarDest.y), 2)
            elif (self.behaviourState == "Evade"):
                pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, (self.tarDest.x, self.tarDest.y), 2)
        
        screen.blit(self.sprite, (self.position.x, self.position.y))

    ########################
    def updateByCollision(self, step):
        self.position = Vector(random.uniform(self.size * 2, 800 - (self.size * 2)), random.uniform(self.size * 2, 600 - (self.size * 2)))
        self.target = self.position
        self.behaviourState = "Wander"
        self.orientation = random.uniform(0, 359)

    ########################
    def registerUnitInRange(self, unit, style):
        print("Enemy base class range method called")

    @staticmethod
    def checkSign(value):
        if (value < 0):
            return -1
        elif (value >= 0):
            return 1

class Humanoid(Enemy):
    def __init__(self, size, position, velocity, orientation, angAccel, angVel):
        spr = pygame.image.load("Humanoid.png")
        spr = pygame.transform.rotate(spr, -90)
        self.original_Sprite = spr.convert_alpha()
        self.target = self.position
        self.maxSpeed = random.uniform(20, 60)
        self.velocity = Vector(random.uniform(-1, 1), random.uniform(-1,1))
        Enemy.__init__(self, size, position, self.velocity, orientation, 0, 0)

    def update(self, step):
        self.orientation = math.degrees(math.atan2(self.velocity.x, self.velocity.y))
        if (self.target is not None and (type(self.target) == Vector or isinstance(self.target, Enemy))):
            if (self.behaviourState == "Wander"):
                self.wander(step)
            elif (self.behaviourState == "Seek"):
                self.seek(self.target, step)
            elif (self.behaviourState == "Pursue"):
                self.pursue(self.target, step)
            elif (self.behaviourState == "Evade"):
                self.evade(self.target, step)
            elif(self.behaviourState == "Flee"):
                self.flee(self.target, step)
    
    def wander(self, step):
        if (self.wanderTimer > 22):
            self.velocity.rotate(random.uniform(-22.5, 22.5))
            self.wanderTimer = 0
        self.wanderTimer += 1
        self.target = self.position + (self.velocity.normalize() * 30)
        self.seek(self.target, step)

    def seek(self, target, step):
        if(type(target) == Vector and target.distance(self.position) <= CHASE_LIM):
            self.velocity = (target - self.position)
            self.position += self.velocity.normalize() * step * self.maxSpeed
            if (self.behaviourState != "Pursue"):
                self.tarDest = target
        elif(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            self.velocity = target.position - self.position
            self.position += self.velocity.normalize() * step * self.maxSpeed
            if (self.behaviourState != "Pursue"):
                self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1])
        else:
            self.behaviourState = "Wander"

    def flee(self, target, step):
        if(type(target) == Vector and target.distance(self.position) <= CHASE_LIM):
            self.velocity = (self.position - target)
            if (self.behaviourState != "Evade"):
                self.tarDest = target
            self.position += self.velocity.normalize() * step * self.maxSpeed
        elif(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            self.velocity = self.position - target.position
            if (self.behaviourState != "Evade"):
                self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1])
            self.position += self.velocity.normalize() * step * self.maxSpeed
        else:
            self.behaviourState = "Wander"

    def pursue(self, target, step):
        if(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            dir = target.position - self.position
            dist = dir.length()
            spd = self.maxSpeed
            pred = 0
            if (spd <= dist / self.maxPred):
                pred = self.maxPred
            else :
                pred = dist / self.maxPred

            self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1]) + target.velocity * pred
            self.seek(target.position + target.velocity * pred, step)
        if (self.position.distance(target.position) > CHASE_LIM):
            self.behaviourState = "Wander"
        
    def evade(self, target, step):
        if(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            dir = self.position - target.position
            dist = dir.length()
            spd = self.maxSpeed
            pred = 0
            if (spd <= dist / self.maxPred):
                pred = self.maxPred
            else :
                pred = dist / self.maxPred


            self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1]) + target.velocity * pred
            self.flee(target.position + target.velocity * pred, step)
        if (self.position.distance(target.position) > CHASE_LIM):
            self.behaviourState = "Wander"

    def registerUnitInRange(self, unit, style):
        if (self.behaviourState is "Wander"):
            if (style is "Fight"):
                if (type(self.target) is Vector):
                    #if (type(unit) == Vehicle):
                    #    self.target = unit
                    #    print("Humanoid: Vehicle in range, seeking")
                    #elif (type(unit) == Gunship):
                    #    self.target = unit
                    #    print("Humanoid: Gunship in range, pursuing")
                    if (type(unit) == Humanoid):
                        self.target = unit
                        if (random.uniform(0, 1) > .5):
                            self.behaviourState = "Seek"
                        else:
                            self.behaviourState = "Pursue"
            elif (style is "Flight"):
                if (type(self.target) is Vector): #and type(unit) is not Humanoid):
                    self.target = unit
                    if (random.uniform(0, 1) > .5):
                        self.behaviourState = "Flee"
                    else:
                        self.behaviourState = "Evade"

class SleepySheepy(Enemy):
    def __init__(self, size, position, linVel, linAccel, orientation, angVel, angAccel):
        self.position = position
        self.linVel = linVel
        self.linAccel = linAccel
        self.orientation = orientation
        self.angVel = angVel
        self.angAccel = angAccel
        spr = pygame.image.load("Circle.png")
        self.original_Sprite = spr.convert_alpha()
        self.velocity = Vector(1,0).rotate(-orientation)
        self.rotTimer = 0
        self.CONST_WANDER_SPEED = 15
        self.behaviourState = "Wander"

    def update(self, step):
        #print("Reassigning: Sleepy Sheepy Position = ", SleepySheepy.position)
        if (self.behaviourState == "Wander"):
            if (self.rotTimer >= 12):
                self.velocity.rotate(random.uniform(-15, 15))
                self.rotTimer = 0
            else:
                self.rotTimer += 1
            self.orientation = -self.velocity.calculateVectorAngle()
            self.position += self.velocity * self.CONST_WANDER_SPEED * step

    def updateFlock(self,step):
        #print("Updating Flock Pattern")
        avg = self.position
        avgVel = self.velocity
        for sheep in self.nearby:
            avg += sheep.position
            if (sheep.position.distance(self.position) < 32):
                sheep.position += (sheep.position - self.position) * step * (sheep.position.distance(self.position)/32.0)
                self.position -= (sheep.position - self.position) * step * (sheep.position.distance(self.position)/32.0)
            avgVel += sheep.velocity
            
        avg = avg.average(len(self.nearby)+1)
        avgVel = avgVel.average(len(self.nearby) + 1)
        self.velocity = (self.velocity * (1 - step) + (avgVel * step))
        self.seek(avg, step)
        #self.position += self.velocity.normalize() * self.CONST_WANDER_SPEED * step

    def draw(self, screen):
        self.drawRect = pygame.Rect(self.position.x, self.position.y, self.sprite.get_width(), self.sprite.get_height())
        self.sprite = pygame.transform.rotate(self.original_Sprite, self.orientation)
        x,y = self.drawRect.center
        self.drawRect = self.sprite.get_rect()
        self.drawRect.center = (x,y)
        screen.blit(self.sprite, (self.position.x, self.position.y))
        pygame.draw.line(screen, (0, 255, 0), self.drawRect.center, self.drawRect.center + pygame.math.Vector2(self.velocity.normalize().x, self.velocity.normalize().y) * 20, 3)
        #for sheep in self.nearby:
        #    pygame.draw.line(screen, (255,255,0), self.drawRect.center, sheep.drawRect.center, 1)

    def seek(self, target, step):
        if(type(target) == Vector and target.distance(self.position) <= CHASE_LIM):
            #self.velocity = (target - self.position)
            #val = Enemy.checkSign((self.position - target).normalize().calculateVectorAngle()) * self.angVel * step if math.fabs((self.position - target).normalize().calculateVectorAngle()) > self.angVel else (self.position - target).normalize().calculateVectorAngle() * step
            val = self.angVel * step
            self.velocity.rotate(val * step)
            self.position += self.velocity.normalize() * step * self.CONST_WANDER_SPEED
            if (self.behaviourState != "Pursue"):
                self.tarDest = target
        elif(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            self.velocity = target.position - self.position
            self.position += self.velocity.normalize() * step * self.maxSpeed
            if (self.behaviourState != "Pursue"):
                self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1])
        else:
            self.behaviourState = "Wander"

    def flee(self, target, step):
        self.behaviourState = "Flee"
        if(type(target) == Vector and target.distance(self.position) <= CHASE_LIM):
            val = Enemy.checkSign(self.velocity.calculateVectorAngle()) * self.angVel * step if math.fabs((self.position - target).normalize().calculateVectorAngle()) >= self.angVel else Enemy.checkSign(self.velocity.calculateVectorAngle()) * self.angVel * step
            self.velocity.rotate(val)
            if (self.behaviourState != "Evade"):
                self.tarDest = target
            self.position += self.velocity.normalize() * step * self.CONST_WANDER_SPEED
        elif(isinstance(target, Enemy) and target.position.distance(self.position) <= CHASE_LIM):
            self.velocity = (self.position - target.position).normalize()
            if (self.behaviourState != "Evade"):
                self.tarDest = Vector(target.drawRect.center[0], target.drawRect.center[1])
            self.position += self.velocity.normalize() * step * self.CONST_WANDER_SPEED
        else:
            self.behaviourState = "Wander"

