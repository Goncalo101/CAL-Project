========================== SMART DELIVERY ========================
-- Bibliotecas Utilizadas:
    GraphViewer -> Visualização de Grafos
    MutablePriorityQueue -> Usada no algoritmo de Dijkstra

-- Instruções de compilação
    Para a compilação do projeto foi usado CMake para gerar o makefile presente em cmake-build-debug.
    O executavel tem de estar nessa mesma pasta visto que é nela que se encontram as pastas de mapas que
    serão transformados em grafos.

    Para compilar num sistema Unix basta correr a seguinte sequencia de comandos:
        $ cd cmake-build-debug
        $ make clean
        $ make
        $ ./CAL-Project <aveiro|braga|coimbra|ermesinde|fafe|gondomar|lisboa|maia|porto|portugal|viseu>

    Ou então, como alternativa usar o ficheiro run.sh fornecido:
        $ sh run.sh <aveiro|braga|coimbra|ermesinde|fafe|gondomar|lisboa|maia|porto|portugal|viseu>


 