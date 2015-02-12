#include <iostream>
#include <boost/bind.hpp>

#include "threadpool.hpp"

using namespace boost::threadpool;

// Some example tasks
void first_task()
{
  std::cout << "Here in 1" << std::endl;
  sleep(2);
  std::cout << "Done in 1" << std::endl;
}
void second_task()
{
  std::cout << "Here in 2" << std::endl;
  sleep(2);
  std::cout << "Done in 2" << std::endl;
}

void third_task()
{
  std::cout << "Here in 3" << std::endl;
  sleep(2);
  std::cout << "Done in 3" << std::endl;
}

void task(unsigned int i)
{
  std::cout << "Here in " << i << std::endl;
  sleep(2);
  std::cout << "Done in " << i << std::endl;
}
 
void execute_with_threadpool()
{
   // Create a thread pool.
   pool tp(2);
    
   // Add some tasks to the pool.
   tp.schedule(&first_task);
   tp.schedule(&second_task);
   tp.schedule(&third_task);

    std::cout << "active = " << tp.active() << std::endl;
    std::cout << "pending = " << tp.pending() << std::endl;
    std::cout << "size = " << tp.size() << std::endl;

   // Leave this function and wait until all tasks are finished.
}

void execute_another_threadpool()
{
   // Create a thread pool.
   pool tp(2);
    
   // Add some tasks to the pool.
   for(unsigned int i=0; i<10; ++i)
	tp.schedule(boost::bind(&task, i));

    std::cout << "active = " << tp.active() << std::endl;
    std::cout << "pending = " << tp.pending() << std::endl;
    std::cout << "size = " << tp.size() << std::endl;

   // Leave this function and wait until all tasks are finished.
}


int main(int argc, char** argv)
{
	execute_another_threadpool();
	

	return 0;
}

