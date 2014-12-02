# -*- coding: utf-8 -*-
import sys
import binascii

archivo = ""

while archivo == "":
	print("---------------------------")
	archivo = raw_input("Introduce el nombre del archivo (sin extension; el archivo de salida se llamara igual que el de entrada): ")
	

file = str(archivo)

filas = 0;
columnas = 0;

try:
	r_file = open(file+".txt",'r')
	w_file = open(file+".dat", 'wb+')

	#Filas
	num = ""
	while True:
		c = r_file.read(1)
		if c == '\n':
			break
		else:
			num += c
	filas = int(num)
	w_file.write(chr(filas))

	#Columnas
	num = ""
	while True:
		c = r_file.read(1)
		if c == '\n':
			break
		else:
			num += c
	columnas = int(num)
	w_file.write(chr(columnas))

	#Leer matriz
	for i in range(filas*(columnas+1)):
		c = r_file.read(1)
		if c != '\n':
			w_file.write(chr(int(c)))

	#Nombre picross y autor
	while True:
		c = r_file.read(1)
		if not c:
			break
		if c == '\n':
			w_file.write("\xFF")
		else:
				w_file.write(c)

	print("---------------------------")
	print("Se ha generado el Picross \""+file+".dat"+"\" correctamente!")
	raw_input("Pulsa Enter para salir.")

except IOError as e:
	print("I/O error({0}): {1}".format(e.errno, e.strerror))
	raw_input("Pulsa Enter para salir.")