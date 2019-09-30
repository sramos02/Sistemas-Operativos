#!/bin/bash

#return 0 -> Correct
#return 1 -> error

#Comprobará que el programa mytar está en el directorio actual y que es ejecutable.
if [ ! -x ./mytar ]; then
	#En caso contrario mostrará un mensaje informativo por pantalla y terminará.
	echo Ha ocurrido un problema, el fichero mtar no existe
	exit 1;
fi

#Comprobará si existe un directorio tmp dentro del directorio actual.
if [ -d tmp ]; then
	#Si existe lo borrará incluyendo todo lo que contenga
	rm -r tmp
fi

#Creará un nuevo directorio temporal tmp dentro del directorio actual y cambiará a este directorio
mkdir tmp && cd ./tmp

#Creará tres ficheros
	echo "Hello World" > file1.txt
	head -n10 /etc/passwd > file2.txt
	head -c1024 /dev/urandom > file3.dat  #Posible error AQUI no he podido comprobar la creación de los ficheros

	array = (file1.txt file2.txt file3.dat)

#Invocará el programa mytar que hemos desarrollado
../mytar -cf filetar.mtar ${array[@]} || { echo "Error creando mytar.\n"; exit 1; }

#Creará un directorio out y copiará el fichero filetar.mtar al nuevo directorio.
mkdir out
cp filetar.mtar out/

#Cambiará al directorio out y ejecutará el programa mytar
cd out
../../mytar -xf filetar.mtar || { echo "Error ejecutando mtar"; exit 1; }
#Comprobación
echo "Mtar creado con exito"

#Usará el programa diff para comparar los fichero extraídos con los originales
for i in "${array[@]}"; do
    echo diff "../$i" "$i"
    diff "../$i" "$i" || exit 1
done;

cd ..
echo "Script terminado con exito (Correct)"
exit 0
