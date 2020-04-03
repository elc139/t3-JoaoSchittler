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
   int index;
   int size;
   bool usemutex;
public:
   SharedArray(int n, bool use) : size(n), index(0), usemutex(use){
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray() {
      delete[] array;
   }
   void addChar(char c) {
   		if(usemutex)
   		{
   			#pragma omp critical
   			{
   				array[index] = c;
      			spendSomeTime();
      			index++;
			}
		}
      	array[index] = c;
      	spendSomeTime();
      	index++;
   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 10000; i++) {
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
   SharedArray* array;
public:
   ArrayFiller(bool usemutex,omp_sched_t mode) {
      array = new SharedArray(nThreads * nTimes, usemutex);
      schedule = mode;
   }
   void fillArrayConcurrently() {
   		omp_set_num_threads(nThreads);
   		omp_set_schedule(schedule,0); // Value less than 1 = default chunksize
   		int i = 0;
		int size = nThreads*nTimes;
		char c = 'A';
		
      	#pragma omp parallel for private(i,c) shared(array)
      	for(i = 0; i < size;i++)
      	{
      		c = 'A' + omp_get_thread_num();
      		array->addChar(c);
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
	ArrayFiller a1 = ArrayFiller(true,omp_sched_static);
	a1.fillArrayConcurrently();
	a1.printStats();
}
