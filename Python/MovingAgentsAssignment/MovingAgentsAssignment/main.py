import pygame
import Enemy
import Vector
from pygame import *
from Enemy import *
from Vector import *

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
Enemies = [Humanoid(60, Vector(200,100), Vector(0,0), 45, 0, 0),
           Vehicle(60, Vector(200, 200), Vector(0,0), 45, 45, 45),
           Gunship(60, Vector(500, 400), Vector(0,0), 45, 45, 45)]

Enemies[0].seek(Vector(300, 100), 1)

clock = time.Clock()
while not done:
    clock.tick(144)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    for character in Enemies:
        for otherChar in Enemies:
            if (otherChar != character):
                if (character.drawRect.colliderect(otherChar.drawRect)):
                    otherChar.updateByCollision(clock.get_time()/1000)
                elif (character.position.distance(otherChar.position) < 200):
                    character.registerUnitInRange(otherChar)
        character.update(clock.get_time() / 1000)
        if (character.position.x + character.size > 800 or character.position.x < 0 or character.position.y < 0 or character.position.y + character.size > 600):
            character.updateByCollision(clock.get_time()/1000)

    screen.fill((100, 149, 237))
    for character in Enemies:
        character.draw(screen)
    pygame.display.flip()