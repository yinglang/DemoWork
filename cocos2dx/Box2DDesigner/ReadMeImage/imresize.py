# coding=utf-8
from PIL import Image
files = 'run'.split(' ')
for file in files:
	im = Image.open(file+'.png')
	im = im.resize((800, 600))
	im.save(file + ".png")