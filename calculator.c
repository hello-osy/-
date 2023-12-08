#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef char DATA;
struct linked_list{
    DATA d;
    struct linked_list *next;
};
typedef struct linked_list ELEMENT;
typedef ELEMENT              *LINK;

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

LINK input_to_list(void);
LINK infix_to_postfix(LINK infix);
LINK calculate_postfix(LINK postfix);
LINK Addition(LINK NUM1, LINK NUM2);

int main(void){
    LINK infix = input_to_list();
    LINK postfix = infix_to_postfix(infix);
    LINK result = calculate_postfix(postfix); //calculate_postfix에서 계산 결과를 스택에 저장하고, 출력해서 우리에게 보여줘야 함. (출력하는 함수를 따로 만들어도 됨.)
}

//사실 덧셈이 해결되면 곱셈, 뺄셈도 같이 구현된 것이나 마찬가지임.

//연결리스트 쓰고 나면 메모리 반납하는 코드 다 넣어줘야 함.

LINK input_to_list(void){
    struct NODE *input_head = malloc(sizeof(struct NODE));
    input_head->next=NULL;

    FILE* file = fopen("math_expression.txt", "r"); //읽기 및 쓰기 모드.
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    char expression_char;
    while((expression_char = fgetc(file)) != EOF){
        addNext(input_head, expression_char);
        printf("%c", expression_char);
    }
    fclose(file);
    return input_head;
}

LINK infix_to_postfix(LINK infix){
    struct NODE *temp_head = malloc(sizeof(struct NODE));
    temp_head->next=NULL;

    struct NODE *postfix_head = malloc(sizeof(struct NODE));
    postfix_head->next=NULL;

    int signal=0;
    while (infix->next != NULL){ //수정해야함. 마지막 문자가 while문에서 처리되지 않음.
        if (infix->d == '('){//char을 비교하므로 '으로 감싸야 함.
            addNext(postfix_head, ' '); //숫자가 아닌 애들은 띄어쓰기로 구분하기 위해서 넣은 코드임.
            addNext(temp_head, removeNext(infix));
            signal=0;
        }
        else if (infix->d == ')'){
            while (temp_head->data != '('){
                addNext(postfix_head, removeNext(temp_head));
            }
            signal=0;
        }
        else if((infix->d == '*') || (infix->d == '/')){
            addNext(postfix_head, ' ');
            if (signal<2){
                addNext(temp_head, removeNext(infix));
            }
            else{
                addNext(postfix_head, removeNext(temp_head));
                addNext(temp_head, removeNext(infix));
            }
            signal=2;
        }
        else if((infix->d == '+') || (infix->d == '-')){
            addNext(postfix_head, ' ');
            if (signal<1){
                addNext(temp_head, removeNext(infix));
            }
            else{
                addNext(postfix_head, removeNext(temp_head));
                addNext(temp_head, removeNext(infix));
            }
            signal=1;
        }
        else if ((infix->d == '0') || (infix->d == '1') || (infix->d == '2') || (infix->d == '3') || (infix->d == '4') || (infix->d == '5') || (infix->d == '6') || (infix->d == '7') || (infix->d == '8') || (infix->d == '9')){
            addNext(postfix_head, removeNext(infix));
        } //일단 공백 입력은 처리했음. //오타도 처리했음.
        else if(infix->d == '.'){
            addNext(postfix_head, removeNext(infix));
        } //소숫점도 일단 그대로 가져옴.
    }
    return postfix_head;
}
//후위표기식에서 숫자들을 띄어쓰기로 구분했다.

//연산 과정
LINK calculate_postfix(LINK postfix){
    struct NODE *temp1_head = malloc(sizeof(struct NODE));
    temp1_head->next=NULL;

    struct NODE *temp2_head = malloc(sizeof(struct NODE));
    temp2_head->next=NULL;

    while(postfix->next!=NULL){//수정해야함. 마지막 문자를 처리할 수 없음.
        int signal=1;
        if ((postfix->d == '.') ||(postfix->d == '0') || (postfix->d == '1') || (postfix->d == '2') || (postfix->d == '3') || (postfix->d == '4') || (postfix->d == '5') || (postfix->d == '6') || (postfix->d == '7') || (postfix->d == '8') || (postfix->d == '9')){
            if (signal==1){
                while(postfix->d != ' '){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp1_head,removeNext(postfix)); //첫 번째 숫자 덩어리
                    signal=2;
                }
            }
            else if (signal==2){
                while(postfix->d != ' '){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp2_head,removeNext(postfix)); //두 번째 숫자 덩어리
                    signal=1;
                }
            }
        }
        else if((postfix->d == '+')){
            return Addition(temp1_head, temp2_head);
        }
    }
}

//이 부분은 addition.c대신에 작성한 부분임.
LINK Addition(LINK NUM1, LINK NUM2){
    //문제가 있음. 가장 작은 자리부터 더해야하는데 가장 큰 자리부터 더하는 코드임.
    int over_ten_num=0;
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    while(true){
        if (NUM1 ->d == ' '){
            removeNext(NUM1);
        }
        else if((NUM1 ->d == '0') || (NUM1 ->d == '1') || (NUM1 ->d == '2') || (NUM1 ->d == '3') || (NUM1 ->d == '4') || (NUM1 ->d == '5') || (NUM1 ->d == '6') || (NUM1 ->d == '7') || (NUM1 ->d == '8') || (NUM1 ->d == '9')){
            if (NUM2 ->d == ' '){
                removeNext(NUM1);
            }
            else if((NUM2 ->d == '0') || (NUM2 ->d == '1') || (NUM2 ->d == '2') || (NUM2 ->d == '3') || (NUM2 ->d == '4') || (NUM2 ->d == '5') || (NUM2 ->d == '6') || (NUM2 ->d == '7') || (NUM2 ->d == '8') || (NUM2 ->d == '9')){
                int temp_result=((NUM1->d)-'0') + ((NUM2->d)-'0') + over_ten_num;
                over_ten_num = temp_result/10; //over_ten_num 이게 if문 밖에서도 바뀐 값이 유지되려나?
                int result = temp_result%10;
                removeNext(NUM1);
                removeNext(NUM2);
                addNext(result_head, result);
            }
        }
    }
}