##### Departamento de Computación y Tecnología de la Información
##### Inteligencia artificial I
##### Abril - Julio 2023
##### Profesor: Carlos Infante
##### Integrantes:
Rommel Llanos 15-10789
Jesus Marcano 12-10359

# Informe del Proyecto 2: Implementación y Evaluación de Modelos de Árboles de Juego y Algoritmos Básicos de Resolución

### Especificaciones del Sistema de Prueba

Capacidad de memoria: 8 GB de RAM
Procesador: AMD Ryzen 4500U 

### Resultados
Inicialmente se completó la representación del juego, específicamente en el archivo denominado "othello_cut.h", al añadir los movimientos que validan las diagonales que inicialmente faltaban. Posteriormente, en el archivo main se diseñó un método llamado "children", responsable de generar las posibles jugadas o "hijos" de un estado dado del juego.

Se estableció un tiempo límite de 20 minutos para la ejecución de cada algoritmo. Los resultados obtenidos de cada prueba han sido documentados en archivos .txt, los cuales se encuentran dentro de la carpeta del proyecto. En todas las variaciones del juego, el valor principal permaneció constante en -4.

##Sección 1- Implementación del Algoritmo Negamax Simple

En esta implementación, se logró llegar hasta el nodo 18 de la variación principal, lo que resultó en la expansión de 625.084.814 nodos y la generación de 876.269.598 nodos.

##Sección 2- Implementación del Algoritmo Negamax con Poda Alfa-Beta

En este caso, se logró avanzar hasta el nodo 12 de la variación principal, con la expansión de 2.230.058.150 nodos y la generación de 2.931.981.147 nodos.

##Sección 3- Implementación del Algoritmo Scout

En el algoritmo Scout, la función auxiliar "test" recibe un parámetro booleano llamado "condition". Este parámetro dictamina si se debe usar el operador mayor estricto (>) en caso de ser "true", o el operador mayor o igual (>=) en caso de ser "false". Así, las funciones “operator<" y “operator==” proporcionadas en el archivo “othello_cut.h” no se utilizaron.

A lo largo de la ejecución del algoritmo, se llegó hasta el nodo 12 de la variación principal, resultando en 693.495 nodos expandidos y 888.835 nodos generados.

##Sección 4- Implementación del Algoritmo Negascout

Durante su ejecución, este algoritmo llegó hasta el nodo 12 de la variación principal, con la expansión de 477.003.110 nodos y la generación de 623.022.874 nodos.

##Conclusión
Como resultado de las pruebas realizadas, se observó que el algoritmo Negamax con poda Alfa-Beta mostró un rendimiento superior en términos de velocidad en comparación con el Negamax simple, un resultado predecible dado que el primero omite ciertas ramas durante su ejecución. Esto también se manifiesta en la cantidad de nodos generados y expandidos, que se redujo a la mitad con la poda Alfa-Beta.

Por otro lado, el algoritmo Scout demostró ser superior al Negamax con poda, tal como se anticipaba. Esto se debe a que, según la teoría
