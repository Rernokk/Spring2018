import math

class Vector(object):
    """description of class"""
    x = 0
    y = 0
    
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return ("Vector: ", self.x, ", ", self.y)

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__ (self, other):
        return Vector (self.x - other.x, self.y - other.y)

    def dot(self, other):
        return self.x * other.y + self.y * other.x

    def scale (self, val):
        return Vector(self.x * val, self.y * val)

    def length (self):
        return math.sqrt(self.x * self.x + self.y * self.y)

    def normalize(self):
        return Vector(self.x/self.length(), self.y/self.length())

    def distance(self, other):
        return (self - other).length()