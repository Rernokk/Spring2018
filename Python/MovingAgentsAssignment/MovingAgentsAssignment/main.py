import pygame
import Enemy
import Vector
from pygame import *
from Enemy import *
from Vector import *

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
Enemies = [Humanoid(60, Vector(200,100), Vector(1,0), 45, 0, 0),
           Humanoid(60, Vector(400,300), Vector(0,0), 0, 0, 0)]
           #Humanoid(60, Vector(200,500), Vector(1,0), -45, 0, 0)]

clock = time.Clock()
while not done:
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    pressed = pygame.key.get_pressed()
    if pressed[pygame.K_x] : Enemies[0].seek(Enemies[1])

    for character in Enemies:
        for otherChar in Enemies:
            if (otherChar != character):
                if (character.drawRect.colliderect(otherChar.drawRect)):
                    otherChar.updateByCollision()
                    print("Collided!")
        character.update()

    screen.fill((100, 149, 237))
    for character in Enemies:
        character.draw(screen)
    pygame.display.flip()