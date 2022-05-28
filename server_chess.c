# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
#include <stdbool.h>



# define  SERVERPORT  8888
# define  LOCALHOST   "127.0.0.1"
# define  MAXNUMDSC   2
# define  MYMSGLEN   2048
# define  PSEUDOLEN    8

typedef struct {
  
  struct sockaddr_in  addr ;
  int  sock ;
  int color;
  char pseudo [ PSEUDOLEN ] ;
  pthread_t threadId ;
  bool to_play;
  
} clientAttrType ;

typedef struct {
  
clientAttrType descriptorList [ MAXNUMDSC ] ;

 //This should be handled with care. In this program we are not taking into account 
 //simultaneous accesses to the same variable

int totalClientNumber ;

} threadArgsType ;




int close_descriptor_list ( clientAttrType descriptorList [ ],  int totalClientNumber ) 
{
	int i ;
	int code ;
	
	for ( i = 0; i < totalClientNumber ; i ++ )
		{
		    shutdown ( descriptorList [ i ].sock, SHUT_RDWR ) ;
		    code = close ( descriptorList [ i ].sock ) ;
		    if ( code == -1 ) 
		      perror ( "socket close" ) ;
      
		}
	return ( 0 ) ;
}

int find_index_desc_from_list ( clientAttrType descriptorList [ ], int sockDesc, int totalClientNumber ) 
{
	int i ;
  
	for ( i = 0; i < totalClientNumber ; i ++ )
		{
		    if ( descriptorList [ i ].sock == sockDesc )
			return ( i ) ;
      
		}
	return ( -1 ) ;
}

void remove_index_desc_from_list ( clientAttrType descriptorList [ ], int index, int * totalClientNumber ) 
{
	int i ;
  
	for ( i = index; i < ( * totalClientNumber ); i ++ )
		{
		  memcpy( &descriptorList [ i ], &descriptorList [ i + 1 ], sizeof ( clientAttrType ) ) ;
		}
	( * totalClientNumber ) -- ;
  
}


void forward_message ( char * msg, int length, clientAttrType descriptorList [ ], int * totalClientNumber )
{
  
	int i ;
	int code ;
	i = 0 ;
  
	while (  i < ( * totalClientNumber ) )
		{
		  code = send ( descriptorList [ i ].sock,  msg, length, 0 ) ;
		  if ( code == -1 )
			{
			  perror ( "send" ) ;
			  // remove this descriptor from the descriptor list 
			  shutdown (  descriptorList [ i ].sock, SHUT_RDWR) ;
			  //closing this socket will normally induce thread exit since the thread is blocked on receiving 
			  close ( descriptorList [ i ].sock ) ; 
			  remove_index_desc_from_list ( descriptorList, i, totalClientNumber ) ;
			}
		  else
		    i ++ ;
		}
}








