import math

class Vector(object):
    """description of class"""
    x = 0
    y = 0
    
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return ("<Vector: "+ str(float(self.x)) + ", " + str(float(self.y)) + ">")

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__ (self, other):
        return Vector (self.x - other.x, self.y - other.y)

    def __mul__ (self, other):
        return Vector(self.x * other, self.y * other)

    def dot(self, other):
        return self.x * other.y + self.y * other.x

    def scale (self, val):
        return Vector(self.x * val, self.y * val)

    def length (self):
        return math.sqrt(self.x * self.x + self.y * self.y)

    def normalize(self):
        if (self.x == 0 and self.y == 0):
            return Vector(self.x,self.y)
        else:
            return Vector(self.x/self.length(), self.y/self.length())

    def distance(self, other):
        return (self - other).length()

    def angleBetween(self, other):
        return math.degrees(math.atan2(self.y - other.y, self.x - other.x));

    def rotate(self, angle):
        val = self.normalize()
        length = math.sqrt(val.x ** 2 + val.y ** 2)
        ang = math.atan2(val.y, val.x)
        #ang = math.degrees(ang)
        #if (self.y > 0 and self.x < 0):
        #    ang += 180
        #elif (self.y < 0 and self.x < 0):
        #    ang += 180
        #elif (self.y < 0 and self.x > 0):
        #    ang += 360
        
        ang += angle
        ang = math.radians(ang)
        self.x = (length * math.cos(ang))
        self.y = (length * math.sin(ang))
        return

    def ghostRotation(self, angle):
        val = self.normalize()
        length = math.sqrt(val.x ** 2 + val.y ** 2)
        ang = math.atan2(val.y, val.x)
        #ang = math.degrees(ang)
        #if (self.y > 0 and self.x < 0):
        #    ang += 180
        #elif (self.y < 0 and self.x < 0):
        #    ang += 180
        #elif (self.y < 0 and self.x > 0):
        #    ang += 360
        
        ang += angle
        ang = math.radians(ang)
        return Vector(length * math.cos(ang), length * math.sin(ang))

    def calculateVectorAngle(self):
        val = self.normalize()
        return math.degrees(math.atan2(val.y, val.x));