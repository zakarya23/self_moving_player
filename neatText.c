/************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h> 

/************************************************************************/

#define INIT 10	 
#define NEWLINE "\n"
#define STAGE0 "==STAGE 0=======================================\n" 
#define STAGE1 "==STAGE 1=======================================\n"
#define coord 2 
#define VALID "The route is valid!"
#define WRONG_GOAL "Goal cell in the route is wrong!"
#define BLOCK "There is a block on this route!"
#define ILLEGAL "There is an illegal move in this route!"
#define INITIALS "Initial cell in the route is wrong!" 
#define ARROW "->" 
#define BORDER "------------------------------------------------"

/************************************************************************/

typedef struct node node_t;

typedef struct {
	int x,y;
} data_t; 

typedef struct {
	data_t data; 
	int count;
} count_t;

struct node {
	data_t data;
	node_t *next;
};

typedef struct newnode newnode_t;

struct newnode {
	count_t datab; 
	newnode_t *nextt;
	newnode_t *prev; 
};


typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

typedef struct {
	newnode_t *head;
	newnode_t *foot;
} newlist_t;


/************************************************************************/

list_t *make_empty_list(void);
list_t *insert_at_head(list_t *list, data_t value);
int is_empty_list(list_t *list);
list_t *get_tail(list_t *list);
data_t get_head(list_t *list);
void free_list(list_t *list);
data_t read_coords();
list_t *insert_at_foot(list_t *list, data_t value);
void i_newnode(newnode_t newnode);
newlist_t *make_empty_newlist(void);
newlist_t *insert_at_newhead(newlist_t *newlist,count_t value);
newlist_t *insert_at_newfoot(newlist_t *list, count_t value);
int is_empty_newlist(newlist_t *list);
newlist_t *get_newtail(newlist_t *list);
count_t get_newhead(newlist_t *list);
data_t upp(data_t coor);
data_t downn(data_t coor);
data_t leftt(data_t coor);
data_t rightt(data_t coor);
int is_in_list(newlist_t *list,data_t element);
void add_doublelist(newlist_t *list, count_t value);
void go_throughlist(newlist_t *list, int num);
void checklist(newlist_t *list,int n,char **grid,data_t after);
int len_oflist(newlist_t *list);
void insert_foot(newlist_t *list,count_t value);
void free_newlist(newlist_t *list);
void print_grid(char **grid,int rows,int columns);

/************************************************************************/

