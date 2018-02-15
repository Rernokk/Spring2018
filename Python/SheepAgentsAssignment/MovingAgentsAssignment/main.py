import pygame
import Enemy
import Vector
from pygame import *
from Enemy import *
from Vector import *
CONST_FACTOR = 1000
CONST_FLOCK_RANGE = 50
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

SheepHerd = [SleepySheepy(16, Vector(400,300), 0,0,45,0,0),
             SleepySheepy(16, Vector(375,325), 0,0,45,0,0),
             SleepySheepy(16, Vector(425,275), 0,0,45,0,0),]

leadSheep = SheepHerd[random.randint(0, len(SheepHerd))]
SleepySheepy.leadSheep = leadSheep

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
    for character in SheepHerd:
        character.update(clock.get_time() * .001)

    for sheep in SheepHerd:
        for otherSheep in SheepHerd:
            if (sheep is not otherSheep and sheep.position.distance(otherSheep.position) <= CONST_FLOCK_RANGE):
                sheep.updateFlock(otherSheep)

    screen.fill((100, 149, 237))
    for character in SheepHerd:
        character.draw(screen)
    pygame.display.flip()