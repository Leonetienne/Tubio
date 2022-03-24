#include "Framework.h"

#ifndef _WIN
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

void Deamonize() {
  // Fork me, kernel-san :o
  pid_t fork_res = fork();

  // Error handling...
  if (fork_res < 0) {
    std::cerr << "Aww shit! Deamonizing failed! Couldn't get forked..." << std::endl;
    exit(-1);
  }

  // Close launcher process...
  if (fork_res > 0) {
    std::cout << "Successfully spawned tubio daemon... Exiting launcher gracefully..." << std::endl;
    exit(0);
  }

  // And set the daemon process free
  if (setsid() < 0) {
    std::cerr << "Aww shit! Deamonizing failed! Couldn't create new session..." << std::endl;
    exit(-1);
  }

}

#endif


int main()
{
  #ifndef _WIN
//    Deamonize();
  #endif

	Framework framework;
	framework.Run();

	return 0;
}
