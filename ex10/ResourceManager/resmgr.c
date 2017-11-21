#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "fifo.h"
#include <fcntl.h>


dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_read_task_D(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write_task_A (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_write_task_B (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_write_task_D (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
void* counterFunc ();

enum {
	UP,
	DOWN,
	IDLE
} COMMAND;

pthread_mutex_t mutex;
pthread_mutex_t FIFO_mutex;
pthread_t counterThread;
fifo_t fifo;

int command = IDLE;
char buf[128];
long counter = 0;
int blocking=0;
int nonblock = 0;
int client_id=0;

void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	printf("Start resource manager\n");
	pthread_mutex_init( &mutex, NULL );

	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	// override functions
	//io_funcs.read = io_read;
	io_funcs.read = io_read_task_D;
	//io_funcs.write = io_write_task_A;
	//io_funcs.write = io_write_task_B;
	io_funcs.write = io_write_task_D;

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("Resmanager attach");
	ctp = dispatch_context_alloc(dpp);


	pthread_create( &counterThread, NULL, counterFunc, NULL );

	// wait forever, handling messages.
	while(1)
	{
		if (!(ctp = dispatch_block(ctp)))
			error("Dispatch block");
		dispatch_handler(ctp);
	}

	pthread_join( counterThread, NULL );
	exit(EXIT_SUCCESS);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{

	printf( "Cat/Read command \n" );

	if(!ocb->offset)
	{
		// set data to return
		SETIOV(ctp->iov, buf, strlen(buf));
		_IO_SET_READ_NBYTES(ctp, strlen(buf));

		// increase the offset (new reads will not get the same data)
		ocb->offset = 1;

		// return
		return (_RESMGR_NPARTS(1));
	}
	else
	{
		// set to return no data
		_IO_SET_READ_NBYTES(ctp, 0);

		// return
		return (_RESMGR_NPARTS(0));
	}
}

/*
 * Task A
 */

int io_write_task_A (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	printf( "Echo/Write command \n" );

	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

	pthread_mutex_lock( &mutex );
	resmgr_msgread(ctp, buf, msg->i.nbytes, sizeof(msg->i));
	buf [msg->i.nbytes] = '\0';
	printf ("Message: %s \n", buf);
	pthread_mutex_unlock( &mutex );

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS (0));
}


int io_write_task_B (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	printf( "Echo/Write command \n" );

	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

	pthread_mutex_lock( &mutex );
	resmgr_msgread(ctp, buf, msg->i.nbytes, sizeof(msg->i));
	buf [msg->i.nbytes] = '\0';
	printf ("Message: %s \n", buf);

	if ( !strcmp(buf,"up\n") ) {
			command = UP;
		} else if ( !strcmp(buf,"down\n") ) {
			command = DOWN;
		} else if ( !strcmp(buf,"stop\n") ) {
			printf( "Stopping counter\n" );
			pthread_cancel( counterThread );
		} else {
			printf( "Error 404 not found\n" );
		}

	pthread_mutex_unlock( &mutex );

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS (0));
}

/*
 * Part of task B
 */
void* counterFunc (){

	for(;;){
		pthread_mutex_lock( &mutex );
		if(command == UP){
			printf( "Count++\n" );
			counter++;
		}
		if(command == DOWN){
			printf( "Count--\n" );
			counter--;
		}
		if(command != IDLE){
			printf( "Current counter %d \n", counter);
		}
		command = IDLE;
		pthread_mutex_unlock( &mutex );
		delay( 100 );
	}

	return NULL;
}

/*
 * Task D
 */

int io_write_task_D (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	char buf_D[WIDTH];

	printf( "Echo/Write command \n" );

	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

	pthread_mutex_lock( &FIFO_mutex );
	resmgr_msgread(ctp, buf_D, msg->i.nbytes, sizeof(msg->i));
	buf_D [msg->i.nbytes] = '\0';
	fifo_add_string( &fifo, buf_D );
	fifo_print(&fifo);

	if ( blocking ) {
		printf( "Block released\n" );
		fifo_rem_string( &fifo, buf_D);
		printf( "Sending %s\n", buf_D );
		MsgReply(client_id, 0, buf_D, strlen(buf_D)+1);
		blocking = 0;
	}
	pthread_mutex_unlock( &FIFO_mutex );

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS (0));
}

int io_read_task_D(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{

	if ( blocking ) {
		_IO_SET_READ_NBYTES(ctp, 0);
		return (_RESMGR_NPARTS(0));
	}

	char buf_D[WIDTH];

	if(!ocb->offset)
	{
		if ( fifo_status(&fifo) ) {

			pthread_mutex_lock( &FIFO_mutex );
			fifo_rem_string( &fifo, buf_D);
			printf( "Return %s\n", buf_D );
			SETIOV(ctp->iov, buf_D, strlen(buf_D));
			_IO_SET_READ_NBYTES(ctp, strlen(buf_D));

			ocb->offset = 1;
			pthread_mutex_unlock( &FIFO_mutex );
			return (_RESMGR_NPARTS(1));

		} else {
			if ( nonblock ) {
				printf( "Return nothing\n" );
				_IO_SET_READ_NBYTES(ctp, 0);
				return (_RESMGR_NPARTS(0));
			} else {
				printf( "Blocking\n" );
				blocking = 1;
				client_id = ctp->rcvid;
				return (_RESMGR_NOREPLY);
			}
		}
	}
	else
	{
		// return nothing
		_IO_SET_READ_NBYTES(ctp, 0);
		return (_RESMGR_NPARTS(0));
	}
}

