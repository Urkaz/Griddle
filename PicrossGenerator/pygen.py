# -*- coding: utf-8 -*-
import sys
import binascii


total = len(sys.argv)

if total <= 1:
	print("---------------------------")
	print("Se necesita un archivo de entrada")
	print("USO:\t'pygen.py n_23'\n\t\t   ^\n\t(El nombre del archivo de entrada no lleva la extension)")
	print("---------------------------")
else:
	file = str(sys.argv[1])

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

		print("Se ha generado el Picross \""+file+".dat"+"\" correctamente!")
	except IOError as e:
		print("I/O error({0}): {1}".format(e.errno, e.strerror))