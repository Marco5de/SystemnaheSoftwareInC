#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_CHILDS 4
#define FILENAME "test.txt"
#define BUFFLEN 128

//for debugging
#define PRINT

//forward declaration
typedef struct qtreeNode_t qtreeNode_t;

typedef struct{
	int x,y;
}Pixel_t;

struct qtreeNode_t{
	qtreeNode_t *lu;
	qtreeNode_t *ru;
	qtreeNode_t *ll;
	qtreeNode_t *rl;

	int size;
	bool set;
};	

int getDim(){
	FILE *file;
	file = fopen(FILENAME,"r");
	if(!file){printf("Error loading file\n");exit(1);}

	char buf[BUFFLEN];
	fgets(buf,BUFFLEN,file);
	return atoi(buf);
}
// returns 0 for put 1 || for print
int handleInput(char *str,Pixel_t *p){
	char *ptr;
	ptr = strtok(str," ");
	if(!strcmp(ptr,"PUT")){
		ptr = strtok(NULL," ");
		p->x = atoi(ptr);
		ptr = strtok(NULL,"\n");
		p->y = atoi(ptr);
		return 0;
	}else if(!strcmp(ptr,"PRINT")){
		return 1;
	}
	return -1;
}

bool checkLeaf(qtreeNode_t *node){
	//printf("lu %p ll %p ru %p rl %p\n",node->lu,node->ll,node->ru,node->rl);
	if(!node->lu && !node->ll && !node->ru && !node->rl){
		//printf("Is leaf!\n");
		return true;
	}else 
		return false;
}

void insert(Pixel_t *pix, qtreeNode_t *node){
	//pruefen ob es sich um den basisfall handelt
	if(node->size == 1){
		node->set = true;
		node->ll=0;node->lu=0;node->rl=0;node->ru=0;
		return;
	}	
	//checking in which quadrant of the tree/subtree the input pixel is in relation to the given node
	int si = node->size;
	//checking inside of each case if selected quadrant is null, if so create new node with ns = size/2	and set attr
	if((pix->x < si/2) && (pix->y <	si/2)){ //pixel in left upper quadrant
		if(!(node->lu)){ // quadrant doesn't exist yet
			node->lu = calloc(1,sizeof(qtreeNode_t));
			if(!(node->lu)){printf("Unable to allocate memory\n");exit(0);}
			node->lu->size = si/2;
			node->lu->set = false;
		}
		insert(pix,node->lu);
	}else if((pix->x < si/2) && (pix->y >= si/2)){ //pixel in left lower quadrant
		if(!(node->ll)){
			node->ll = calloc(1,sizeof(qtreeNode_t));
			if(!(node->ll)){printf("Unable to allocate memory\n");exit(0);}
			node->ll->size = si/2;
			node->ll->set = false;
		}	
		pix->y -= si/2; //even because si is always power of 2
		insert(pix,node->ll);
	}else if((pix->x >= si/2) && (pix->y < si/2)){ //pixel in right upper quadrant
		if(!(node->ru)){
			node->ru = calloc(1,sizeof(qtreeNode_t));
			if(!(node->ru)){printf("Unable to allocate memory\n");exit(1);}
			node->ru->size = si/2;
			node->ru->set = false;
		}	
		pix->x -= si/2;
		insert(pix,node->ru);
	}else if((pix->x >= si/2) && (pix->y >= si/2)){ //pixel in right lower quadrant
		if(!(node->rl)){
			node->rl = calloc(1,sizeof(qtreeNode_t));
			if(!(node->rl)){printf("Unable to allocate memory\n");exit(1);}
			node->rl->size = si/2;
			node->rl->set = false;
		}	
		pix->x -= si/2;
		pix->y -= si/2;
		insert(pix,node->rl);
	}
	//check if this part of the tree can be minimized
	bool leaf = checkLeaf(node);
	if(leaf){
			if(node->ll->set && node->lu->set && node->rl->set && node->ru->set){
				free(node->ll);node->ll=0;
				free(node->lu);node->lu=0;
				free(node->rl);node->rl=0;
				free(node->ru);node->ru=0;
				node->set = true;
				#ifdef PRINT
				printf("Knoten wurden zusammengelegt\n");
				#endif
			}
	}	


}

void printString(qtreeNode_t *node){
	if(!node){printf("B");return;}
	if(checkLeaf(node)){
		if(node->set)
			printf("W");
		else
			printf("B");
	}else{
		printf("[");
		printString(node->ll);
		printString(node->lu);
		printString(node->rl);
		printString(node->ru);
		printf("]");
	}
	

}

bool isSet(qtreeNode_t *node, Pixel_t *pix){
	if(!node)
		return false;
	
	if(node->size == 1)
		return node->set;
	
	int si = node->size;
	if((pix->x < si/2) && (pix->y < si/2)){
		return isSet(node->lu,pix);
	}else if((pix->x < si/2) && (pix->y >= si/2)){
		pix->y -= si/2;
		return isSet(node->ll,pix);
	}else if((pix->x >= si/2) && (pix->y < si/2)){
		pix->x -= si/2;
		return isSet(node->ru,pix);
	}else if((pix->x >= si/2) && (pix->y >= si/2)){
		pix->x -= si/2;
		pix->y -= si/2;
		return isSet(node->rl,pix);
	}
	return false;
}	

void printImg(qtreeNode_t *node){
	int size = node->size;
	Pixel_t *pix = calloc(1,sizeof(Pixel_t));
	for(int i=0; i<size;i++){
		for(int j=0; j<size;j++){
			pix->x = i;
			pix->y = j;
			if(isSet(node,pix)){
				printf(" ");
			}else{
			 	printf("#");
			}	
		}
		printf("\n");
	}	
	printf("\n\n");
	free(pix);
}

void workLoop(qtreeNode_t *tree){
	FILE *file;
	file = fopen(FILENAME,"r");
	if(!file){printf("Error loading file\n");exit(1);}
	char buf[BUFFLEN];
	fgets(buf,BUFFLEN,file);

	while(fgets(buf,BUFFLEN,file)){
		Pixel_t *pix;
		if(!(pix = calloc(1,sizeof(pix)))){printf("Error allocating memory!\n");exit(1);}
		if(!handleInput(buf,pix)){
			insert(pix,tree);
		}else if(handleInput(buf,pix) == 1)

			printString(tree);
			printf("\n");
			printImg(tree);
		}	free(pix);
	}
//TODO zusammenfassen von gesetzen knoten! alles weiße zusammfassen
	fclose(file);
}


int dim = 0;

int main(){
	dim = getDim();

	qtreeNode_t *root;
	root = calloc(1,sizeof(qtreeNode_t));
	if(!root){printf("Unable to allocate memory\n");exit(0);}
	root->size = dim;
	root->set = false;

	workLoop(root);
	return 0;
}	
