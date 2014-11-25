# -*- coding: utf-8 -*-
import sys
import binascii


total = len(sys.argv)

if total <= 2:
	print("---------------------------")
	print("Se necesita un archivo de entrada y otro de salida como parametros")
	print("USO:\t'pygen.py picross.txt n_23'\n\t\t\t\t^\n\t(El nombre del archivo de salida no lleva la extension)")
	print("---------------------------")
else:
	file = str(sys.argv[1])
	nombre = str(sys.argv[2])

	filas = 0;
	columnas = 0;

	try:
		r_file = open(file,'r')
		w_file = open(nombre+".dat", 'wb+')

		filas = int(r_file.read(1)) #filas
		w_file.write(chr(filas))

		r_file.read(1) #Se ignora fin de linea

		columnas = int(r_file.read(1)) #columnas
		w_file.write(chr(columnas))

		c = r_file.read(1) #Se ignora fin de linea

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
				w_file.write("\x00")
			else:
				w_file.write(c)

		print("Se ha generado el Picross \""+nombre+".dat"+"\" correctamente!")
	except IOError as e:
		print("I/O error({0}): {1}".format(e.errno, e.strerror))