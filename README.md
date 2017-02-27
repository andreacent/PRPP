Se almacena el grafo como listas de adyacencia
Se obtiene las componentes conexas del conjunto R con dfs
Unir la componente conexa que contiene al vertice deposito con las demas componentes conexas del grafo, usando aristas del conjunto P unido Q unido R.
Para saber si es factible la union se saca el siguiente calculo GananciaComponente[0] - 2 * costoArista + GananciaComponente[i], si es mayor a 0, esa arista se agrega a nuestra solucion, de caso contrario se ignora.
Si quedan hojas luego de esto, se revisa de que tipo son. Solo permitimos hojas que pertenezcan a aritas del conjunto R.
Corro kruskal-Dijkstra (permitiendo ciclos)
