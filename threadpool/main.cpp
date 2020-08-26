#include <iostream>
#include <thread>
#include "fixed_thread_pool.h"


using namespace std;

void test_pool(int test){
    printf( "tasks: [%d]  \n", test);
}

int main() {
    gu::Fixed_thread_pool* thread_pool = new gu::Fixed_thread_pool(10);

    for(int i = 0; i<13; i++) {

        thread_pool->execute(test_pool, i);
    }

   this_thread::sleep_for(10s);


}