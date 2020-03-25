## Práctica 2 - Buscador SSOOIIGLE

### Objetivo
Se solicita el diseño y codificación del buscador SSOOIIGLE, capaz de buscar información en un fichero de forma paralela con múltiples hilos. En líneas generales, el usuario debe especificar al buscador el fichero donde buscar, la palabra que desea buscar y el número de hilos que se deben emplear en la búsqueda. 

De esta forma el fichero se "dividirá" en tantas partes como hilos se indiquen. Por otro lado, el buscador debe mostrar por pantalla la aparición de la palabra en el archivo con la siguiente información: 
* **Fragmento** en el que se ha encontrado la palabra. A su vez un fragmento se define mediante un *número de hilo*, *línea inicial* y *línea final*.
* **Línea del fichero** en la que ha sido encontrada. 
* Referencia a las dos palabras entre las que se encuentra la buscada: palabra anterior y palabra posterior. 

El resultado de la búsqueda debe mostrarse en orden. 

### Compilación del código
Para compilar se escribe en el terminal 
```shell
make all
```

### Ejecución del código 
Para la ejecución de manera genérica se escribe en el terminal 
```shell
./exec/SSOOIIGLE /books/<nombre_fichero> <palabra_a_buscar> <numero_hilos>
```

Para probar he facilitado con el Makefile un par de ejemplos. Para el primero se escribe 
```shell
make test
``` 
El cual mostrará un ejemplo de la búsqueda de una palabra con tilde
![Texto alternativo](/img/test.png)

Para el segundo se escribe 
```shell
make test1
```
El cual mostrará un ejemplo de la búsqueda de una palabra normal. 
![Texto alternativo](/img/test1.png)
