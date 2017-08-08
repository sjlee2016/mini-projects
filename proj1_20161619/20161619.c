#include "a.h"
void beforeWord(int*i,int*j,char**lines);//전에 단어를 찾아주는 함수
void nextWord(int*i,int*j,char**lines);//다음 단어를 찾아주는 함수
void save(char* filename, char**lines);// lines의  내용을 파일에  저장해주는 함수
void Enter(int*i,int*j,char**lines);//lines의 내용을 커서 기준으로 엔터를 한 상태로 만들어주는 함수
int find(int*i,int*j,char**lines,char*exco,int length);// /search를 구현하는 함수. 찾지못하몄으면 -1, 찾았으면 1을 돌려준다. 
void backSpace(int* i,int*j,char**lines); // backspace 함수
void Space(int* i,int* j, char** lines); //space함수 
int main(int argc,char *argv[]) {
    char *filename = argv[1]; //argv[1] : edit filename
    FILE * fin,*fout, *vim; //fin는 받아오는 파일 fout은 끝에 저장해줄  파일 
    int i,j,x,y,z,k,N,E,X,check,h,flag,num,tabstop,vim_found;
    char c; // 프로그램에서 사용자가 입력하거나 파일에서 불러오는 char은 모두 c 로 정해둠
    char *entered,*exco,*co,*order; // entered는  excommand모드에서 사용자가 입력해준 char로 구성될 배열(이미 사이즈가 20으로 정해짐). exco는이 entered 이라는 배열을 사용자가
    //엔터 쳐줄때까지만의 내용을 가지고있음. 즉, 끝에 '\0'이 들어가지 않음.
    char **copied,**vim_info; //이 2차원 char 배열은 excommand모드에서 :q 라고 쳤을때 수정한부분이 있는지 알아보기 위해 사용됨. 이 배열은 처음 시작할때 lines와 똑같은 내용을 가지고 있음. 또한 excommand모드에서 w를 해주면 이 값이 그 당시에 lines의 내용으로 바뀜. 그러므로 :q 를 해줬을때 copied와  lines의 다른점이 있으면 종료가 안되게함. 
        //vim_info 는 .vimrc 파일의 내용을 담아줄 배열이다.
    
    vim_found=0;
    check=0;
    tabstop=3;
    h=0;
    flag=0;
    co=(char*)malloc(sizeof(char)*20);
    entered=(char*)malloc(sizeof(char)*20);
    copied=(char**)malloc(sizeof(char*)*33); 
    vim_info=(char**)malloc(sizeof(char*)*33); 
    for(i=0;i<33;i++){
	copied[i]=(char*)malloc(sizeof(char)*88);
	vim_info[i]=(char*)malloc(sizeof(char)*88);
	for(j=0;j<88;j++)
	{
	    copied[i][j]='\0';
	    vim_info[i][j]='\0';
	}
    }
    
    for(x=0;x<20;x++)
    {
       entered[x]='\0';
       co[x]='\0';
    }

    //앞에서 설명했던 배열들을 동적할당으로 선언해주고 안에는 '\0'로 채워줌  
    flag=0;
    int mode=0; //mode 0=command 1=edit 2= quit
    fin=fopen(filename,"r");
    vim=fopen(".vimrc","r");
    
   char **lines = (char **)malloc(sizeof(char*)*33);
     for ( x = 0 ; x < 33 ; x++ ) {
	        lines[x] = (char*)malloc(sizeof(char)*88);
	               for ( y = 0 ; y < 88 ; y++ )
		               lines[x][y] = '\0';	   
     }
     if(vim!=NULL)
    { 
         i=0;
         j=0;
	for(x=0;x<HEIGHT;x++)
	{
	    for(y=0;y<WIDTH;y++)
	    {
		c=fgetc(vim);
		if(c!=EOF&&c!='\n'&&c!='\t'&&c!='\b'&&c!='\a'&&c!='\r')
		{
		    vim_info[x][y]=c;
		}else if(c==ENTER)
		{
		    break;
		}
	    }
	    
	}
            order="set nu"; //find 함수를 통해 vimrc에 set nu가적혀있는지 확인한다. 있다면 vim_found 는 1이 되고 없다면 -1이 된다.
	    vim_found=find(&i,&j,vim_info,order,6);
    if(vim_found==1) 
    {
	lines[HEIGHT][WIDTH]='1'; //프로그램 다른 부분에서 lines는 HEIGHT-1 과 WIDTH-1 이상으론 값을 번경시키지 않는다. 또한 lines는 처음에 [33]x[88]로 선언되었기 때문에 lines[HEIGHT][WIDTH] 를 사용하여도 segmentation error 가 뜨지않기 때문에 문제가 생기지 않는다. 그렇기 때문에 vimrc 에 set nu 가 있거나 나중에 excommand 에서 사용자가 :set nu를 해주면 이 자리에 '1'을 넣어줄것이다. 이를 통해 draw함수는 lines[HEIGHT][WIDTH]의 값을 체크하고 '1' 이면 줄 넘버를 출력하고 '\0'면 그냥 원래처럼 프린트 할것이다.
    }
    }
    /*
     * initializing editor start 
     */
    
    if(fin!=NULL)//filename과 동일한 이름을 가진 파일이 있을 경우에만 lines에 그 파일에 내용을 넣어줌
    {
	for(i=0;i<HEIGHT;i++)
	{
	    for(j=0;j<WIDTH;j++)
	    {
		c=fgetc(fin);
		if(c!=EOF&&c!='\n'&&c!='\t'&&c!='\b'&&c!='\a'&&c!='\r')
		{
		lines[i][j]=c;
		copied[i][j]=c; //설명했드시 나중에 :q에서 수정한 부분이 있는지 판단해주기 위해 카피를 만듬
		}else if(c==ENTER)
		    break;
	    }
	}
    }
    	
    /*
     * initializing editor end
     */

    /*
     * inf loop start
     */
    i=0;
    j=0;
    N=true;
    E=false;
    X=false; //여기서 N,E,X는 각자 normal mode, edit mode, excommand mode 를 뜻함. 이를 통해 프로그램에서 어떤조건일때  어떤 모드인지 조절가능하게함.
    while ( true ) {
        draw(lines,i,j);  // draw editor, 0,0 : cursor position
	printf("[NORMAL] > ./%s > [CURSOR]%d:%d\n",filename,i,j);
	printf("--COMMAND--");
        char c = getch(); // wait input character
	
	    while(N==true) //Normal mode일때
	    {
              draw(lines,i,j);
	      printf("[NORMAL] > ./%s > [CURSOR]%d:%d\n",filename,i,j);
              printf("--COMMAND--");
	      c=getch();
	      if(c==UP||c=='k') //입력 받은 키가 위 방향키거나 k일때 
	      {
		  if(i!=0) //커서가 맨윗줄이면 이 기능이 돌아가지 않게 만듬
		  {
		      for(x=WIDTH-1;x>=0;x--) //커서 윗줄에 있는 마지막 원소(널이 아닌)를 찾음
		      {
			  if(lines[i-1][x]!='\0')
			  {
			      break;
			  }
		      }
		      if(x<j)//윗줄의 마지막원소의 자리가 커서보다 더왼쪽이면 마지막원소의 자리로 가준다. 그렇지 않으면 그냥 커서의 자리에서 윗칸으로 간다 
		      {
		      j=x;
		      i--;
		      }else
		      {
			  i--;
		      }
		  }
	      }else if(c==DOWN||c=='j')//방향키 아래나 j가 입력됬을때 
	      { 
		  if(i<HEIGHT-1)//커서가 젤 마지막줄에 있을땐 아래방향키가 안먹히게 한다
		  { 
		      for(y=WIDTH-1;y>=0;y--) //커서 아랫줄의 마지막원소 자리를 찾는다
		      { 
			  if(lines[i+1][y]!='\0')
			  {
			      break;
			  }
		      }
		      if(y<0) //밑줄에 아무것도 없다면 아랫방향키가 안먹힌다 
                      { }
			  else if(y<j) //밑줄의 마지막원소 자리가 지금커서 자리보다 왼쪽에 있다면 그 마지막 원소자리로 가게한다. 그렇지 않다면 그냥 커서를 아래로 내려준다.
		      {
			j=y;
		        i++;
		      }else
		      {
			  i++;
		      }
		 }

		     
	      } 
	      else if((c==LEFT)||(c=='h'))//사용자가 왼쪽 방향키나 h 를 입력했을때 
	      {
		  if(j>0) //지금 커서가 젤 왼쪽에 위치한다면(즉 j==0) 왼쪽으로 움직이지 않게한다
		  {
		      j--;
		  }
		}
	      else if((c==RIGHT)||(c=='l'))//사용자가 오른쪽 방향키나 l을 입력했을때
	      {
		  if(j<WIDTH-1&&lines[i][j+1]!='\0')//지금 커서가 젤 오른쪽이 아닐때, 그리고 커서 바로 오른쪽에 원소가 없을때가 아닐때만 커서를 오른쪽으로 한칸 옮긴다
		  {
		      j++;
		  }
	      
	    }else if(c=='i')
	    {
		E=true; // Normal mode에서 Edit mode로 변환된다
		N=false;
		
	    }else if(c=='o')
	    {
		if(i<HEIGHT-1)//커서가 젤 아래쪽에 있지않을경우에 커서를 다음줄을 개행하고 edit mode로 변환해준다 
		{   j=0;
		    i=i+1;
		    Enter(&i,&j,lines);
		    E=true;
		    N=false;
		}
	    }else if(c=='d')
	    {
		c=getch();
		if(c=='d') //dd를 입력해줄경우 그 줄에 내용을 삭제한다. 
		{  for(y=0;y<j;y++)
		    {
			lines[i][y]='\0';
		    }

		   for(y=i;y<HEIGHT-1;y++) //그 줄 다음 줄 내용을 한 줄앞으로 옮겨준다 
		   {		       
		    for(x=WIDTH-1;x>=0;x--)
		    {
			lines[y][x]=lines[y+1][x];
		    }
		   }
		   for(y=0;y<WIDTH-1;y++)
		       lines[HEIGHT-1][y]='\0';//마지막줄의 내용을 없앤다
		
              for(y=0;y<WIDTH;y++)
	      {
		  if(lines[i][y]!='\0'&&lines[i][y]!=' ') //원래 커서가 있던  줄에 처음 원소가 있는 위치로 간다. 
		  {
		      j=y;
		      break;
		  }
	      }
		}
	   }else if(c=='0')//0을 입력해주면 커서가 젤 왼쪽으로 가게 한다.
	    {
             j=0;
	    }else if(c=='^')//라인의 첫 단어의 첫 문자에 커서를 위치한다
	    {
		for(x=0;x<WIDTH;x++)
		{
		    if(lines[i][x]!='\0'&&lines[i][x]!=' ')
		    {
		        j=x;
			break;
		    }
		}
		
	    }else if(c=='$')//라인의 끝단어의 끝문자에 커서를 위치한다
	    {
		for(x=WIDTH-1;x>=0;x--)
		{
		    if(lines[i][x]!='\0'&&lines[i][x]!=' ')
		    {
			j=x;
			break;
		    }
		} 
	    }else if(c=='w')//다음단어를 찾아 커서를 위치한다
	    {
		  nextWord(&i,&j,lines);//전 단어를 찾아 커서를 위치한다
	    }else if(c=='b')
	    {	
                 beforeWord(&i,&j,lines);		
	    }else if(c==':')//normal mode에서 excommand모드로 변환한다
	    {
		N=false; 
		X=true;
		break;
	    }else if(c=='/')
	    {
		for(x=0;x<20;x++)
		   co[x]='\0';
		printf("\n/");
		flag=1;
		co[0]='/';
		while(c!=ENTER)
		{ 
		    c=getch();
		    if(c==BACKSPACE&&flag==1)
		    {
			N=false;
			E=true;
			break;
		    }else if(c==BACKSPACE)
		    {
			co[flag]='\0';
			flag--;
		    }else if(c==ENTER)
		    {
			break;
			
		    }else
		    {
			co[flag]=c;
			flag++;
		    }
		    printf("%c",c);
		}
		
		find(&i,&j,lines,co,flag-1);
		N=false;
		E=true;
		break;
	    }
		
	    }
	    
	    while(E==true)//edit mode
	    {   
		draw(lines,i,j);
		printf("[INSERT] > ./%s > [CURSOR]%d:%d\n",filename,i,j);
		printf("--INSERT--");
                c=getch();	
		if(c==ESC)//사용자가 ESC를 눌러주면 edit 모드에서 normal mode로 변환한다
		{
		    N=true;
		    E=false;
		}else if(c==' ') //사용자가 스페이스를 입력하면 
		{ 
                  Space(&i,&j,lines);
		}	
		else if(c==BACKSPACE)//사용자가 백스페이스를 입력하면
		 {
		     backSpace(&i,&j,lines);
	         }
		else if(c==ENTER)//사용자가 엔터를 입력하면
		{
		   Enter(&i,&j,lines); 
		}
		else if(c==TAB)//사용자가 탭을 입력하면 SPACE 함수를 TABSTOP만큼  불러준다. 여기서 TABSTOP은 처음에는 3이지만 excommand에서 :set tabstop=4 나 :set expandtab 를 해주면 4로 늘어난다.
		{	
		  if(!(i==HEIGHT-1&&j==WIDTH-4))
			  {
				for(x=0;x<tabstop;x++)
				   Space(&i,&j,lines);
			  }	  
		}else  
		{
		    if(j!=WIDTH-1) //아래 컨디션을 모두 만족하지 않았다면 사용자가 입력해준것을  lines에 넣어준다. 
		    {
		    for(x=WIDTH-2;x>j;x--)//커서 뒤에 문장을 한칸 뒤로 당겨준다
		    {
			lines[i][x]=lines[i][x-1];
		    }
		   
		    lines[i][j]=c; //커서위치에 입력된 문자를 입력한다
		    j++; //커서를 왼쪽으로 옮긴다 
		    draw(lines,i,j);
		}
		}
    }
       check=0; //처음에 excommand 의 상태
       while(X==true)//excommand mode 
       {   for(x=0;x<20;x++)
	   {
	      entered[x]='\0';
	   }
	    
	   flag=0;//excommand에서 받는 명령어의 문자 갯수 
	   draw(lines,i,j);
	   printf("[NORMAL] > ./%s > [CURSOR]%d:%d \n", filename,i,j);
	   switch(check)
	   {
	       case 1: printf("System> No write since last change(add ! to override)\n:"); break; //excommand에서 :q 를 입력했는데 수정을 해준뒤 저장을 안했을때 warning을 출력해준다
	       case 2: printf("System> saved \n:"); break; // excommand에서 :w 를 입력했을때 이 문구를 출력한다.
	       default : printf(":"); break; 
	    

	   }


	   c=getch();
	   while(c!=ENTER)//사용자가 엔터를 쳐줄때까지 입력을 받는다 
	   {
	     c=getch();
	     if(c==ENTER)//ENTER를 처주면 while loop를 바로 벗어난다
		 break;
	     if(c==BACKSPACE&&flag==0) //사용자가 BACKSPACE를입력하고 받은 명령어의 문자갯수가 0이면  excommand mode에서 normal mode로 변환한다
	     {
		 X=false;
		 N=true;
		 break;
	     }else if(c==BACKSPACE) //사용자가 BACKSPACE를 입력하면 바로 전에 받은 원소를 지워준다. 그러므로,  flag(명령어의 문자갯수)도 하나 줄여준다
	     {
		 entered[flag]=' ';
		 flag--;
	     }else 
	     {
		 entered[flag]=c; // 사용자가 BACKSPACE,ENTER가 아닌 다른 문자를 입력했다면 이를 명령어가 저장되있는 entered 이라는 배열에 저장한다. flag도 하나 늘려준다
		 flag++;
	     }
	     printf("%c",c); //받은 문자를 출력한다 
	   }

	   exco=(char*)malloc(sizeof(char)*flag);//받은 명령어의 문자 갯수만큼 exco 라는 문자 배열을 동적선언한다

	   for(x=0;x<flag;x++)
	   {
	       exco[x]=entered[x]; //exco 문자 배열에 entered 문자배열의 내용을 복사한다. (그 대신 exco 배열은 
	   }
           if(exco[0]=='w'&&flag==1)//명령어가 w 라면 lines 내용을 파일에 저장해준다. 
	   {   
	     save(filename,lines);  
	       for(x=0;x<HEIGHT;x++)
		   for(y=0;y<WIDTH;y++)
		   {
		       copied[x][y]=lines[x][y];//copied배열에 다시 저장하여 나중에 :q 를 해줬을때 수정한 부분이 있는지 확인가능하게 한다. 
		   }
	       check=2; //저장을 해줬다는 sign을 프린트하게 한다. 
	       
	       
	   }else if(exco[0]=='w'&&exco[1]=='q')//wq를 입력하면 저장해주고 프로그램을 종료한다.
	   {
	       save(filename,lines);
	       printf("\n"); 
	       return 0;

	   }else if(exco[0]=='q'&&flag==1)//q를 입력했을때 만약 수정한 부분이 있다면 종료가 안되게 한다.  없다면 종료한다.
	   {   h=0;
	       check=0;
	       for(x=0;x<HEIGHT;x++)
		   for(y=0;y<WIDTH;y++)
		   {   
		       if(copied[x][y]!=lines[x][y])
		       {
			  check=1;
			  h++;
			  break;
		       }
		   }
	        if(h==0)
		{
		    
	       printf("\n");
		return 0;
	       }
	   }
	   else if(exco[0]=='q'&&exco[1]=='!') //q!를 입력하면 강제종료한다.
	   {
	       printf("\n");
	       return 0;

	   }else if(strcmp(exco,"set nu")==0) //set nu 를 하면 lines[HEIGHT][WIDTH]='1'로 만든다(이 배열원소는 실제 출력되는 lines의 내용물은 영향을 주지 않기때문에 사용하였다. 이를 통해 draw함수에서 lines[HEIGHT][WIDTH]='1'이면 줄 넘버를 출력하게 해주었다. 그후에 바로 excommand모드에서 edit mode로 변환된다.  
	   {
	       lines[HEIGHT][WIDTH]='1';
		X=false;
 		N=true;
		break;
	       
	   }else if(strcmp(exco,"set nonu")==0)
	   {
	       lines[HEIGHT][WIDTH]='\0';
	       X=false;
		N=true;
		break;
	   }else if((strcmp(exco,"set expandtab")==0)||(strcmp(exco,"set tabstop=4")==0)) //set expandtab 이나 set tabstop=4를 해주면 tab이 스페이스 3번이 아닌 4번이 된다. 이를 변경해주고 바로 excommand mode에서 edit 모드로 변환한다.
	   {
	      tabstop=4;
	      X=false;
	      N=true;
	      break;
	     } else if(exco[0]>='0'&&exco[0]<='9'&&(exco[1]>='0'&&exco[1]<='9'||exco[1]=='\0'))// 명령어에서 숫자를 입력했을때
	   {
	        if(exco[1]=='\0') //명령어에서 한자리  숫자만 입력했을때. exco는 char형이  들어있기때문에 이를 int형으로 바꿔준다. 
		{
		  num=(int)(exco[0]-'0');
		}
		  else 
		  num=((int)exco[0]-'0')*10+(int)(exco[1]-'0');//명령어에서 두자리 숫자를 입력했을때 char형에서 int형으로 바꿔준다
		  
		 
		for(z=HEIGHT-1;z>=0;z--)
		{
		    for(k=WIDTH-1;k>=0;k--)
		    { 
			
			if(lines[z][k]!='\0')//lines에서 마지막 문자의 위치를 알아낸다
			{
			    x=z;
			    z=0;
			}
		    }
		    
		}
		if(num>x||(exco[2]>='0'&&exco[2]<='9'))//명령어로 입력된 숫자(즉, 사용자가 가고싶은 line number)가 lines 마지막 원소보다 더 아랫줄이라면 마지막 원소가 위치한 줄에서 젤 왼쪽에 커서를  위치하게한다
		{
		  i=x-1;
		  j=0;
		}else if(num<0)//만약 사용자가 0보다 작은 숫자를 입력해주면 첫번째 줄로 가게한다.
		{
		    i=0;
		    j=0;
		}else 
		{               //사용자가 입력한 줄, 젤 왼쪽으로  커서를  위치한다.
		    i=num-1;
		    j=0;
		    }

		X=false;
                N=true;
            

	   }else if(exco[0]=='/') //명령어 젤 첫 문자가 '/'라면 그 뒤 단어를 search 하라는 명령어다. 
	   {
               
	       x=find(&i,&j,lines,exco,flag-1);//find는 그 단어를 찾고 그 단어 첫 문자에 커서를 위치하게 하는 함수이다.
	       free(exco);
	       X=false; // 이후 바로 excommand 모드에서 edit 모드로 변환한다.
	       N=true;
	       
	   }
	   else
	   {  //이외 명령어는 알아듣지 못하므로 excommand 모드에서 normal 모드로 변환한다.
	       X=false;
	       N=true;
	   } 
   	   	   
	   } 
       
      
}
    free(exco);
    free(co);
    for(x=0;x<HEIGHT;x++)
{
    free(lines[x]); 
    free(vim_info[x]);
}	    
    return 0;
}