int
main(int argc, char *argv[]) { 
	printf(STAGE0);
	list_t *list;
	newlist_t *newlist; 
	data_t point,next,initial,goal,bloc,prev,after,up,down,left,right,blob; 
	int rows,columns,coor=0;
	int block=0,count=0,i,j,tcoor=0,c=0,con,f_con;
	char **grid;
	count_t *co;
	count_t start,p;
	newnode_t *temp; 

	
	list = make_empty_list();
	newlist = make_empty_newlist(); 
	/* Scand the first line to get dimensions and then mallocs the */ 
	/* required space */ 
	
	if (scanf("%dx%d",&rows,&columns)){
		grid = malloc(rows*sizeof(*grid));
		co = malloc(rows*columns*sizeof(*co)); 
		assert(grid); 
		
		for (i=0;i<rows;i++) {
			grid[i] =malloc(columns*sizeof(*grid[i])); 
			assert(grid[i]); 
		}
		
		/* Initialises each entry in the grid */ 
		for (i=0;i<rows;i++) {
			for (j=0;j<columns;j++) {
				grid[i][j] = ' ' ; 
			}
		} 
		printf("The grid has %d rows and %d columns.\n",rows,columns);
	}
	
	
	initial = read_coords();  /* Gets the initial coordinates from func */ 
	goal = read_coords(); 		/* Gets the goal coordinates from func */ 
	
	grid[initial.x][initial.y] = 'I';
	grid[goal.x][goal.y] = 'G';
	
	/* Gets all the blocked coordinates and then stores them as "#" in */ 
	/* grid, and also gets total number of blocks */
	while (scanf(" [%d,%d]",&bloc.x,&bloc.y)) {
		grid[bloc.x][bloc.y] = '#'; 
		block++; 
		 
	}
	
	/* Prints out all the stuff that has been scanned */
	printf("The grid has %d block(s).\n", block);
	printf("The inital cell in the grid is [%d,%d].\n",initial.x,initial.y);
	printf("The goal cell in the grid is [%d,%d].\n",goal.x,goal.y);
	printf("The proposed route in the grid is:\n");
	
	
	/* Starts scanning the route */ 
	
	if (scanf(" $ [%d,%d]",&point.x,&point.y)) { 
			list = insert_at_head(list,point); 	/* First element at head */
			printf("[%d,%d]",point.x,point.y);
			coor++;
			tcoor++; 
		
		}
		
		while ((scanf(" -> [%d,%d]",&point.x,&point.y)) == 2){
			tcoor++; 
			if (grid[point.x][point.y] !='#' && grid[point.x][point.y]!='G') { 
				grid[point.x][point.y] = '*';	
			}
			if (coor==5) {
				printf(ARROW); 
				printf(NEWLINE); 
				list = insert_at_foot(list,point); 
				printf("[%d,%d]",point.x,point.y);
				
				coor = 0; 
			}
			else {
				list = insert_at_foot(list,point); 
				printf("->[%d,%d]",point.x,point.y);
			}
			coor++;
		}
		printf(".");
		printf(NEWLINE); 
		 
	
	/* Prints out elements in the list one by one and at the same time */ 
	/* checking if they meet all the requirements */ 
	
	while (!is_empty_list(list)) {
		next = get_head(list);
		if (count == 0) {
			/* If initial cell doesnt match one given in route */
			if (next.x != initial.x || next.y != initial.y) {
				printf(INITIALS);
				exit(EXIT_FAILURE);
				break; 
			}
			/* If there is a block in the route and its the first cell */
			if (grid[next.x][next.y] == '#') {
				printf(BLOCK); 
			
				break; 
			}
		
		count++; 
		prev=next;
		}
		
		/* This is just checking for any illegal moves on the route */ 
		else if (((next.x == prev.x) && (prev.y!= next.y+1 &&				/* Will make func out of this */ 
			prev.y != next.y-1)) || ((next.y==prev.y) &&
				(prev.x!=next.x+1 && prev.x !=next.x-1))) {
					printf(ILLEGAL); 
					exit(EXIT_FAILURE); 	
					break; 
				} 
		else if (next.x == prev.x && next.y == prev.y) {
			printf(ILLEGAL);
			exit(EXIT_FAILURE);
			break;
		}
		else if (next.x <0 || next.y <0 || prev.x <0 || prev.y <0) {
			printf(ILLEGAL);
			exit(EXIT_FAILURE);
			break; 
		}
		
		
		/* Checks for any other blocks besides the first one */ 
		if (grid[next.x][next.y] == '#') {
			start.data = prev; 
			start.count = c; 
			c++; 
			list = get_tail(list); 
			after = get_head(list);  
			printf(BLOCK); 
			break; 
		}
		count++;
		prev=next;
		list = get_tail(list);
		
		if (count == tcoor+1) {
			prev=next;
			/* If goal cell given is different to one on the route */	 
			if (next.x != goal.x || next.y !=goal.y) {
				printf(WRONG_GOAL); 
				exit(EXIT_FAILURE);
				break; 
			}
		/* If all criteria is passed then the valid statemnt is print out */ 
		printf(VALID); 
		}
		
		
	}
	printf(NEWLINE); 
	printf(STAGE1); 
	printf(" "); 
	/* ALGORITHMS ARE FUN ! */ 
	print_grid(grid,rows,columns); 
	printf(BORDER);
	
	/* Starts work for stage1 */ 
        
       printf(NEWLINE);
       
       add_doublelist(newlist,start);
       count=0; 
       temp = newlist-> head; 
       blob = temp->datab.data;

       while (temp) {
       	   con = temp->datab.count; 
       	   blob = temp->datab.data;

       	   up = upp(blob);
       	   down = downn(blob);
       	   left = leftt(blob);
       	   right = rightt(blob); 
       	   /* If any of the moves gives you the goal cell then put it */ 
       	   /* at the end of the list */ 
       	   if ((up.x == after.x && up.y == after.y) 
       	   	   || (down.x == after.x && down.y == after.y)|| 
       	   		(right.x == after.x && right.y == after.y) ||
       	   		(left.x == after.x && left.y == after.y) ) {
       	       f_con = p.count;
       	       insert_foot(newlist,p);
       	   	   break; 
       	   }
       	   /* Checking all valid coordinates above and if valid added to */
       	   /* the list */ 
       	   if (up.x >0 && up.y >0 && grid[up.x][up.y] != '#' &&
       	   	   !is_in_list(newlist,up)) {
       	   	   p.data = up; 
       	   	   p.count = con+1;
       	   	   add_doublelist(newlist,p); 
       	   }
       	    /* Checking all valid coordinates down and if valid added to */
       	   /* the list */ 
       	   if (down.x>0 && down.y>0 && grid[down.x][down.y] != '#' &&
       	   	   !is_in_list(newlist,down)) {
       	   	   p.data = down; 
       	   	   p.count = con+1;
       	   	   add_doublelist(newlist,p);    
       	   }
       	    /* Checking all valid coordinates left and if valid added to */
       	   /* the list */ 
       	   if (left.x>0 && left.y && grid[left.x][left.y] != '#' &&
       	   	   !is_in_list(newlist,left)) {
       	   	   p.data = left; 
       	   	   p.count = con+1;
       	   	   add_doublelist(newlist,p);   
       	   }
       	    /* Checking all valid coordinates right and if valid added to */
       	   /* the list */ 
       	   if (right.x>0 && right.y && grid[right.x][right.y] != '#' &&
       	   	   !is_in_list(newlist,right)) {
       	   	   p.data = right; 
       	   	   p.count = con+1;
       	   	   add_doublelist(newlist,p);   
       	   }
       	   temp = temp->nextt;
       	   blob = temp->datab.data;   	  
       }
     
      checklist(newlist,f_con,grid,after); 
      /* ALGORITHMS ARE FUN ! */ 
      printf(" ");
      print_grid(grid,rows,columns); 
      printf(BORDER);
      printf(NEWLINE); 
      	while (!is_empty_list(list)) {
		next = get_head(list);
		/* If there is a block in the route and its the first cell */
			if (grid[next.x][next.y] == '#') {
				break; 
			}
			printf("[%d,%d]->",next.x,next.y); 
		count++;
		prev=next;
		list = get_tail(list);
		
      }
      printf(NEWLINE); 
      printf(VALID); 
      /* Frees the alllocates memories */ 
      free_list(list); 
      free_newlist(newlist); 
      for (i=0;i<rows;i++) {
      	  free(grid[i]); 
      }
      free(grid); 
      return 0; 
}

	

