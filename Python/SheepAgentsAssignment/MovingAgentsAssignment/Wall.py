import pygame
import Vector
from pygame import *
from Vector import *

class Wall(object):
    """description of class"""
    def __init__(self, points, radius):
        self.points = points
        self.radius = radius

    def draw(self, screen):
        i = 0

        for point in self.points:
            pygame.draw.circle(screen, (255,0,0), (point.x, point.y) , self.radius, 0)

        while (i < len(self.points) - 1):
            s1 = (self.points[i] + Vector(1,1) * self.radius).convertToPy()
            s2 = (self.points[i] + Vector(-1, 1) * self.radius).convertToPy()
            s3 = (self.points[i] + Vector(1, -1) * self.radius).convertToPy()
            s4 = (self.points[i] + Vector(-1, -1) * self.radius).convertToPy()
            e1 = (self.points[i+1] + Vector(1,1) * self.radius).convertToPy()
            e2 = (self.points[i+1] + Vector(-1, 1) * self.radius).convertToPy()
            e3 = (self.points[i+1] + Vector(1, -1) * self.radius).convertToPy()
            e4 = (self.points[i+1] + Vector(-1, -1) * self.radius).convertToPy()
            #pygame.draw.line(screen, (0,0,255), self.points[i].convertToPy(), self.points[i+1].convertToPy(), 3)
            pygame.draw.line(screen, (0,0,255), s1, e1, 3)
            pygame.draw.line(screen, (0,0,255), s2, e2, 3)
            pygame.draw.line(screen, (0,0,255), s3, e3, 3)
            pygame.draw.line(screen, (0,0,255), s4, e4, 3)
            i+=1
         
    def checkSign(self, value):
        if (value < 0):
            return -1
        elif (value >= 0):
            return 1

    def checkCollision(self, screen, pos, vec):
        i = 0
        while (i < len(self.points)-1):
            if ((self.points[i] - self.points[i+1]).x == 0):
                #Check if inside Y-Range
                if ((self.points[i].y < pos.y and self.points[i+1].y > pos.y) or (self.points[i].y > pos.y and self.points[i+1].y < pos.y)):
                    #Check for cross in X
                    if (self.checkSign(self.points[i].x - pos.x) != self.checkSign(self.points[i+1].x - (pos + vec).x)):
                        #Collision!
                        pygame.draw.circle(screen, (255,0,0), (int(self.points[i].x), int((pos + vec).y)), 15)
                        return (True, Vector(self.points[i].x,(pos+vec).y))
            elif (self.points[i].y - self.points[i+1].y == 0):
                #Check if inside Y-Range
                if ((self.points[i].x < pos.x and self.points[i+1].x > pos.x) or (self.points[i].x > pos.x and self.points[i+1].x < pos.x)):
                    #Check for cross in Y
                    if (self.checkSign(self.points[i].y - pos.y) != self.checkSign(self.points[i+1].y - (pos + vec).y)):
                        #Collision!
                        pygame.draw.circle(screen, (0,255,0), (int((pos+vec).x), int(self.points[i].y)), 15)
                        return (True, Vector((pos+vec).x, self.points[i].y))
            i+=1
        return (False, 0)