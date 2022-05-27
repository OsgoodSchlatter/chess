# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/ioctl.h>
# include <sys/time.h>
# include <time.h>



# include "consoleEscape.h"
# define SERVERPORT 8888
# define LOCALHOST "127.0.0.1"

# define MYMSGLEN   2048
# define PSEUDOLEN    12

typedef struct {
  
  int sock ;
  int lines ;
  int columns ;
  char pseudo [ PSEUDOLEN ] ;
} threadAttrType ;

void get_window_attr ( int * lines, int *columns )
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	*lines = w.ws_row ;
	*columns = w.ws_col ;
	    
}
void question ( int sock, char * msg )
{
	int code, length;
    // int ret;

	// Send the length of the string.
	length = strlen ( msg ) + 1 ;
	
	// Send the string of characters.
	code = send ( sock, msg, length, 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		close ( sock ) ;
		exit ( 1 ) ;
	}
	
	//printf ( "Message sent to server, waiting for the answer ..... \n " ) ;
	// Wait for the answer.
	memset ( msg, 0, length ) ;
	
	code = recv ( sock, ( char * ) msg, length, 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		close ( sock ) ;
		exit ( 1 ) ;
	}

	// Connection closed by remote peer
	if ( code == 0 )
	{
	  // Release ressources.
	  code = close ( sock ) ;
	  if ( code == -1 ) {
		  perror ( "close" ) ;
		 
	  }
	  exit ( 1 ) ;
	}
	 
}

int connecting ( )
{
	// Socket creation.
	int sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock == -1 ) {
		perror ( "socket" ) ;
		exit ( 1 ) ;
	}

	// Initialisation of the sockaddr_in data structure

	struct sockaddr_in addr ;
	socklen_t len ;
	
	struct sockaddr_in newAddr ;
	struct sockaddr_in peerAddr ;
	
	memset ( & addr, 0, sizeof ( struct sockaddr_in ) ) ;
	addr . sin_family = AF_INET ;
	addr . sin_port = htons(SERVERPORT) ;
	addr . sin_addr . s_addr = inet_addr(LOCALHOST) ;

	// Name the socket.
	int code ;
	code = connect ( sock, ( struct sockaddr * ) & addr, sizeof ( struct sockaddr_in ) ) ;
	if ( code == -1 ) {
		perror ( "connect" ) ;
		close ( sock ) ;
		exit ( 1 ) ;
	}

	len = sizeof ( struct sockaddr_in ) ;
	code = getsockname ( sock, ( struct sockaddr * ) & newAddr, & len ) ;
	if ( code == -1 ) {
	      perror ( "getsockname" ) ;
	      close ( sock ) ;
		
	      exit ( 1 ) ;
	}
	len = sizeof ( struct sockaddr_in ) ;
	code = getpeername ( sock, ( struct sockaddr * ) & peerAddr, & len ) ;
	if ( code == -1 ) {
	      perror ( "getpeername" ) ;
	      close ( sock ) ;
	      exit ( 1 ) ;
	}
		
	printf ( " The local address bound to the current socket --> %s:%d \n" , inet_ntoa ( newAddr.sin_addr ), ntohs ( newAddr.sin_port )  ) ;
	printf ( " The peer address bound to the peer socket --> %s:%d \n" , inet_ntoa ( peerAddr.sin_addr ), ntohs ( peerAddr.sin_port )  ) ;
	// Return the socket ID.
	return sock ;
}

void * sock_reading_thread_handler ( void * theAttr )
{
	
	threadAttrType * attr = ( threadAttrType * ) theAttr ;	
	int sock  ;
	// int length ;
	char string [ MYMSGLEN ] ;
	char param [ 5 ] ;
	int code ;
	sock = attr->sock ;
	
	while ( 1 )
	{
	  //printf ( " Waiting for the server messages  ..... \n " ) ;
	  // Wait for the answer.
	  memset ( string, 0, MYMSGLEN ) ;
	
	  code = recv ( sock, string, MYMSGLEN, 0 ) ;
	  if ( code == -1 ) {
		perror ( "recv" ) ;
		close ( sock ) ;
		pthread_exit ( NULL )  ;
	  }

	  // Connection closed by remote peer
	  if ( code == 0 )
	    {
	      // Release ressources.
	      code = close ( sock ) ;
	      if ( code == -1 ) {
		    perror ( "close" ) ;
		 
	      }
	      pthread_exit ( NULL )  ;
	     }
	     
	     memset ( param, 0, sizeof (param) ) ;
	     sprintf ( param, "%d;%d",   attr->lines - 6, 1 ) ;
	     memoriseCur ;
	     positionneCur(param) ;
	     couleur("36") ;

		 // printer le message recu
	    //  printf ( "\n--> %s\n", string ) ;
	     restitueCur ;
	     couleur("37") ;
	     fflush(stdout) ;
	     
	}
}

