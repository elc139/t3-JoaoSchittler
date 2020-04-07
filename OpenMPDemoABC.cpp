#include <omp.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <string>
#define STATIC  1
#define DYNAMIC 2
#define GUIDED  3
#define AUTO    4

class SharedArray {
private:
   char* array;
   int size;
public:
   SharedArray(int n) : size(n){
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray() {
      delete[] array;
   }
   void addChar(char c,int i)
   {
        #pragma omp critical
        {
            array[i] = c;
            spendSomeTime();
        }
   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 1000; i++) {
         for (int j = 0; j < 100; j++) {
            // These loops shouldn't be removed by the compiler
         }
      }
   }
};


class ArrayFiller {
private:
   static const int nThreads = 3;
   static const int nTimes = 20;
   omp_sched_t schedule;
   int chunksize;
   bool usemutex;
   SharedArray* array;
public:
   ArrayFiller(bool usemutex,omp_sched_t mode,int chunksize = 0) {
      array = new SharedArray(nThreads * nTimes);
      schedule = mode;
      this->usemutex = usemutex;
      this->chunksize = chunksize;
   }
   void fillArrayConcurrently() {
        omp_set_num_threads(nThreads);
        int i = 0;
        int size = nThreads*nTimes;
        char c = 'A';
        omp_set_schedule(schedule,chunksize); // Value less than 1 = default chunksize

        //Caso especifico para demonstrar condição de corrida
        if (schedule == omp_sched_dynamic && !usemutex)
        {
            int cont = 0;
            #pragma omp parallel for schedule(runtime) private(i,c) shared(array)
            for(i = 0; i < size;i++)            
            { 
                c = 'A' + omp_get_thread_num();
                array->addChar(c,cont);
                cont++;
            }
            return;
        }
        //Caso geral
        #pragma omp parallel for schedule(runtime) private(i,c) shared(array)
        for(i = 0; i < size;i++)            
        { 
            c = 'A' + omp_get_thread_num();
            array->addChar(c,i);
        }

   }
   void printStats() {
      std::cout << array->toString() << std::endl;
      for (int i = 0; i < nThreads; ++i)
         std::cout << (char) ('A'+i) << "=" 
                   << array->countOccurrences('A'+i) << " ";
      std::cout << std::endl;
   }
   ~ArrayFiller() {
      delete array;
   }
};


int main() {
    //All runtime schedule variations
    ArrayFiller a1 = ArrayFiller(true,omp_sched_static);
    a1.fillArrayConcurrently();
    printf("Static default\n");
    a1.printStats();

    ArrayFiller a2 = ArrayFiller(true,omp_sched_dynamic);
    a2.fillArrayConcurrently();
    printf("\nDynamic default\n");
    a2.printStats();

    ArrayFiller a3 = ArrayFiller(true,omp_sched_guided);
    a3.fillArrayConcurrently();
    printf("\nGuided default\n");
    a3.printStats();

    ArrayFiller a4 = ArrayFiller(true,omp_sched_auto);
    a4.fillArrayConcurrently();
    printf("\nAuto\n");
    a4.printStats();

    ArrayFiller a5 = ArrayFiller(false,omp_sched_dynamic);
    a5.fillArrayConcurrently();
    printf("\nDynamic sem mutex\n");
    a5.printStats();

    ArrayFiller a6 = ArrayFiller(true,omp_sched_static,5);
    a6.fillArrayConcurrently();
    printf("\nStatic chunksize 5\n");
    a6.printStats();

    ArrayFiller a7 = ArrayFiller(true,omp_sched_dynamic,5);
    a7.fillArrayConcurrently();
    printf("\nDynamic chunksize 5\n");
    a7.printStats();

    ArrayFiller a8 = ArrayFiller(true,omp_sched_guided,5);
    a8.fillArrayConcurrently();
    printf("\nGuided chunksize 5\n");
    a8.printStats();
    printf("\a\n");


}
