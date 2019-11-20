#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
        int key;
        struct node* left;
        struct node* right;
};

struct node* newNode(int key){//if null, make new node
        struct node *temp = (struct node*) malloc (sizeof(struct node));
        temp->key = key;
        temp->left = NULL;
        temp->right = NULL;

        return temp;//point return
}

//stack

char stack[1024];
int top;
int notCnt = 0;

void init_stack();
char push(char t);
char pop();
char s_top();
int isEmpty();

void init_stack(){// stack initialization
        top --;
}

char push(char t) {
        if( top >= 1024-1){
                printf("Stack overflow occur.\n");
                return -1;
        }
        stack[++top] = t;
        return t;
}

char pop(){
        if(top < 0) {
                printf("Stack is empty.\n");
                return -1;
        }
        return stack[--top];
}

char s_top(){
        return stack[top];
}

int isEmpty(){
        if(top <= 0) return 1;
        else return 0;
}

char* parse(struct node* root, char* exp);
struct node* pushTree(struct node* root, int data);
struct node* negateNode(struct node* root, int data);
struct node* deleteNode(struct node* root, int data);
void distributive(struct node* root);
void printResult(struct node* root);
void negate(struct node* root);
void preOrderPrint(struct node* root);
int check(char* expr);

char* parse(struct node* root, char* exp){
        char* pch;
        pch = strtok(exp, " ");//space by space
        root = pushTree(root, check(pch));

        while(pch != NULL){
                pch = strtok(NULL, " ");
                if(pch != NULL){
                        pushTree(root, check(pch));
                }
        }

        root = negateNode(root, 0);
        for(int i = 0; i < notCnt; i++) {//notCnt??????
                root = deleteNode(root, 0);
        }

        distributive(root);
        preOrderPrint(root);
        printf("\n");

        return pch;
}

int check(char* expr){
        if(strcmp(expr, "(not") == 0){
                notCnt++;
                return 0;
        }else if(strcmp(expr, "(or") == 0){
                return -1;
        }else if(strcmp(expr, "(and") == 0){
                return 1;
        }else{ // ex) a1, a2, a3...
               int num = expr[1] - '0';
                return num + 1;//if a1, 1 is already 'and'
        }
}

struct node* negateNode(struct node* root, int data){//key in tree, data $
        if(root == NULL) {
                printf("Tree is empty\n");
                return NULL;
        }else{
                if(root->key == data){
                        negate(root);
                }
                if(root->left != NULL){
                        negateNode(root->left,0);
                }
                if(root->right != NULL){
                        negateNode(root->right,0);
                }
        }
        return root;
}

void negate(struct node* root){
        if(root == NULL) return ;
        else{
                root->key = (root->key)* -1; //negation
                if(root->left != NULL){
                        negate(root->left);
                }
                if(root->right != NULL){
                        negate(root->right);
                }
        }
}

struct node* deleteNode(struct node* root, int data){
        if(root == NULL)return NULL;

        if(root->key == data){
                struct node* temp = (struct node*)malloc(sizeof(struct node));
                temp = root->left; //save root->left
                free(root);
                return temp;
        }else{
                //traverse to find the node to delete
                root->left = deleteNode(root->left, data);
                root->right = deleteNode(root->left, data);
        }

        return root;

}

void preOrderPrint(struct node* root){
        if(root == NULL) return ;

        preOrderPrint(root->left);
        if(root->key == -1) printf("\n");//change 1 --> -1

        if(root->key != 0 && root->key != 1 && root->key != -1){
                int value = root->key;
                if(value < 0) value ++;
                else value--;

                printf("%d ", value);
        }

        preOrderPrint(root->right);
}

int hasBothChild(struct node* node){
        return node != NULL && node->right != NULL && node->left != NULL;
}

void distributive(struct node* root){
        printf("distributive..\n");
}

struct node* pushTree(struct node* node, int data){
        if(node == NULL) return newNode(data);

        if(node->key == 0){//not

                if(node->left == NULL){
                node->left = pushTree(node->left, data);
                }else{//node->left != NULL
                        pushTree(node->left, data);
                }
        }else if(node->key == 1 || node->key == -1){//or, and

                if(node->left == NULL){//left first
                node->left = pushTree(node->left, data);
                }else if(node->right == NULL){//and then right

                        if(node->left->key == 0){
                          if(!hasBothChild(node->left->left)) pushTree(node->left, data);
                          else{
                                node->right = pushTree(node->right, data);
                          }
                        }else{
                                node->right = pushTree(node->right, data);
                        }
                }else return node; // 굳이?
        }

        return node;
}

int main() {
        char exp[1024];

        printf("Enter the expression : ");
        gets(exp);

        for(int i = 0; i < strlen(exp); i++){
                if(exp[i] == '(') push(exp[i]);//push stack (
                else if(exp[i] == ')'){
                        if(s_top() == '(') pop(); // pop stack (
                        else push(exp[i]);
                }
        }//if it is a right form, it will be empty

        if(!isEmpty()){
                printf("Wrong input...\n");
                return -1;
        }
        //good formula, program operating
        struct node* root = NULL;
        parse(root, exp);

        printf("0\n");
        //findSolution();//구현하기
}