void * display_time ( void * theAttr )
{
	threadAttrType * attr = ( threadAttrType * ) theAttr ;
	char param [ 5 ] ;
	struct timeval current_time;
	struct tm *tm ;
	while ( 1 )
	{
	    gettimeofday ( &current_time, NULL ) ;	
	    tm = localtime ( &current_time. tv_sec ) ;
	    snprintf ( param, 5, "%d;%d",   attr->lines - 4, 1 ) ;
	    memoriseCur ;
	    positionneCur(param) ;
	    effaceLigneCourante ; 
	    couleur("32") ;
	    printf ( ">>>>>>>> username : %s (%02d:%02d:%02d)\n",  attr->pseudo, tm->tm_hour, tm->tm_min, tm->tm_sec ) ;
	    
	    restitueCur ;
	    couleur("37") ;
	    fflush(stdout) ;
	    sleep ( 1 ) ;
	}
}


void* display_chess_board(void *theAttr){
    

    // threadAttrType *attr = ( threadAttrType * ) theAttr ;
    while(1){
		// printf("jiaja\n");
        sleep(3);
        // print_board();
        
        fflush(stdout) ;



    }
}



void * handle_command ( void * theAttr )
{

	
	threadAttrType * attr = ( threadAttrType * ) theAttr ;
	int sock ;
	 int length ;
	int item ;
	int code ;
	char param [ 5 ] ;
	
	sock = attr->sock ;
	// Ask for the string of characters.
	char string [ MYMSGLEN ] ;
	char buffer [ MYMSGLEN +100] ;
	while ( 1 )
	  {
	    memset ( string, 0, sizeof( string ) ) ;
	    memset ( buffer, 0, sizeof( buffer ) ) ;
	    memset ( param, 0, sizeof (param) ) ;
	    snprintf ( param, 5, "%d;%d",   attr->lines - 3, 1 ) ;
	    // positionneCur(param) ;
	    
		
	    printf ( "Please type ur next move with format \"a2,b2\":" ) ;

		 fflush(stdout) ;
	    item = scanf ( "%[^\n]%*c", string ) ;  //reads a hole line till a new line feed
	    
	    if ( item == 0 )
	      { 
		scanf("%*c");
		continue;      
	      } 
	
	    if ( !memcmp ( "quit#", string, 5 ) )
	      {

		shutdown ( sock, SHUT_RDWR ) ;  
		
		code = close ( sock ) ;
		if ( code == -1 ) {
		  perror ( "close socket" ) ;
		}
		printf ( " Client exiting upon user request \n" ) ; 
		pthread_exit ( NULL ) ;
	      }
	    
		printf("\n\n");

	// traditionnal chat
	    // sprintf ( buffer, "%s# says : %s ", attr->pseudo, string );
	//chess
		sprintf ( buffer, "%s", string );
	    length = strlen ( buffer ) ; // + 1 ;
	    code = send ( sock, buffer, length, 0 ) ;
	    if ( code == -1 ) {
		perror ( "send" ) ;
		close ( sock ) ;
		pthread_exit ( NULL ) ;
		
	    }
	   	
	    
	}
  
}