int board[12][12]={
       {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, // ligne -1
       {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, //ligne -1
       {-1,-1,4,6,8,10,12,8,6,4,-1,-1}, //ligne 0
       {-1,-1,2,2,2,2,2,2,2,2,-1,-1}, //ligne 1
       {-1,-1,0,0,0,0,0,0,0,0,-1,-1}, //ligne 2
       {-1,-1,0,0,0,0,0,0,0,0,-1,-1}, //ligne 3
       {-1,-1,0,0,0,0,0,0,0,0,-1,-1}, //ligne 4
       {-1,-1,0,0,0,0,0,0,0,0,-1,-1}, //ligne 5
       {-1,-1,3,3,3,3,3,3,3,3,-1,-1}, //ligne 6
       {-1,-1,5,7,9,11,13,9,7,5,-1,-1}, //ligne 7
       {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, // ligne -1
       {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} //ligne -1
             
   };

int playable_moves[12][12];



bool black_is_in_check = false;
bool white_is_in_check=false;


void print_board(){
    for(int i=0;i<10;i++){
        for (int j = 0; j < 10; j++){

        switch (board[i][j])
        {
        case 2:
            printf("p |");
            break;
        case 3:
            printf("P |");
            break;
        case 4:
            printf("r |");
            break;
        case 5:
            printf("R |");
            break;
        case 6:
            printf("n |");
            break;
        case 7:
            printf("N |");
            break;
        case 8:
            printf("b |");
            break;
        case 9:
            printf("B |");
            break;
        case 10:
            printf("q |");
            break;
        case 11:
            printf("Q |");
            break;
        case 12:
            printf("k |");
            break;
        case 13:
            printf("K |");
            break;
        case 0:
            printf("  |");
            break;

        
        default:
            break;
        }
            
        }
        printf("\n");
        printf("------------------------\n");
    }
        
    }


void next_move_black_pawn(int line, int column){
     
    

    if(line==3 && board[line+1][column] == 0 && board[line+2][column]== 0 ){
       
            
            playable_moves[line+1][column]=1;
            playable_moves[line+2][column]=1;
        }
        else if (line==3 && board[line+1][column] == 0 && board[line+2][column]!= 0){
            playable_moves[line+1][column]=1;
        }
        else if (line>3 && line !=9 && board[line+1][column] ==0 ){
            playable_moves[line+1][column] = 1;
        }
        

        if(board[line+1][column-1]%2 == 1){
            playable_moves[line+1][column-1] = 2;
        }
        if(board[line+1][column+1]%2 == 1){
            
            playable_moves[line+1][column+1] = 2;
        }


}

void next_move_white_pawn(int line, int column){
    if(line==8 && board[line-1][column] == 0 && board[line-2][column]== 0 ){
            
            playable_moves[line-1][column]=1;
            playable_moves[line-2][column]=1;
        }
        else if (line==8 && board[line-1][column] == 0 && board[line-2][column]!= 0){
            playable_moves[line-1][column]=1;
        }
        else if (line<8 && line !=2 && board[line-1][column] ==0 ){
            playable_moves[line-1][column] = 1;
        }
        

        if(board[line-1][column-1]%2 == 0 && board[line-1][column-1]>0){
            playable_moves[line-1][column-1] = 2;
        }
        if(board[line-1][column+1]%2 == 0 && board[line-1][column+1]>0 ){
            
            playable_moves[line-1][column+1] = 2;
        }


}


void next_move_black_rook(int line, int column){
    int tmp_line = line;
    int tmp_column = column;

    // right
        
        while (board[line][column+1]==0)
        {
            playable_moves[line][column+1] = 1;
            
            column++;
        }
        // case found is an opposite piece
        if (board[line][column+1]%2==1){
            playable_moves[line][column+1] = 2;
        }


        //left
        line = tmp_line;
        column= tmp_column;
        while (board[line][column-1]==0)
        {
            playable_moves[line][column-1] = 1;
            column--;
        }
        // case found is an opposite piece
        if (board[line][column-1]%2==1){
            playable_moves[column][column-1] = 2;
        }

        //up
        line = tmp_line;
        column = tmp_column;
        while (board[line -1][column]==0)
        {
            playable_moves[line -1][column] = 1;
            line--;
        }
        // case found is an opposite piece
        if (board[line-1][column]%2==1){
            playable_moves[line -1][column] = 2;
        }

        //down
        line = tmp_line;
        column = tmp_column;
        while (board[line +1][column]==0)
        {
            playable_moves[line +1][column] = 1;
            line++;
        }
        // case found is an opposite piece
        if (board[line+1][column]%2==1){
            playable_moves[line +1][column] = 2;
        }
}

void next_move_white_rook(int line, int column){
    int tmp_line = line;
    int tmp_column = column;

        // right
        while (board[line][column+1]==0)
        {
            playable_moves[line][column+1] = 1;
            
            column++;
        }
        // case found is an opposite piece
        if (board[line][column+1]%2==0 && board[line][column+1]>0){
            playable_moves[line][column+1] = 2;
        }


        //left
        line = tmp_line;
        column= tmp_column;
        while (board[line][column-1]==0)
        {
            playable_moves[line][column-1] = 1;
            column--;
        }
        // case found is an opposite piece
        if (board[line][column-1]%2==0 && board[line][column-1] >0){
            playable_moves[column][column-1] = 2;
        }

        //up
        line = tmp_line;
        column = tmp_column;
        while (board[line -1][column]==0)
        {
            playable_moves[line -1][column] = 1;
            line--;
        }
        // case found is an opposite piece
        if (board[line-1][column]%2==0 && board[line-1][column]>0){
            playable_moves[line -1][column] = 2;
        }
        
        //down
        line = tmp_line;
        column = tmp_column;
        while (board[line +1][column]==0)
        {
            playable_moves[line +1][column] = 1;
            line++;
        }
        // case found is an opposite piece
        if (board[line+1][column]%2==0 && board[line+1][column]>0){
            playable_moves[line +1][column] = 2;
        }

}

void next_move_black_bishop(int line, int column){
    int tmp_line = line;
    int tmp_column = column;
      
        //up & right
        while (board[line-1][column+1]==0)
        {
            playable_moves[line-1][column+1] = 1;
            line --;
            column++;
        }
        if(board[line-1][column+1]%2 == 1){
            playable_moves[line-1][column+1] = 2;
        }
        //up & left
        line = tmp_line;
        column = tmp_column; 
        while (board[line-1][column-1]==0)
        {
            playable_moves[line-1][column-1] = 1;
            line --;
            column--;
        }
        if(board[line-1][column-1]%2 == 1){
            playable_moves[line-1][column-1] = 2;
        }
        //down & right
        line = tmp_line;
        column = tmp_column;
        while (board[line+1][column+1]==0)
        {
            playable_moves[line+1][column+1] = 1;
            line ++;
            column++;
        }
        if(board[line+1][column+1]%2 == 1){
            playable_moves[line+1][column+1] = 2;
        }
        // down and left
        line = tmp_line;
        column = tmp_column;
        while (board[line+1][column-1]==0)
        {
            playable_moves[line+1][column-1] = 1;
            line ++;
            column--;
        }
        if(board[line+1][column-1]%2 == 1){
            playable_moves[line+1][column-1] = 2;
        }
}

void next_move_white_bishop(int line, int column){
    int tmp_line = line;
    int tmp_column = column;


        //up & right
        while (board[line-1][column+1]==0)
        {
            playable_moves[line-1][column+1] = 1;
            line --;
            column++;
        }
        if(board[line-1][column+1]%2 == 0 && board[line-1][column+1]>0){
            playable_moves[line-1][column+1] = 2;
        }
        //up & left
        line = tmp_line;
        column = tmp_column; 
        while (board[line-1][column-1]==0)
        {
            playable_moves[line-1][column-1] = 1;
            line --;
            column--;
        }
        if(board[line-1][column-1]%2 == 0 && board[line-1][column+1]>0){
            playable_moves[line-1][column-1] = 2;
        }
        //down & right
        line = tmp_line;
        column = tmp_column;
        while (board[line+1][column+1]==0)
        {
            playable_moves[line+1][column+1] = 1;
            line ++;
            column++;
        }
        if(board[line+1][column+1]%2 == 0 && board[line-1][column+1]>0){
            playable_moves[line+1][column+1] = 2;
        }
        // down and left
        line = tmp_line;
        column = tmp_column;
        while (board[line+1][column-1]==0)
        {
            playable_moves[line+1][column-1] = 1;
            line ++;
            column--;
        }
        if(board[line+1][column-1]%2 == 0 && board[line-1][column+1]>0){
            playable_moves[line+1][column-1] = 2;
        }

}

void next_move_black_knight(int line, int column){
    

        //1, starting up right
            if(board[line-2][column+1]==0){
                playable_moves[line-2][column+1]= 1;
            }
             if(board[line-2][column+1]%2 == 1){
                playable_moves[line-2][column+1]= 2;
            }
                //2
            if(board[line-1][column+2]==0){
                playable_moves[line-1][column+2]= 1;
            }
             if(board[line-1][column+2]%2 == 1){
                playable_moves[line-1][column+2]= 2;
            }
                //3
             if(board[line+1][column+2]==0){
                playable_moves[line+1][column+2]= 1;
            }
             if(board[line+1][column+2]%2 == 1){
                playable_moves[line+1][column+2]= 2;
            }
                //4
             if(board[line+2][column+1]==0){
                playable_moves[line+2][column+1]= 1;
            }
             if(board[line+2][column+1]%2 == 1){
                playable_moves[line+2][column+1]= 2;
            }
            //5

              if(board[line+2][column-1]==0){
                playable_moves[line+2][column-1]= 1;
            }
             if(board[line+2][column-1]%2 == 1){
                playable_moves[line+2][column-1]= 2;
            }
            //6 
             if(board[line+1][column-2]==0){
                playable_moves[line+1][column-2]= 1;
            }
             if(board[line+1][column-2]%2 == 1){
                playable_moves[line+1][column-2]= 2;
            }

            //7
            if(board[line-1][column-2]==0){
                playable_moves[line-1][column-2]= 1;
            }
             if(board[line-1][column-2]%2 == 1){
                playable_moves[line-1][column-2]= 2;
            }
            //8
            if(board[line-2][column-1]==0){
                playable_moves[line-2][column-1]= 1;
            }
             if(board[line-2][column-1]%2 == 1){
                playable_moves[line-2][column-1]= 2;
            }
}

void next_move_white_knight(int line, int column){

                //1, starting up right
            if(board[line-2][column+1]==0){
                playable_moves[line-2][column+1]= 1;
            }
             if(board[line-2][column+1]%2 == 0 && board[line-2][column+1]>0){
                playable_moves[line-2][column+1]= 2;
            }
                //2
            if(board[line-1][column+2]==0){
                playable_moves[line-1][column+2]= 1;
            }
             if(board[line-1][column+2]%2 == 0 && board[line-1][column+2]>0){
                playable_moves[line-1][column+2]= 2;
            }
                //3
             if(board[line+1][column+2]==0){
                playable_moves[line+1][column+2]= 1;
            }
             if(board[line+1][column+2]%2 == 0 && board[line+1][column+2]>0){
                playable_moves[line+1][column+2]= 2;
            }
                //4
             if(board[line+2][column+1]==0){
                playable_moves[line+2][column+1]= 1;
            }
             if(board[line+2][column+1]%2 == 0 && board[line+2][column+1]>0){
                playable_moves[line+2][column+1]= 2;
            }
            //5

              if(board[line+2][column-1]==0){
                playable_moves[line+2][column-1]= 1;
            }
             if(board[line+2][column-1]%2 == 0 && board[line+2][column-1]>0){
                playable_moves[line+2][column-1]= 2;
            }
            //6 
             if(board[line+1][column-2]==0){
                playable_moves[line+1][column-2]= 1;
            }
             if(board[line+1][column-2]%2 == 0 && board[line+1][column-2]>0){
                playable_moves[line+1][column-2]= 2;
            }

            //7
            if(board[line-1][column-2]==0){
                playable_moves[line-1][column-2]= 1;
            }
             if(board[line-1][column-2]%2 == 0 && board[line-1][column-2]>0){
                playable_moves[line-1][column-2]= 2;

               
            }
            //8
            if(board[line-2][column-1]==0){
                playable_moves[line-2][column-1]= 1;
            }
             if(board[line-2][column-1]%2 == 0 && board[line-2][column-1]>0){
                 
                playable_moves[line-2][column-1]= 2;
            }
}

void next_move_black_king(int line, int column){
    if (board[line+1][column] == 0 ){
        playable_moves[line+1][column] = 1;
    }
    else if (board[line+1][column]%2==1){
        playable_moves[line+1][column] = 2;
    }
     if (board[line-1][column] == 0 ){
        playable_moves[line-1][column] = 1;
    }
    else if (board[line-1][column+1]%2==1){
        playable_moves[line-1][column+1] = 2;
    }
    if (board[line][column+1] == 0 ){
        playable_moves[line][column+1] = 1;
    }
    else if (board[line][column+1]%2==1){
        playable_moves[line][column+1] = 2;
    }
    if (board[line][column-1] == 0 ){
        playable_moves[line][column-1] = 1;
    }
    else if (board[line][column-1]%2==1){
        playable_moves[line][column-1] = 2;
    }

    if (board[line-1][column-1] == 0 ){
        playable_moves[line-1][column-1] = 1;
    }
    else if (board[line-1][column-1]%2==1){
        playable_moves[line-1][column-1] = 2;
    }if (board[line-1][column+1] == 0 ){
        playable_moves[line-1][column+1] = 1;
    }
    else if (board[line-1][column+1]%2==1){
        playable_moves[line-1][column+1] = 2;
    }

     if (board[line+1][column-1] == 0 ){
        playable_moves[line+1][column-1] = 1;
    }
    else if (board[line+1][column-1]%2==1){
        playable_moves[line+1][column-1] = 2;
    } if (board[line+1][column+1] == 0 ){
        playable_moves[line+1][column+1] = 1;
    }
    else if (board[line+1][column+1]%2==1){
        playable_moves[line+1][column+1] = 2;
    }

    

}

void next_move_white_king(int line, int column){
    if (board[line+1][column] == 0 ){
        playable_moves[line+1][column] = 1;
    }
    else if (board[line+1][column]%2==0 && board[line+1][column]>0){
        playable_moves[line+1][column] = 2;
    }
     if (board[line-1][column] == 0 ){
        playable_moves[line-1][column] = 1;
    }
    else if (board[line-1][column]%2==0 && board[line-1][column]>0){
        playable_moves[line-1][column] = 2;
    }

    else if (board[line-1][column+1]%2==0 && board[line-1][column+1]>0){
        playable_moves[line-1][column+1] = 2;
    }
    if (board[line][column+1] == 0 ){
        playable_moves[line][column+1] = 1;
    }
    else if (board[line][column+1]%2==0 && board[line][column+1]>0){
        playable_moves[line][column+1] = 2;
    }
    if (board[line][column-1] == 0 ){
        playable_moves[line][column-1] = 1;
    }
    else if (board[line][column-1]%2==0 && board[line][column-1]>0){
        playable_moves[line][column-1] = 2;
    }

    if (board[line-1][column-1] == 0 ){
        playable_moves[line-1][column-1] = 1;
    }
    else if (board[line-1][column-1]%2==0 && board[line-1][column-1]>0){
        playable_moves[line-1][column-1] = 2;
    }if (board[line-1][column+1] == 0 ){
        playable_moves[line-1][column+1] = 1;
    }
    else if (board[line-1][column+1]%2==0 && board[line-1][column+1]>0){
        playable_moves[line-1][column+1] = 2;
    }

     if (board[line+1][column-1] == 0 ){
        playable_moves[line+1][column-1] = 1;
    }
    else if (board[line+1][column-1]%2==0 && board[line+1][column-1]>0){
        playable_moves[line+1][column-1] = 2;
    } if (board[line+1][column+1] == 0 ){
        playable_moves[line+1][column+1] = 1;
    }
    else if (board[line+1][column+1]%2==0 && board[line+1][column+1]>0){
        playable_moves[line+1][column+1] = 2;
    }

}


void next_move(int line, int column){

    line = line +2;
    column = column +2;

    
    
    // now, first line up = 2
    //now, black pawns line = 3
    // now, white pawns line = 8
    // now, last line = 9

    memset(playable_moves, 0, 144*sizeof(int));
    // displaying current piece
    playable_moves[line][column] = 8;
    
    switch (board[line][column])
    {
        
        //black pawn
        case 2:
            
            next_move_black_pawn(line,column);
        break;


        // white pawn
        case 3:
        next_move_white_pawn(line,column);
               
        break;

        // black rook
        case 4: 
        next_move_black_rook(line, column);            
            break;

        //white rook
        case 5: 
        next_move_white_rook(line, column);
            break;

        //black knight
        case 6: 
        next_move_black_knight(line,column);
            break;

        // white knigth
        case 7: 
        next_move_white_knight(line,column);
            

            break;

        //black bishop
        case 8:
        next_move_black_bishop(line,column);
            break;


        //white bishop
        case 9: 
        next_move_white_bishop(line,column);
            break;

        //black queen
        case 10:
        next_move_black_bishop(line,column);
        next_move_black_rook(line,column);
            break;
        
        //white queen
        case 11:
            next_move_white_bishop(line,column);
            next_move_white_rook(line,column);
            break;

        //black king
        case 12:
            next_move_black_king(line,column);
            break;      

        case 13:
            next_move_white_king(line,column);
            break;
        
    default:
        break;
    }

    

}


void print_next_move(){
    for (int i = 2; i < 10; i++)
    {
        for (int j = 2; j < 10; j++)
        {
            printf("%d ", playable_moves[i][j]);
        }
        printf("\n");
    }
    
}

void play_next_move_local(){
    int line, column, next_line, next_column;

    while(1){
            printf("ligne de la pièce que vous voulez jouer:");
            scanf("%d", &line);
             printf("column de la pièce que vous voulez jouer:");
            scanf("%d", &column);
             printf("ligne de la case où vous placez votre pièce:");
            scanf("%d", &next_line);
             printf("column de la case où vous placez votre pièce:");
            scanf("%d", &next_column);
             

    next_move(line,column);
    print_next_move();
    printf("%d\n", playable_moves[next_line+2][next_column+2]);
    if(playable_moves[next_line+2][next_column+2] == 1 || playable_moves[next_line+2][next_column+2] == 2 ){
        printf("coup possible\n");
        board[next_line+2][next_column+2]=board[line+2][column+2];

        board[line+2][column+2] = 0;
    }
    else {
        printf("coup impossible\n");
    }
    print_board();
    
    
    }


}

void play_next_move(char * params){
      
  char d[] = ",";
  char e[] = " ";

  char *from = strtok(params, d);
    char *to = strtok(NULL,e);
    printf("pos1 = %s, pos2 = %s\n",from,to);

    int line_from, column_from; 
    int line_to, column_to;

    switch ((char)from[0])
    {
    case 'a':
        column_from = 0;
        break;

    case 'b':
        column_from = 1;
        break;

    case 'c':
        column_from = 2;
        break;

    case 'd':
        column_from = 3;
        break;

    case 'e':
        column_from = 4;
        break;

    case 'f':
        column_from = 5;
        break;

    case 'g':
        column_from = 6;
        break;

    case 'h':
        column_from = 7;
        break;
    
    default:
        break;
    }

    switch ((char)from[1])
    {
    case '1':
        line_from = 7;
        break;

    case '2':
        line_from = 6;
        break;

    case '3':
        line_from = 5;
        break;

    case '4':
        line_from = 4;
        break;

    case '5':
        line_from = 3;
        break;

    case '6':
        line_from = 2;
        break;

    case '7':
        line_from = 1;
        break;

    case '8':
        line_from = 0;
        break;
    
    default:
        break;
    }

    switch ((char)to[0])
    {
    case 'a':
        column_to = 0;
        break;

    case 'b':
        column_to = 1;
        break;

    case 'c':
        column_to = 2;
        break;

    case 'd':
        column_to = 3;
        break;

    case 'e':
        column_to = 4;
        break;

    case 'f':
        column_to = 5;
        break;

    case 'g':
        column_to = 6;
        break;

    case 'h':
        column_to = 7;
        break;
    
    default:
        break;
    }

    switch ((char)to[1])
    {
    case '1':
        line_to = 7;
        break;

    case '2':
        line_to = 6;
        break;

    case '3':
        line_to = 5;
        break;

    case '4':
        line_to = 4;
        break;

    case '5':
        line_to = 3;
        break;

    case '6':
        line_to = 2;
        break;

    case '7':
        line_to = 1;
        break;

    case '8':
        line_to = 0;
        break;
    
    default:
        break;
    }

    printf("line from = %d column from = %d line to = %d column to = %d \n",line_from,column_from,line_to,column_to);

    
    //on calcule les coups possibles de la pièce sélectionnée
    next_move(line_from,column_from);
    print_next_move();
    if(playable_moves[line_to+2][column_to+2] == 1 || playable_moves[line_to+2][column_to+2] == 2 ){
        // printf("coup possible\n");
        board[line_to+2][column_to+2]=board[line_from+2][column_from+2];
        board[line_from+2][column_from+2] = 0;
        print_board();
    }
    else {
        printf("coup impossible\n");
    }

    
}




void * client_thread_handler ( void * threadArg)
{
 
	threadArgsType * myThreadArgs = ( threadArgsType * ) threadArg ;
	int * totalClientNumber = & ( myThreadArgs->totalClientNumber ) ;
	clientAttrType  * descriptorList = myThreadArgs->descriptorList ;
	int clientSock = myThreadArgs->descriptorList [ ( * totalClientNumber ) - 1 ].sock ;
	
	char message [ MYMSGLEN ] ;
	int length ;
	int index ;
	int ret ;
	
        printf("x.color = %d\n",descriptorList[0].color);
        if(descriptorList[0].color == 0){

		descriptorList[0].to_play = true;
        descriptorList[1].to_play = false;
        }
        else {
            descriptorList[0].to_play = false;
            descriptorList[1].to_play = true;
        }

	printf (" clientSock number %d \n", clientSock ) ;
	while ( 1 ) 
		{
		memset ( message, 0, MYMSGLEN );
		length = recv ( clientSock, message, MYMSGLEN, 0 ) ;
		//printf (" length value %d \n ", length ) ;
		
			
		printf("client sock = %d\n", clientSock);
		if(descriptorList[0].to_play && clientSock == 4){
		  play_next_move(message);
		  descriptorList[0].to_play = false;
		  descriptorList[1].to_play = true;
			
		}
		else if (descriptorList[1].to_play && clientSock == 5){
			play_next_move(message);
			descriptorList[0].to_play = true;
		  descriptorList[1].to_play = false;
		}
		else{
			printf("not your turn !\n");
		}
			

		  if ( ( length == -1 ) || ( length == 0 ) )
			{
			  perror ( "Client recv" ) ;
			  close ( clientSock ) ;
			  
			  index = find_index_desc_from_list ( descriptorList, clientSock,  (* totalClientNumber ) ) ;
			  if ( index >= 0 )
				remove_index_desc_from_list ( descriptorList, index,  totalClientNumber ) ;
	  
			  pthread_exit ( &ret ) ; 
			  //close this soccket and exit thread
			}

		forward_message ( message, length, descriptorList,  totalClientNumber ) ;
    
		}
		
}



int process_new_connection ( int sockDesc, struct sockaddr_in * from, threadArgsType * myThreadArgs )
{
	//char  firstMsg [ MYMSGLEN ] ;
	// int len ;
	int code ;
	pthread_t *threadId;
	clientAttrType *descriptorList = myThreadArgs->descriptorList ;
	int * totalClientNumber = & myThreadArgs->totalClientNumber ;
	clientAttrType * myClientAttr = & descriptorList [ ( *totalClientNumber ) ] ;
	
	printf ( "-------------------------------------------------------------------------------------------------\n" ) ;
	printf ( " The peer address of the new client bound to the new created socket --> %s:%d \n" , inet_ntoa ( from->sin_addr ), ntohs ( from->sin_port )  ) ;
	printf ( "-------------------------------------------------------------------------------------------------\n" ) ;
	/*  Pseudonyms handling -- Optional 	
	memset ( firstMsg, 0, MYMSGLEN ) ;
  
	length = recv ( sockDesc, firstMsg, MYMSGLEN, 0 ) ;
  
	if ( length == -1 )
		{
		  perror( "recv" ) ;
		  return ( -1 ) ;
		}
  
	if ( 0 != memcmp ( firstMsg, "READER#",  6 ) )
	{
	  if ( ( * totalReaderNumber ) > MAXNUMDSC )
		{
		  printf ( "Maximum allowed reader number reached \n" ) ; 
		  //send ( sockDesc, ) ;
		  close ( sockDesc ) ;
		  return ( -1 ) ;
		}
  */
	  myClientAttr->sock = sockDesc ;
	  myClientAttr->addr.sin_family = from->sin_family ;
	  myClientAttr->addr.sin_port = from->sin_port ;
	  myClientAttr->addr.sin_addr.s_addr = from->sin_addr.s_addr ;
	 
	  ( * totalClientNumber ) ++ ;
	  threadId = & myClientAttr->threadId ; 
	
	  code = pthread_create ( threadId,  0, client_thread_handler, ( void * ) myThreadArgs ) ;

	  if ( code != 0 )
	      {
		perror ( "Thread creation" ) ;
		//close ( sockDesc ) ; //Closing all sockets ? 		
		return ( -1 ) ;
	      }
	    
	  
	
	
	
  return ( 0 ) ;
}

int naming ( )
{
	// Socket creation.
	int sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock == -1 ) {
		perror ( "socket" ) ;
		exit ( 1 ) ;
	}

	// Reuse the same port (useful when developing...).
	int code, enable = 1;
	code = setsockopt ( sock, SOL_SOCKET, SO_REUSEADDR, & enable, sizeof ( int ) ) ;
	if (code == -1 ) {
		perror ( "setsockopt" ) ;
		exit ( 1 ) ;
	}

	// Initialisation of the sockaddr_in data structure.
	struct sockaddr_in addr ;
	memset ( & addr, 0, sizeof ( struct sockaddr_in ) ) ;
	addr . sin_family = AF_INET ;
	addr . sin_port = htons ( SERVERPORT ) ;
	addr . sin_addr . s_addr = inet_addr ( LOCALHOST ) ;

	// Name the socket.
	code = bind ( sock, ( struct sockaddr * ) & addr, sizeof ( struct sockaddr_in ) ) ;
	if ( code == -1 ) {
		perror ( "bind" ) ;
		exit ( 1 ) ;
	}

	// Return the focket ID.
	return sock ;
}







int main ( int argc, char * argv [ ] )
{
	
	threadArgsType myThreadArgs ;
	// Name the socket.
	int sockDesc ;
	
	myThreadArgs.totalClientNumber = 0 ;
	
	sockDesc = naming ( ) ;

	// Set up listening on the port.
	int code ;
	code = listen ( sockDesc, 5 ) ;
	if ( code == -1 ) {
		perror ( "listen" ) ;
		exit ( 1 ) ;
	}

	// Wait for incoming connections and deal with the clients.
	int newSockDesc ;
	socklen_t len ;
	struct sockaddr_in addr ;
	printf ( "-------------------------------------------------------------------------------------------------\n" ) ;
	printf ( " Welcome to NET4102 chat server \n" ) ;
	printf ( "-------------------------------------------------------------------------------------------------\n" ) ;
	for ( ; ; ) {
		len = sizeof ( struct sockaddr_in ) ;
		newSockDesc = accept ( sockDesc, ( struct sockaddr * ) & addr, & len ) ;
		if ( newSockDesc == -1 ) {
			perror ( "accept" ) ;
			close_descriptor_list ( myThreadArgs.descriptorList,  myThreadArgs.totalClientNumber ) ;
			close ( sockDesc ) ;
			exit ( 1 ) ;
		}
		if ( myThreadArgs.totalClientNumber < MAXNUMDSC ) {
		  if ( process_new_connection ( newSockDesc, & addr, & myThreadArgs ) == -1 ) {
		    close_descriptor_list ( myThreadArgs.descriptorList,  myThreadArgs.totalClientNumber ) ;
		    close ( sockDesc ) ;
		    exit ( 1 ) ;

		  }
		}
		else
		{
		  printf ( "Warning: Maximum client number reached " ) ;
		  close ( newSockDesc ) ;
		}
		
	}

	// This point in the program will never be reached.
	return ( 0 ) ;
}