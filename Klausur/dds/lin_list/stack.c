#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//kein wirklicher stack, eigentliche eine lineare Liste von welcher sich nur das letze element entfernen lässt
//für stack würde man sich das letzte element + eine stack größe speichern, siehe anderes programm!

typedef struct stack_ele_t stack_ele_t;

struct stack_ele_t{
	int val;
	stack_ele_t *next;
};

typedef struct stack{
	stack_ele_t *top;
}stack_t;


size_t stack_len(const stack_t stack){
	stack_ele_t *sel= stack.top;
	size_t size = 0;
	while(sel){
		size++;
		sel=sel->next;
	}
	return size;
}

stack_ele_t *create_ele(int val){
	stack_ele_t *new_ele = calloc(1,sizeof(stack_ele_t));
	if(new_ele){
		new_ele->val = val;
		new_ele->next = NULL;
	}
	return new_ele;
}

stack_ele_t *go_to_end_of_stack(stack_t stack){
	stack_ele_t *sel = stack.top;
	if(!sel)
		return NULL;
	while(sel->next)
		sel=sel->next;
	return sel;
}

int push(stack_t stack,int val){
	stack_ele_t *new_ele = create_ele(val);
	if(!new_ele)
		return -1;
	stack_ele_t *end = go_to_end_of_stack(stack);
	if(!end)
		return -1;
	end->next = new_ele;
	return 0;
}


void print_stack(stack_t stack){
	stack_ele_t *el = stack.top;
	for(;el;el=el->next)
		printf(" %d",el->val);
	printf("\n\n");
}

int main(){
	printf("\a\a\a");
	stack_t *stack = calloc(1,sizeof(stack_t));
	stack->top = calloc(1,sizeof(stack_ele_t));
	stack->top->val = 17;

	print_stack(*stack);	

	for(int i=0; i<1000;i++){
		push(*stack,i);
	}

	printf("Stacklen: %ld\n",stack_len(*stack));
	print_stack(*stack);

}
