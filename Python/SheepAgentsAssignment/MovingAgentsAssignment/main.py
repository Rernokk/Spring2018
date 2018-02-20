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


#Sheepsies
SheepHerd = []
j = 0
while (j < 9):
	SheepHerd.append(SleepySheepy(32, Vector(random.uniform(300, 500), random.uniform(50, 75)), 0, 0, 0, 180, 0))
	j+=1

#Dog/Wolf
Wolf = Humanoid(16, Vector(700, 200), Vector(0,-1), 90, 0, 0)
targetTimer = 0
tar = SheepHerd[random.randint(0, len(SheepHerd)-1)]
for sheep in SheepHerd:
	sheep.wolfRef = Wolf

#Pen
CenterPoint = (400, 300)
PenHeight = 150
PenWidth = 200
Pen = Wall([Vector(CenterPoint[0] - int(PenWidth/2), CenterPoint[1] + PenHeight), Vector(CenterPoint[0] - PenWidth, CenterPoint[1] + PenHeight), Vector(CenterPoint[0] - PenWidth, CenterPoint[1] - PenHeight), Vector(CenterPoint[0] + PenWidth, CenterPoint[1] - PenHeight), Vector(CenterPoint[0] + PenWidth, CenterPoint[1] + PenHeight), Vector(CenterPoint[0] + int(PenWidth/2), CenterPoint[1] + PenHeight)], 10)
RoomBoundary = Wall([Vector(10, 10), Vector(790, 10), Vector(790, 590), Vector(10, 590), Vector(10, 10)], 10)


#Game Loop
clock = time.Clock()
while not done:
	clock.tick(144)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
			
	for sheep in SheepHerd:
		sheep.nearby = []
		for otherSheep in SheepHerd:
			if (sheep is not otherSheep and sheep.position.distance(otherSheep.position) <= CONST_FLOCK_RANGE):
				sheep.nearby.append(otherSheep)
		sheep.updateFlock(clock.get_time() * .001)


	screen.fill((100, 149, 237))
	for character in SheepHerd:
		tempPos = character.position
		tempOri = character.orientation
		character.update(clock.get_time() * .001)
		if ((character.position + character.velocity).x < 0 or (character.position + character.velocity).x >= 800):
			character.velocity = Vector(character.velocity.x * -1, character.velocity.y)
		elif ((character.position + character.velocity).y < 0 or (character.position + character.velocity).y >= 600):
			character.velocity = Vector(character.velocity.x, character.velocity.y * -1)
		#if Pen.checkInRange(character):
			#print("Pen bounds in range")
	Wolf.behaviourState = "Pursue"
	Wolf.pursue(tar, clock.get_time() * .001)
	if (targetTimer > 360):
		targetTimer = 0
		tar = SheepHerd[random.randint(0, len(SheepHerd)-1)]
	else:
		targetTimer += 1
	Wolf.draw(screen)
	for character in SheepHerd:
		character.draw(screen)
	#Pen.draw(screen)
	#RoomBoundary.draw(screen)
	pygame.display.flip()