#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int Global = 0;
	
void Fork(){
	int local = 0;
	printf("** fork(); ** \n");	
	pid_t pid = fork();
	if( pid == 0){ 

		//Child

		Global++;
		local++;
		printf("Child, global %i, local %i \n", Global, local);
		_exit(0);

	}else{

		//Parent

		Global++;
		local++;
		printf("Parent, global %i, local %i \n", Global, local);

	}

}

void VFork(){
	int local = 0;
	printf("** vfork(); ** \n");	
	pid_t pid = vfork();
	if( pid == 0){ 

		//Child

		local++;
		Global++;
		printf("Child, global %i, local %i \n", Global, local);
		_exit(0);

	}else{

		//Parent

		local++;
		Global++;
		printf("Parent, global %i, local %i \n", Global, local);

	}

}
void main(){
	VFork();

}


/*
Fork will make a copy of the process. An independent copy of the process. So, if a global variable contains 3 at the time you fork, each copy of the process gets their very own 3. 
And if they modify,their modifications are completely independent.
*/

/*
vfork() is an obsolete optimization. Before good memory management, fork() made a full copy of the parent's memory, so it was pretty expensive. since in many cases a fork()
was followed by exec(), which discards the current memory map and creates a new one, it was a needless expense. Nowadays, fork() doesn't copy the memory; 
it's simply set as "copy on write", so fork()+exec() is just as efficient as vfork()+exec().
*/
