#include "calculator.h"
#include "list.h"
#include "add.h"
//LINK 관한 설명은 list.h참고
//LINK는 그냥 연결리스트임.

LINK string_to_list(char s[]){
    LINK head;

    if (s[0]=='\0'){
        return NULL;
    }
    else{
        head=malloc(sizeof(ELEMENT));
        head -> d = s[0];
        head -> next = string_to_list(s+1);
        return head;
    }
}
//연결리스트 관련 코드
struct NODE{
    struct NODE *next;
    char data;
};

void addNext(struct NODE *target, char data){//target 다음에 새로운 노드를 추가해줌.
    struct NODE *newNode = malloc(sizeof(struct NODE));
    newNode->next = target->next;
    newNode->data = data;

    target->next = newNode;
}

char removeNext(struct NODE *target){ //target노드를 지워줌.
    struct NODE *removeNode = target->next;
    target->next = removeNode->next;
    char data = target->data;
    target->data=removeNode->data;
    free(removeNode);
    return data;
}

LINK infix_to_postfix(LINK infix){
    struct NODE *temp_head = malloc(sizeof(struct NODE));
    temp_head->next=NULL;

    struct NODE *postfix_head = malloc(sizeof(struct NODE));
    postfix_head->next=NULL;

    int signal=0;
    while (infix->next != NULL){ //수정해야함. 마지막 문자가 while문에서 처리되지 않음.
        if (infix->d == "("){
            addNext(postfix_head, " "); //숫자가 아닌 애들은 띄어쓰기로 구분하기 위해서 넣은 코드임.
            addNext(temp_head, removeNext(infix));
            signal=0;
        }
        else if (infix->d == ")"){
            while (temp_head->data != "("){
                addNext(postfix_head, removeNext(temp_head));
            }
            signal=0;
        }
        else if((infix->d == "*") || (infix->d == "/")){
            addNext(postfix_head, " ");
            if (signal<2){
                addNext(temp_head, removeNext(infix));
            }
            else{
                addNext(postfix_head, removeNext(temp_head));
                addNext(temp_head, removeNext(infix));
            }
            signal=2;
        }
        else if((infix->d == "+") || (infix->d == "-")){
            addNext(postfix_head, " ");
            if (signal<1){
                addNext(temp_head, removeNext(infix));
            }
            else{
                addNext(postfix_head, removeNext(temp_head));
                addNext(temp_head, removeNext(infix));
            }
            signal=1;
        }
        else if ((infix->d == "0") || (infix->d == "1") || (infix->d == "2") || (infix->d == "3") || (infix->d == "4") || (infix->d == "5") || (infix->d == "6") || (infix->d == "7") || (infix->d == "8") || (infix->d == "9")){
            addNext(postfix_head, removeNext(infix));
        } //일단 공백 입력은 처리했음. //오타도 처리했음.
        else if(infix->d == "."){
            addNext(postfix_head, removeNext(infix));
        } //소숫점도 일단 그대로 가져옴.
    }
    return postfix_head;
}
//후위표기식에서 숫자들을 띄어쓰기로 구분했다.

//연산 과정
int calculate_postfix(LINK postfix){
    struct NODE *temp1_head = malloc(sizeof(struct NODE));
    temp1_head->next=NULL;

    struct NODE *temp2_head = malloc(sizeof(struct NODE));
    temp2_head->next=NULL;

    while(postfix->next!=NULL){//수정해야함. 마지막 문자를 처리할 수 없음.
        int signal=1;
        if ((postfix->d == ".") ||(postfix->d == "0") || (postfix->d == "1") || (postfix->d == "2") || (postfix->d == "3") || (postfix->d == "4") || (postfix->d == "5") || (postfix->d == "6") || (postfix->d == "7") || (postfix->d == "8") || (postfix->d == "9")){
            if (signal==1){
                while(postfix->d != " "){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp1_head,removeNext(postfix)); //첫 번째 숫자 덩어리
                    signal=2;
                }
            }
            else if (signal==2){
                while(postfix->d != " "){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp2_head,removeNext(postfix)); //두 번째 숫자 덩어리
                    signal=1;
                }
            }
        }
        else if((postfix->d == "+")){
            Add(temp1_head,temp2_head);
        }
    }
}