/*********************************************************************/
/* Got from listops */ 
list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

newlist_t
*make_empty_newlist(void) {
	newlist_t *list;
	list = (newlist_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}
/**********************************************************************/
/* Got from listops */ 
list_t
*insert_at_head(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = list->head;
	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}

/**********************************************************************/
/* Got from listops */ 
list_t
*insert_at_foot(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}


/**********************************************************************/
/* Got from listops */ 
int
is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}

int
is_empty_newlist(newlist_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}
/**********************************************************************/
/* Got from listops */ 
data_t
get_head(list_t *list) {
	assert(list!=NULL && list->head!=NULL);
	return list->head->data;
}


/**********************************************************************/
/* Got from listops */ 
list_t
*get_tail(list_t *list) {
	node_t *oldhead;
	assert(list!=NULL && list->head!=NULL);
	oldhead = list->head;
	list->head = list->head->next;
	if (list->head==NULL) {
		/* the only list node just got deleted */
		list->foot = NULL;
	}
	free(oldhead);
	return list;
}

/**********************************************************************/
/* Got from listops */ 
void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

/**********************************************************************/

void
free_newlist(newlist_t *list) {
	newnode_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->nextt;
		free(prev);
	}
	free(list);
}
/**********************************************************************/

/* Reads coordinates from the file using scanf */ 

data_t 
read_coords() {
	data_t a; 
	scanf(" [%d,%d]",&a.x,&a.y); 	
	return a; 
}

/********************************************************************/

/* Returns the top coordinates */
data_t 
upp(data_t coor) {
	data_t c;  
	c.x = coor.x -1; 
	c.y =coor.y;
	return c;
}

/**********************************************************************/

/* Returns the bottom coordinates */
data_t 
downn(data_t coor) {
	data_t c;  
	c.x = coor.x +1;
	c.y =coor.y;
	return c;
}

