import pygame
import Enemy
import Vector
import Wall
from pygame import *
from Enemy import *
from Vector import *
from Wall import *

CONST_FACTOR = 1000
CONST_FLOCK_RANGE = 100
pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
#Enemies =  [#Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #Humanoid(16, Vector(500, 100), Vector(1,0), 0, 0, 0)
           #,Humanoid(16, Vector(300, 100), Vector(1,0), 0,0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Vehicle(16, Vector(200, 200), Vector(1,0), 75, 45, 45)
           #,Gunship(16, Vector(300, 100), Vector(1,0), 45, 45, 45)
           #]

#Enemies[0].seek(Vector(300, 100), 1)

#i = 0
#while (i < 28):
#    Enemies.append(Humanoid(16, Vector(random.uniform(32, 768), random.uniform(32, 568)), Vector(1,0), 0, 0, 0))
#    i+= 1

SheepHerd = []
j = 0
while (j < 16):
    SheepHerd.append(SleepySheepy(16, Vector(random.uniform(500, 700), random.uniform(400, 500)), 0, 0, random.uniform(180, 180), 180, 0))
    j+=1

#Pen
CenterPoint = (200, 150)
PenHeight = 150
PenWidth = 200
Pen = Wall([Vector(CenterPoint[0] - int(PenWidth/2), CenterPoint[1] + PenHeight), Vector(CenterPoint[0] - PenWidth, CenterPoint[1] + PenHeight), Vector(CenterPoint[0] - PenWidth, CenterPoint[1] - PenHeight), Vector(CenterPoint[0] + PenWidth, CenterPoint[1] - PenHeight), Vector(CenterPoint[0] + PenWidth, CenterPoint[1] + PenHeight), Vector(CenterPoint[0] + int(PenWidth/2), CenterPoint[1] + PenHeight)], 3)
RoomBoundary = Wall([Vector(10, 10), Vector(790, 10), Vector(790, 590), Vector(10, 590), Vector(10, 10)], 0)

#Dog/Wolf
Wolf = Humanoid(16, Vector(700, 500), Vector(0,-1), 90, 0, 0)

#Game Loop
clock = time.Clock()
while not done:
    clock.tick(144)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    #for character in SheepHerd:
    #    character.update(clock.get_time()/CONST_FACTOR)
    #    for otherChar in SheepHerd:
    #        if (otherChar != character):
    #            if (character.drawRect.colliderect(otherChar.drawRect)):
    #                otherChar.updateByCollision(clock.get_time()/CONST_FACTOR)
                
    #            if (character.position.distance(otherChar.position) < 100 and character.behaviourState == "Wander" and otherChar.behaviourState == "Wander"):
    #                character.registerUnitInRange(otherChar, "Fight")
    #                otherChar.registerUnitInRange(character, "Flight")

        #if (character.position.x + character.size > 800 or character.position.x < 0 or character.position.y < 0 or character.position.y + character.size > 600):
            #character.updateByCollision(clock.get_time()/CONST_FACTOR)

            
    for sheep in SheepHerd:
        sheep.nearby = []
        for otherSheep in SheepHerd:
            if (sheep is not otherSheep and sheep.position.distance(otherSheep.position) <= CONST_FLOCK_RANGE):
                sheep.nearby.append(otherSheep)
        sheep.updateFlock(clock.get_time() * .001)


    screen.fill((100, 149, 237))
    Wolf.update(clock.get_time() * .001)
    for character in SheepHerd:
        tempPos = character.position
        tempOri = character.orientation
        character.update(clock.get_time() * .001)
        collides, point = Pen.checkCollision(screen, character.position, character.velocity.normalize() * 96)
        if (collides):
            character.position = tempPos
            character.orientation = tempOri
            character.flee(point, clock.get_time() * .001)
        collides, point = RoomBoundary.checkCollision(screen, character.position, character.velocity.normalize() * 96)
        if (collides):
            print((point - character.position).calculateVectorAngle())
            character.flee(point, clock.get_time() * .001)

    Wolf.draw(screen)
    for character in SheepHerd:
        character.draw(screen)
    Pen.draw(screen)
    RoomBoundary.draw(screen)
    pygame.display.flip()