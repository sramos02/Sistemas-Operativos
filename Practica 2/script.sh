#bin/bash!
# Creamos la carpeta "tmp" desde cero.
rm -R -f tmp
mkdir tmp
# Copiamos los archivos tanto en nuestro
#  sistema de ficheros como en la carpeta "tmp".
cp ./src/myFS.h ./tmp
cp ./src/myFS.h ./mount-point
cp ./src/fuseLib.c ./tmp
cp ./src/fuseLib.c ./mount-point
./my.fsck virtual-disk
# Comprobamos que los ficheros copiados son iguales.
if(diff ./tmp/myFS.h ./mount-point/myFS.h) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
if(diff ./tmp/fuseLib.c ./mount-point/fuseLib.c) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
# Truncamos el primer fichero para que ocupe un bloque de datos menos.
truncate -o -s -1 ./tmp/fuseLib.c
truncate -o -s -1 ./mount-point/fuseLib.c
./my.fsck virtual-disk
# Comprobamos que los ficheros truncados son iguales.
if(diff ./src/fuseLib.c ./tmp/fuseLib.c) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
if(diff ./src/fuseLib.c ./mount-point/fuseLib.c) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
# Creamos y copiamos un tercer archivo en el sistema de ficheros.
echo "Esto es el tercer archivo." > ./tmp/TercerArchivo.txt
cp ./tmp/TercerArchivo.txt ./mount-point/TercerArchivo.txt
./my.fsck virtual-disk
# Comprobamos que los nuevos archivos se han creado correctamente.
if(diff ./tmp/TercerArchivo.txt ./mount-point/TercerArchivo.txt) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
# Truncamos el segundo fichero haciendo que ocupe un bloque de datos más.
truncate -o -s +1 ./tmp/myFS.h
truncate -o -s +1 ./mount-point/myFS.h
./my.fsck virtual-disk
# Comprobamos que los ficheros truncados son iguales
if(diff ./src/myFS.h ./tmp/myFS.h) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;
if(diff ./src/myFS.h ./mount-point/myFS.h) > /dev/null then
	echo "Iguales."
else echo "No son iguales."
fi;