/**********************************************************************/

/* Returns the left coordinates */
data_t 
leftt(data_t coor) {
	data_t c;  
	c.x = coor.x;
	c.y = coor.y -1; 
	return c;
}

/**********************************************************************/

/* Returns the right coordinates */
data_t 
rightt(data_t coor) {
	data_t c;  
	c.x = coor.x;
	c.y = coor.y +1; 
	return c;
}

/**********************************************************************/

/* Checks if a coordinate is already in a list */ 

int 
is_in_list(newlist_t *list,data_t element) {
	newnode_t *p;
	data_t data;
	p=list->head;
	while(p->nextt!=NULL) {
		data = p->datab.data;
		if (data.x == element.x && data.y == element.y) {
			return 1;
		}
		p=p->nextt;
	}
	return 0; 
}

/**********************************************************************/

/* Keeps adding to the double linked list */ 

void 
add_doublelist(newlist_t *list, count_t value) {
	newnode_t *temp; 
	temp = (newnode_t *)malloc(sizeof(*temp));
	temp->nextt = NULL; 
	temp->prev = NULL; 
	temp->datab = value; 
	if (list->head == NULL) {
		list->head = temp;
	}
	else {
		newnode_t *p; 
		p = list->head; 
		while (p->nextt !=NULL) {
			p=p->nextt;
		}
		p->nextt = temp; 
		temp->prev =p; 
	}
}

/**********************************************************************/

/*Checks the list for a valid route and hence appends grid accordingly */ 
void 
checklist(newlist_t *list,int n,char **grid,data_t after) {
	newnode_t *temp; 
	count_t data;
	int count=n-1;

	temp = list->foot;
	data = temp->datab; 
	 
	while(temp) {
		
		if (temp->datab.count == count) {

			if (after.x-1 == temp->datab.data.x &&
				after.y == temp->datab.data.y) {
				grid[after.x-1][after.y] = '*'; 
				after.x = temp->datab.data.x; 
				after.y = temp->datab.data.y;
				count--;
				
			}
			else if (after.x+1 == temp->datab.data.x &&
				after.y == temp->datab.data.y) {
				grid[after.x+1][after.y] = '*'; 
				after.x = temp->datab.data.x; 
				after.y = temp->datab.data.y;
				count--;
			
			}
			else if (after.x == temp->datab.data.x &&
				after.y-1 == temp->datab.data.y) {
				grid[after.x][after.y-1] = '*';
				after.x = temp->datab.data.x; 
				after.y = temp->datab.data.y;
				count--;
				
			}
			if (after.x == temp->datab.data.x &&
				after.y+1 == temp->datab.data.y) {
				grid[after.x][after.y+1] = '*';
				after.x = temp->datab.data.x; 
				after.y = temp->datab.data.y;
				count--; 
				
			}	
		}
		else if ( count<0) {
			break; 
		}
		temp = temp->prev; 
	}

}

/**********************************************************************/

/* Returns the lemgth of the list */ 

int 
len_oflist(newlist_t *list) {
	newnode_t *temp; 
	int count=0; 
	temp = list->head;
	while(temp) {
		count++; 
		temp=temp->nextt; 
	}
	return count; 
}

/**********************************************************************/

/* Inserts an element at the end of the list pointed by the foot */ 
void
insert_foot(newlist_t *list,count_t value) {
	newnode_t *temp,*po; 
	int len,x=1; 
	temp = (newnode_t *)malloc(sizeof(*temp)); 
	temp->prev = NULL; 
	temp->nextt = NULL; 
	temp->datab = value; 
	po = list->head; 
	len = len_oflist(list);
	
	while(x<len-1) {
		po = po->nextt; 
		x++;
	}
	temp->nextt = NULL;
	po->nextt->prev = temp; 
	temp->prev = po; 
	po->nextt = temp;
	list->foot = temp; 
	
}

/**********************************************************************/

/* Prints the grid */ 
void 
print_grid(char **grid,int rows,int columns) {
	int i,j; 
	for (i=0; i<columns;i++) {
		printf("%d",i); 
	}
	printf(NEWLINE); 
	for (i=0;i<columns;i++) {
		printf("%d",i); 
		for (j=0;j<rows;j++) {
			printf("%c",grid[i][j]); 
		}
		printf(NEWLINE) ; 
	}

}