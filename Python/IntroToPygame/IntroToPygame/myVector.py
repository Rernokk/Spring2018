import math

class myVector(object):
    """description of class"""
    x = 0
    y = 0

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        print("Vector: " + self.x + ", " + self.y)

    def __add__(self, other):
        return myVector(self.x + other.x, self.y + other.y)

    def __subtract__ (self, other):
        return myVector (self.x - other.x, self.y - other.y)

    def dot(self, other):
        return self.x * other.y + self.y * other.x

    def scale (self, val):
        return myVector(self.x * val, self.y * val)

    def length (self):
        return math.sqrt(self.x * self.x + self.y * self.y)

    def normalize(self):
        return myVector(self.x/self.length(), self.y/self.length())
