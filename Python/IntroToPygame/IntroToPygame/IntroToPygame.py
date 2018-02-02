import pygame
import myVector
import Enemy
from pygame import *
from myVector import *
from Enemy import *

pygame.init()
screen = pygame.display.set_mode((800,600))
done = False
playerX = 30
playerY = 30

myEnemy = Enemy(myVector(200, 200), myVector(1, 1), 60)

clock = time.Clock()
while not done:
   clock.tick(60)
   for event in pygame.event.get():
       if event.type == pygame.QUIT:
           done = True

   pressed = pygame.key.get_pressed()
   if pressed[pygame.K_w] : playerY -= 3
   if pressed[pygame.K_s] : playerY += 3
   if pressed[pygame.K_a] : playerX -= 3
   if pressed[pygame.K_d] : playerX += 3
   screen.fill((100,100,100))
   pygame.draw.rect(screen, (0, 128, 255), pygame.Rect(playerX, playerY,60,60))
   myEnemy.update()
   myEnemy.draw(screen)
   pygame.display.flip()
