#include <stdlib.h>
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
    int data;
};

void addNext(struct NODE *target, int data){
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

    struct NODE *num_append_head = malloc(sizeof(struct NODE));
    num_append_head->next=NULL;

    int signal=0;
    while (infix->d != "\0"){
        if (infix->d == "("){
            addNext(temp_head, removeNext(infix) - 48); //removeNext리턴값은 char, addNext의 두 번쨰 인자는 int
            signal=0;
        }
        else if (infix->d == ")"){
            while (temp_head->data != "("){
                addNext(postfix_head, removeNext(temp_head) - 48);
            }
            signal=0;
        }
        else if((infix->d == "*") || (infix->d == "/")){
            if (signal<2){
                addNext(temp_head, removeNext(infix) - 48);
            }
            else{
                addNext(postfix_head, removeNext(temp_head) - 48);
                addNext(temp_head, removeNext(infix) - 48);
            }
            signal=2;
        }
        else if((infix->d == "+") || (infix->d == "-")){
            if (signal<1){
                addNext(temp_head, removeNext(infix)- 48);
            }
            else{
                addNext(postfix_head, removeNext(temp_head)- 48);
                addNext(temp_head, removeNext(infix)- 48);
            }
            signal=1;
        }
        else{
            while ((infix->d == "0") || (infix->d == "1") || (infix->d == "2") || (infix->d == "3") || (infix->d == "4") || (infix->d == "5") || (infix->d == "6") || (infix->d == "7") || (infix->d == "8") || (infix->d == "9")){
                addNext(num_append_head, removeNext(infix)- 48);
            }
            int number=0;
            int product=1;
            while (num_append_head->next != NULL){
                int num_fragment=removeNext(num_append_head)-48; //char->int 변환
                number=number+ num_fragment*product;
                product*=10;   
            }
            //숫자를 number로 묶어서 한 덩어리로 만들기는 했다. 그런데 이거를 postfix 스택에 추가할 수 있나? postfix스택은 한 노드가 int타입데이터만 저장 가능. 아 가능하구나.
            addNext(postfix_head, number);
        }
    }
    return postfix_head;
}


//덧셈

//뺄셈

//곱셈

//나눗셈

int main(void){
    FILE *ifp, *ofp;

    ifp = fopen("input_file","r"); //입력 파일 받음. 파일이름은 나중에 수정.
    ofp = fopen("output_file","w"); //입력 파일 받음. 파일이름은 나중에 수정.
    
    //들어온 식을 스택에 저장.
    LINK infix_notation;
    infix_notation=string_to_list(ifp);

    //중위 표기법-> 후위 표기법
    LINK postfix_notation;
    postfix_notation=infix_to_postfix(infix_notation);

    //스택 사용해서 계산

    //소수점 보정

    //계산 결과 프린트

    //다시 입력 받기
    //...
}


//소수를 정수로 바꿔주는 함수

//계산 결과를 소수로 보정해주는 함수

//식은 문자열로 오는데, 그 안에 잘못 입력된 애도 있고, 공백 문자도 있고, 연산자, 피연산자가 있다. 예외처리할지, 제거할 건 제거할지 결정하고 프로그래밍하자.