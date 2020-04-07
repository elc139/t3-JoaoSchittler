# Programação Paralela Trabalho 3
Feito por João Gabriel da Cunha Schittler - 201810078

O arquivo OpenMPABC.cpp está no repositório de entrega do trabalho.

Comentários sobre o código:
- Para poder melhor visualizar as direrenças entre os schedules, foi mudado o método SharedArray::addChar(), fazendo com que ele receba um índice externo.

No programa, são testados os schedules Static, Dynamic com e sem mutex, Guided, Auto.
Static, Dynamic com mutex e Guided são testados com o chucksize padrão e chunksize 5.
# Exemplo de saída do programa

Static default
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 

Dynamic default
CABABCABCABCABCABCABCABACABCABCABCABCABCACBACBACABCABACBACCA
A=22 B=18 C=20 

Guided default
CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAAAAAAAABBBBAAAAABB
A=20 B=20 C=20 

Auto
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 

Dynamic sem mutex
CBACBCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA--
A=19 B=19 C=20 

Static chunksize 5
AAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCC
A=20 B=20 C=20 

Dynamic chunksize 5
BBBBBAAAAACCCCCAAAAABBBBBCCCCCAAAAACCCCCBBBBBAAAAACCCCCBBBBB
A=20 B=20 C=20 

Guided chunksize 5
BBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAAAAAAAACCCCCAAAAAC
A=20 B=20 C=20 

Eu uso um schedule diferente para testar sem mutex, pois, usando o iterador do for como indice do array faz com que não existam condições de corrida devido ao scheduling.
# Referências

https://computing.llnl.gov/tutorials/openMP/#RunTimeLibrary

https://www.rookiehpc.com/openmp/docs/omp_sched_t.php