void draw(char **lines,int y,int x) {/*lines[HEIGHT][WIDTH]는 원래 '\0'이 들어있지만 excommand에서 :set nu를 해주면 '1'로 채워진다. 이를 통해 사용자가 
    set nu를 해주었나 구분하고 이에 따라 editor를 그려준다.*/
    int i,j;
if(lines[HEIGHT][WIDTH]!='1') //이는 set nu를 하지 않았을때 상태이다. 그러므로 줄을 출력하지않고 원래있었던 코드 그대로 editor를 그려준다. 
{    system("clear");
    printf("┏");
    for ( i = 0 ; i < WIDTH ; i++ ) 
        printf("━");
    printf("┓\n");
    for (  i = 0 ; i < HEIGHT ; i++ ) {
        printf("┃");
        for (  j = 0 ; j < WIDTH ; j++ ) {
            if ( y == i && x == j ) {
                printf("I");
            } else if(lines[i][j]=='\0')
		printf(" ");
		    else {
               // if(lines[i][j]!=ENTER&&lines[i][j]!='\n')
		    printf("%c",lines[i][j]);
            }
        }
        printf("┃");
        puts("");
    }
    printf("┗");
    for ( i = 0 ; i < WIDTH ; i++ ) 
        printf("━");
    printf("┛\n");
}else //lines[HEIGHT][WIDTH]=='1' 일때. 즉 사용자가 set nu를 해주었을때이다. 
{
    system("clear");
    printf("┏");
    for ( i = 0 ; i < WIDTH ; i++ )
            printf("━");
     printf("┓\n");
     for (  i = 0 ; i < HEIGHT ; i++ ) {
             if(lines[i][0]!='\0') // 그줄에 원소가 있다면 그 줄의 넘버를 출력한다. 
	     {if(i<9)
	     { printf("| %d ",i+1);
	     }else
		 printf("|%d ",i+1); 
	     }
	     else printf("|   "); //그 줄에 원소가 없다면 그냥 왼쪽 상자 면만 그려준다. 
     
             for (  j = 0 ; j <WIDTH-3 ; j++ ) { //line number 를 출력해줄것이기 때문에 상자 모양을 지키기 위해 각 줄마다 프린트할 원소를 2개 줄인다..
	             if ( y == i && x == j ) {
	                     printf("I");
	                 } else if(lines[i][j]=='\0')
		                 printf(" ");
	                         else {
		                       printf("%c",lines[i][j]);
		             }
	             }
             printf("┃");
             puts("");
         }
     printf("┗");
     for ( i = 0 ; i <WIDTH ; i++ )
             printf("━");
     printf("┛\n");

}
}
void Space(int* i,int *j, char** lines)//스페이스를 해주는 함수
{   
   int x;
   if(lines[*i][WIDTH-1]!='\0') // 커서가 있는 줄에 젤 오른쪽에 원소가 있다면 스페이스를 할수없게한다. 
       return;

   {
	    for(x=WIDTH-1;x>*j;x--)//커서에 오른쪽에 있는 문자을 한칸 오른쪽으로 옮긴다. 
	    {   
		lines[*i][x]=lines[*i][x-1];
	    }
	    
	lines[*i][*j]=' '; //커서가 있었던 자리는 빈칸으로 만든다 
	
	if(*j<WIDTH-1)//만약 커서가 젤 오른쪽에 위치하지 않았다면 커서를 오른쪽으로 한칸 옮긴다
	*j=*j+1;
        else if(*i<HEIGHT-1) //커서가 젤오른쪽에 있었다면 커서를 다음줄 젤 첫번째 자리로 옮긴다. 
	{
	  *i=*i+1;
	  *j=0;
	}	  
   } 
}
void backSpace(int*ip,int*jp, char**lines)
{
    int i,j,x,y,k,flag;
    i=*ip;
    j=*jp;
    if(j==0&&i!=0)//커서가 제일  왼쪽에 있고 그 줄이 첫줄이 아닐때 
    {
	for(x=WIDTH-1;x>=0;x--) //커서 윗줄에 마지막 원소 자리를 찾는다 
	{
	    if(lines[i-1][x]!='\0')
	    {
		break;
	    }
	}
	flag=x; //flag 는 즉, 커서 윗줄이 넘치지않을때까지 윗줄에 넣어줄수있는 문자 갯수이다. 
	for(y=0;y<=WIDTH-1;y++) 
	{
	    if(lines[i][y]=='\0') //이 루프가 끝나면 y는 커서가 위치한 줄에 문자 갯수이다  
	    {
		break;
	    }
	}
	
	    for(k=0;k<=y-1;k++)//커서 윗줄에 커서가 있던 줄에 내용을 옮겨준다. 
	    {
		lines[i-1][x+1+k]=lines[i][k];
	    }
           
       
	for(x=i;x<HEIGHT-1;x++) //커서가 원래 위치 뒤에 있던 내용들은 한 줄 앞당겨준다 
	{
	    for(y=0;y<WIDTH;y++)
	    {
		lines[x][y]=lines[x+1][y];
	    }
	}
	for(y=0;y<WIDTH;y++)//마지막 줄 내용은 없앤다 
	{
	    lines[HEIGHT-1][y]='\0';
	}
	*ip=*ip-1; //커서를 한칸 위로 위치하고 옮긴 문장의 첫 문자 위치로 가게 한다. 
	*jp=flag+1;
    }
    else if(!(i==0&&j==0)) //커서가 젤 왼쪽에 위치하지 않는다면 BACKSPACE를 해줘도 그 줄에 내용만 바뀌게 한다. 
    {            
	    for(x=j-1;x<=WIDTH-1;x++) //커서 왼쪽에 있던 내용을 다 한칸 오른쪽으로 옮긴다
	    {
		lines[i][x]=lines[i][x+1];
	    }                           
	
	    lines[i][WIDTH-1]='\0'; //그 줄 마지막 원소를 없앤다
	    if(j>0) //만약 커서가 젤 왼쪽이 아니라면 왼쪽으로 한칸 옮긴다
	    {
		*jp=*jp-1;
	    }
    }  

	                 
}
void Enter(int *i, int*j,char**lines)
{
   int x,y,z,k,flag;
   z=*j;
   k=*i;
   for(x=0;x<WIDTH;x++)//lines의 젤 마지막 줄에 어떠한 내용이 있다면 엔터를 불가능하게 한다.
   {
       if(lines[HEIGHT-1][x]!='\0')
       {
	   return;
       }
   }

   if(*i!=HEIGHT-1)//엔터를 쳤을때 커서가 젤 마지막 줄에 위치하지 않는다면 
   {
       for(x=HEIGHT-1;x>*i;x--) //엔터를 친 위치에서 두줄 아래  내용을 모두 한 줄 더  아래로 옮긴다
       {
	   for(y=0;y<=WIDTH-1;y++)
	   {
	       lines[x][y]=lines[x-1][y];
	   }
       }

       for(y=0;y<=WIDTH-1;y++)//엔터를 친 위치 바로 아랫줄에 내용을 없앤다
       {
	   lines[*i+1][y]='\0';
       }
  
       for(y=0;y<=*j;y++)//엔터를 친 위치에서 오른쪽에 있는 내용을 바로 아랫줄에 옮긴다 
       {
	   lines[*i+1][y]=lines[*i][*j]; 
	   *j=*j+1;
	   if(*j==WIDTH-1)
	   {
	       *j=0;
	       *i=*i+1;
	       break;
	   }
       }
       for(x=WIDTH-1;x>=z;x--) //원래 커서가 있던 위치에 오른쪽에 있는 내용은 삭제한다. 
	   lines[k][x]='\0';
       if(z==0) //만약 엔터를 쳤을때 커서가 젤 왼쪽에 있었다면 그 줄에 젤 왼쪽 자리에 ' '를 넣는다. 
       {
	   lines[k][0]=' '; 
       }

       
       }
   
}
int find(int*i,int*j,char**lines,char*exco,int length)// 사용자가 excommand 에서 /search를 해주면 불러오는 함수. search해줄 단어의 첫 문자의 위치에 커서를 옮긴다  exco 문자열엔 찾는 단어가 들어있음. length는 그 단어의 길이.
{
  int x,y,k,check,found;
  found=-1;
  for(x=*i;x<HEIGHT;x++) //커서 위치 바로 오른쪽에서 부터 search를 시작함. 
  {
      k=1;     // 해당 줄에 단어를 찾지 못하였으면 k=1 그리고 check=0으로 initialize 해줌. 이를 통해 찾는 단어는 무조건 같은 줄에 있어야함. 
      check=0; //여기서 k가 0인 이유는 exco[0]~exco[length] 까지 찾는 단어의 원소가 들어있기 때문이다. 
      //check 는 lines에서 찾는 단어의 원소와 같을때마다 1을 늘려준다.
      for(y=0;y<WIDTH;y++)
      {
	if(x==*i&&y<=*j)//이 컨디션을 통해 lines에서 커서 위치 전에 있는 내용에서는 단어를 찾지않음. 
	{

	}
	else if(lines[x][y]==exco[k]) // 찾는 단어의 문자를 찾으면 다음원소와 그 다음 문자를 비교해주기 위해 k를 1 더해줌. check를 더해주어 찾은 단어의 갯수를 업데이트함.
	{
	    k++;
	    check++;
	}else 
	{
	    k=1;
	    check=0;
	}
	if(check==length)//check가 length 와 같다는 뜻은 해당 단어를 찾았다는걸 뜻한다. 
	{
	   
	    *i=x;
	    *j=y-length+1;
	    y=WIDTH;
	    x=HEIGHT;
	    found= 1;
	   /* }else
	    {
		check=0;
		k=1;
	    }*/
	}
  }
 }
return found;
}
void save(char* filename, char**lines)//lines 2차원 배열을 파일에 저장한다.  
{
    FILE *fout;
    int x,y;
    fout=fopen(filename,"w");
    for(x=0;x<HEIGHT;x++)
    {
	for(y=0;y<WIDTH;y++)
	{
	    if(lines[x][y]!='\0')//lines[x][y]가 비어있지않다면 파일에 해당 char 을 옮겨준다 
		fputc(lines[x][y],fout);
	}
	fputc(ENTER,fout); //각 줄이 끝날때 ENTER를 넣어준다. 
    }
    fclose(fout);//파일저장이 끝나면  fclose를 해준다
}
void nextWord(int* i, int*j,char** lines)//커서 위치로부터  다음 단어를 찾아 커서를 그 단어 첫 문자로 위치하게 한다
{
    int x,y;
      for(y=*i;y<HEIGHT;y++)
      {
	  for(x=0;x<WIDTH-1;x++)
	  {
	      if(y==*i&&x<=*j) //커서가 있던 그 줄과 그 줄 왼쪽 내용은 무시한다 
	      {
	      }
	      else if(x==0&&lines[y][x]!=' '&&lines[y][x+1]!='\0')//문자가 줄 제일 왼쪽에 있고 바로 두번째 칸에도 문자가 있다면 커서를 젤 왼쪽으로 위치하게 한다.
	      {
		  *i=y;
		  *j=x;
		  y=HEIGHT;
		  x=WIDTH;
		  break;
	      }
	      else if(lines[y][x]=='_') //문자가 _ 일때 
	      {
		  if(((lines[y][x-1]>='a'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='A'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='0'&&lines[y][x-1]<='9')))// '_' 앞에 숫자나 알파벳이 없을때만 커서를 '_'위치에 옮긴다 
		  {
		   *i=y;
		   *j=x;
		    y=HEIGHT;
		    x=WIDTH;
		    break;  
		  
		  }

	      }
	      else if((lines[y][x]>=33&&lines[y][x]<=47)||(lines[y][x]>=58&&lines[y][x]<=64)||(lines[y][x]>=91&&lines[y][x]<=96)||(lines[y][x]>=123&&lines[y][x]<=126))
	      { //문자가 punctuation mark일때 
		 if(!((lines[y][x-1]>=33&&lines[y][x-1]<=47)||(lines[y][x-1]>=58&&lines[y][x-1]<=64)||(lines[y][x-1]>=91&&lines[y][x-1]<=96)||(lines[y][x-1]>=123&&lines[y][x-1]<=126)))
		 {            
		  *i=y;
		  *j=x;
		  y=HEIGHT;
		  x=WIDTH;
		  break;
		 }
	      }else if((lines[y][x]>='a'&&lines[y][x]<='z')||(lines[y][x]>='A'&&lines[y][x]<='Z')||(lines[y][x]>='0'&&lines[y][x]<='9'))
	      { //문자가 알파벳이거나 숫자일때 
		  if((lines[y][x-1]==' ')||(lines[y][x-1]>=33&&lines[y][x-1]<=47)||(lines[y][x-1]>=58&&lines[y][x-1]<=64)||(lines[y][x-1]>=91&&lines[y][x-1]<=96)||(lines[y][x-1]>=123&&lines[y][x-1]<=126))//그 문자 바로 왼쪽이 ' '이거나 punctuation mark 일때만
		  {
		      if(!(lines[y][x-1]=='_')) //하지만 punctuation mark중 '_' 일땐 무시해야한다. 바로 왼쪽이 '_'가 아닌 다른 punctuation mark일때 그 문자에 커서를 위치한다. 
			      {
		      *i=y;
		      *j=x;
		      y=HEIGHT;
		      x=WIDTH;
		      break;
		      }
		  }
	      }
	  }
	  } 
}
void beforeWord(int*i,int*j,char**lines)//커서 위치로 부터 전에 단어의 첫 문자에 커서를 옮겨준다 
{
	int x,y;
	for(y=*i;y>=0;y--)
	{
	    for(x=WIDTH-2;x>=0;x--)
	    {
		if(y==*i&&x>=*j)//커서에서 다음내용은 무시한다 
		{
		}
		else if(x==0&&lines[y][x]!=' '&&lines[y][x+1]!='\0'&&lines[y][x+1]!=' ')//문자가 젤 왼쪽에있고 다음칸에도 문자가 있다면 커서를 젤 왼쪽으로 위치한다
		{
		   *i=y;
		   *j=x;
		   y=-1;
		   x=-1;
		   break;
		}
		else if(lines[y][x]=='_') //문자가 '_'이면 왼쪽에 알파벳이나 숫자가 없을때만커서를 '_'가 있는 위치에 옮겨준다
		{
		  if((lines[y][x-1]>='a'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='A'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='0'&&lines[y][x-1]<='9'))
		  {
		    *i=y;
		    *j=x;
		    y=-1;
		    x=-1;
		    break;
		}
	    }else if((lines[y][x]>=33&&lines[y][x]<=47)||(lines[y][x]>=58&&lines[y][x]<=64)||(lines[y][x]>=91&&lines[y][x]<=96)||(lines[y][x]>=123&&lines[y][x]<=126))
	    { //문자가 punctuation mark일떄 
		if((lines[y][x-1]==' ')||(lines[y][x-1]>='a'&&lines[y][x-1]<='z')||(lines[y][x-1]>='A'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='0'&&lines[y][x-1]<='9')||(lines[y][x+1]>='a'&&lines[y][x+1]<='z')||(lines[y][x+1]>='A'&&lines[y][x+1]<='Z')||(lines[y][x+1]>='0'&&lines[y][x+1]<='9'))
		{ //그 문자 전이나 앞이 비었거나 알파벳이나 숫자일때 커서를 그 punctuation mark가 있는곳으로 옮겨준다 
		   *i=y;
		   *j=x;
		   y=-1;
		   x=-1;
		   break;
		}
	    }else if((lines[y][x]>='a'&&lines[y][x]<='z')||(lines[y][x]>='A'&&lines[y][x]<='Z')||(lines[y][x]>='0'&&lines[y][x]<='9'))//문자가 알파벳이나 숫자일때
	    {
	       if((lines[y][x-1]==' ')||(lines[y][x-1]>=33&&lines[y][x-1]<=47)||(lines[y][x-1]>=58&&lines[y][x-1]<=64)||(lines[y][x-1]>=91&&lines[y][x-1]<=96)||(lines[y][x-1]>=123&&lines[y][x-1]<=126)) //그 문자 왼쪽이 ' '이거나 punctuation mark 이고 그 punctuation mark가 '_' 가 아닐때 커서를 그 문자에 위치한다 
	       {
		   if(!(lines[y][x-1]=='_')) 
		   {
		   *i=y;
		   *j=x;
		   y=-1;
		   x=-1;
		   break;
		   }
	       }
	       else if(!((lines[y][x-1]>='a'&&lines[y][x-1]<='z')||(lines[y][x-1]>='A'&&lines[y][x-1]<='Z')||(lines[y][x-1]>='0'&&lines[y][x-1]<='9')))
		{
		    
			*i=y;
			*j=x;
			y=-1;
			x=-1;
			break;
		    
		}
	    }
	    }
}
}
