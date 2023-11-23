#include "calculator.h"
#include "list.h"
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

void addNext(struct NODE *target, char data){
    struct NODE *newNode = malloc(sizeof(struct NODE));
    newNode->next = target->next;
    newNode->data = data;

    target->next = newNode;
}

char removeNext(struct NODE *target){
    struct NODE *removeNode = target->next;
    target->next = removeNode->next;
    char data = target->data;
    free(removeNode);
    return data;
}

LINK infix_to_postfix(LINK infix){
    struct NODE *temp_head = malloc(sizeof(struct NODE));
    temp_head->next=NULL;

    struct NODE *postfix_head = malloc(sizeof(struct NODE));
    postfix_head->next=NULL;

    int signal=0;
    while (infix->next != NULL){
        if (infix->d == "("){
            addNext(postfix_head, " ");
            addNext(temp_head, removeNext(infix));
            signal=0;
        }
        else if (infix->d == ")"){
            addNext(postfix_head, " ");
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

    while(postfix->next!=NULL){  //후위표기법 계산은 왼쪽에서부터 하므로, 순서를 반대로 바꿔서 다른 스택에 저장한다.
        addNext(temp1_head, removeNext(postfix));
    }

    while(temp1_head->next!=NULL){
        if ((temp1_head->data == "0") || (temp1_head->data == "1") || (temp1_head->data == "2") || (temp1_head->data == "3") || (temp1_head->data == "4") || (temp1_head->data == "5") || (temp1_head->data == "6") || (temp1_head->data == "7") || (temp1_head->data == "8") || (temp1_head->data == "9")){
            //여기부터 구현 이어가기.
        }
        
    }
}

//덧셈
int add_postfix(int a, int b){
    //내용 구현하기
    return 0;
}

//뺄셈
int subtract_postfix(int a, int b){
    //내용 구현하기
    return 0;
}
//곱셈
int multiply_postfix(int a, int b){
    //내용 구현하기
    return 0;
}
//나눗셈
int divide_postfix(int a, int b){
    //내용 구현하기
    return 0;
}



//소수를 정수로 바꿔주는 함수

//계산 결과를 소수로 보정해주는 함수

//식은 문자열로 오는데, 그 안에 잘못 입력된 애도 있고, 공백 문자도 있고, 연산자, 피연산자가 있다. 예외처리할지, 제거할 건 제거할지 결정하고 프로그래밍하자.
