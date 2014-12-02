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

	#Leer cuadricula
	num = ""
	count = 0
	while True:
		c = r_file.read(1)
		if c == '\n':
			w_file.write(chr(int(num)))
			num = ""
			count+=1
			if count == 5:
				break
		elif c == ' ':
			w_file.write(chr(int(num)))
			num = ""
			continue
		else:
			num += c
	
	w_file.write("\xFF")

	#Nombre paquete
	while True:
		c = r_file.read(1)
		if not c:
			break
		else:
			w_file.write(c)
	
	print("---------------------------")
	print("Se ha generado el Paquete \""+file+".dat"+"\" correctamente!")
	raw_input("Pulsa Enter para salir.")
	
except IOError as e:
	print("I/O error({0}): {1}".format(e.errno, e.strerror))
	raw_input("Pulsa Enter para salir.")