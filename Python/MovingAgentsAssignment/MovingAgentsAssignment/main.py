import pygame
import Enemy
import Vector
from pygame import *
from Enemy import *
from Vector import *
CONST_FACTOR = 1000
pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
Enemies =  [#Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           Humanoid(16, Vector(500, 100), Vector(1,0), 90, 0, 0)
           ,Humanoid(16, Vector(300, 100), Vector(1,0),60,0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Humanoid(16, Vector(random.uniform(16 * 2, 800 - (16 * 2)), random.uniform(16 * 2, 600 - (16 * 2))), Vector(random.uniform(-10, 10),random.uniform(-10, 10)), random.uniform(-180, 180), 0, 0)
           #,Vehicle(16, Vector(200, 200), Vector(1,0), 75, 45, 45)
           #,Gunship(16, Vector(300, 100), Vector(1,0), 45, 45, 45)
           ]

#Enemies[0].seek(Vector(300, 100), 1)

clock = time.Clock()
while not done:
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    for character in Enemies:
        for otherChar in Enemies:
            if (otherChar != character):
                if (character.drawRect.colliderect(otherChar.drawRect)):
                    otherChar.updateByCollision(clock.get_time()/CONST_FACTOR)
                
                if (character.position.distance(otherChar.position) < 400):
                    character.registerUnitInRange(otherChar, "Fight")
                    otherChar.registerUnitInRange(character, "Flight")

        character.update(clock.get_time()/CONST_FACTOR)
        #if (character.position.x + character.size > 800 or character.position.x < 0 or character.position.y < 0 or character.position.y + character.size > 600):
            #character.updateByCollision(clock.get_time()/CONST_FACTOR)

    screen.fill((100, 149, 237))
    for character in Enemies:
        character.draw(screen)
    pygame.display.flip()