void * sock_sending_thread_handler ( void * theAttr )
{

	
	threadAttrType * attr = ( threadAttrType * ) theAttr ;
	int sock ;
	 int length ;
	int item ;
	int code ;
	char param [ 5 ] ;
	
	sock = attr->sock ;
	// Ask for the string of characters.
	char string [ MYMSGLEN ] ;
	char buffer [ MYMSGLEN +100] ;
	while ( 1 )
	  {
	    memset ( string, 0, sizeof( string ) ) ;
	    memset ( buffer, 0, sizeof( buffer ) ) ;
	    memset ( param, 0, sizeof (param) ) ;
	    // snprintf ( param, 5, "%d;%d",   attr->lines - 3, 1 ) ;
	    // positionneCur(param) ;
	    // effaceLigneCourante ; 
	    
	    // printf ( "\n----------------------------------------------------------------------\n" ) ;
	    //  effaceLigneCourante ; 
	    printf ( "Please type a message to transfer :" ) ;
	    // memoriseCur ; printf (" \n "); 
		// effaceLigneCourante ;
		 fflush(stdout) ;
	    // restitueCur ;
	    item = scanf ( "%[^\n]%*c", string ) ;  //reads a hole line till a new line feed
	    
	    if ( item == 0 )
	      {
		scanf("%*c");
		continue;      
	      } 
	
	    if ( !memcmp ( "quit#", string, 5 ) )
	      {

		shutdown ( sock, SHUT_RDWR ) ;  
		//With close() no FIN is sent on the wire but with shutdown(fd, SHUT_WR) FIN is sent on the wire?
		//Call shutdown(WR) will send FIN,but close have little different:if fd reference count not equal 0 will not send FIN.
		
		code = close ( sock ) ;
		if ( code == -1 ) {
		  perror ( "close socket" ) ;
		}
		printf ( " Client exiting upon user request \n" ) ; 
		pthread_exit ( NULL ) ;
	      }
	    
	    //printf ( "The message being sent is: '%s' \n", string ) ;
	    //printf ( "----------------------------------------------------------------------\n" ) ;

	    // Ask the question and wait for the answer.
	    // Send the string of characters.
	    



	    sprintf ( buffer, "%s# says : %s ", attr->pseudo, string );


        


	    length = strlen ( buffer ) ; // + 1 ;
	    code = send ( sock, buffer, length, 0 ) ;
	    if ( code == -1 ) {
		perror ( "send" ) ;
		close ( sock ) ;
		pthread_exit ( NULL ) ;
		
	    }
	   	
	    
	}
  
}

int main ( int argc, char * argv [ ] )
{
	
	
	int code ;
	// pthread_t  sockSendThreadId ;
	pthread_t  sockReadThreadId ;
	// pthread_t  displayTimeThreadId ;
    pthread_t displayChessBoard;
	pthread_t handleCommand;


	threadAttrType  attr;
	char param [ 64 ] ;
	
	// Connect to the server.
	int sock ;
	if ( argc < 2 )
	  {
	    printf ( " Missing username \n " ) ;
	    printf ( " Usage: %s <username> \n", argv[ 0 ] );
	    exit ( 0 ) ;
	  }
	snprintf ( attr.pseudo, 12, "%s", argv [ 1 ] ) ;
	
	sock = connecting ( ) ;
	attr.sock = sock ;
	get_window_attr ( &attr.lines, &attr.columns ) ;
	
	clrscr() ;
	memset ( param, 0, sizeof (param) ) ;
	sprintf ( param, "%d;%d", 1,  attr.lines - 5 ) ;
	
	defilZeone(param) ;
	
	code = 0;

	
	// code = pthread_create ( &sockSendThreadId,  0, sock_sending_thread_handler, ( void * ) &attr ) ;

	  if ( code != 0 )
	      {
		perror ( "Thread sockSend creation" ) ;
		close ( sock ) ; 
		
		return ( -1 ) ;
	      }
	      
	      code = pthread_create ( &sockReadThreadId,  0, sock_reading_thread_handler, ( void * ) &attr ) ;

	  if ( code != 0 )
	      {
		perror ( "Thread sockRead creation" ) ;
		close ( sock ) ; 
		
		return ( -1 ) ;
	      }
	       if ( code != 0 )
	      {
		perror ( "Thread sockSend creation" ) ;
		close ( sock ) ; 
		
		return ( -1 ) ;
	      }
	      
	    //   code = pthread_create ( &displayTimeThreadId,  0, display_time, ( void * ) &attr ) ;
          code = pthread_create ( &displayChessBoard,  0, display_chess_board, ( void * ) &attr ) ;
		  code = pthread_create ( &handleCommand,  0, handle_command, ( void * ) &attr ) ;
		  

          
	    
	pthread_join ( sockReadThreadId,  NULL ) ;
	// pthread_join ( sockSendThreadId,  NULL ) ;
	pthread_join(displayChessBoard,NULL);
	pthread_join(handleCommand,NULL);
	
	return ( 0 ) ;
}