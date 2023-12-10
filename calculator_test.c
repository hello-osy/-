#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct NODE {
    struct NODE *next;
    char data;
};

void addNext(struct NODE *target, char data) {
    struct NODE *newNode = malloc(sizeof(struct NODE));
    newNode->next = target->next;
    newNode->data = data;

    target->next = newNode;
}

char removeNext(struct NODE *target) {
    char data = target->data;
    if (target->next==NULL){ //사실 이렇게 하면 메모리 낭비가 좀 있음.
        target->data=' ';
    }else{
        struct NODE *removeNode = target->next;
        target->next = removeNode->next;
        target->data = removeNode->data;
        free(removeNode);
    }
    return data;
}

struct NODE *input_to_list(void);
struct NODE *infix_to_postfix(struct NODE *infix);
struct NODE *calculate_postfix(struct NODE *postfix);
struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2);
struct NODE *reverseDataOrder(struct NODE *false_head);
void freeLinkedList(struct NODE** head);

int main(void) {
    struct NODE *reversed_infix_head = input_to_list(); //reversed_infix_head는 식의 마지막 문자부터 가리키는 상태임.
    printf("reversed_infix_head generated.\n");
    struct NODE *infix_head = reverseDataOrder(reversed_infix_head); //reversed_infix_head는 메모리 반납됨.
    printf("infix_head generated.\n");

    struct NODE *reversed_postfix_head = infix_to_postfix(infix_head); //reversed_postfix_head는 식의 마지막 문자부터 가리키는 상태임. infix_head는 메모리 반납됨.
    printf("reversed_postfix_head generated.\n");
    struct NODE *postfix_head = reverseDataOrder(reversed_postfix_head); //reversed_postfix_head는 메모리 반납됨.
    printf("postfix_head generated.\n");

    struct NODE *result_head = calculate_postfix(postfix_head); //result_head는 head가 가장 큰 자리수를 가리킴. postfix_head는 메모리 반납됨.
    printf("result_head generated.\n");

    //결과 프린트해보기
    while(result_head!=NULL){
        char print_char = result_head->data;
        if(print_char!=' '){
            printf("%c", print_char);
        }
    }
    return 0;
}

struct NODE *input_to_list(void) {
    struct NODE *input_head = malloc(sizeof(struct NODE));
    input_head->next = NULL;
    input_head->data = ' ';

    FILE *file = fopen("math_expression.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return NULL;
    }

    char expression_char;
    int count=0;
    while ((expression_char = fgetc(file)) != EOF) {
        addNext(input_head, expression_char);
        //printf("%c", expression_char);
        ++count;
    }
    printf("%d counted.\n", count);
    fclose(file);
    return input_head;
}

struct NODE *infix_to_postfix(struct NODE *infix) {
    struct NODE *temp_head = malloc(sizeof(struct NODE));
    temp_head->next = NULL;
    temp_head->data = ' ';

    struct NODE *postfix_head = malloc(sizeof(struct NODE));
    postfix_head->next = NULL;
    postfix_head->data = ' ';

    int signal = 0;
    while (true) {
        if (infix->data == '('){//char을 비교하므로 '으로 감싸야 함.
            addNext(postfix_head, ' '); //숫자가 아닌 애들은 띄어쓰기로 구분하기 위해서 넣은 코드임.
            addNext(temp_head, removeNext(infix));
            printf("\n( was found.\n");
            signal=0;
        }
        else if (infix->data == ')'){
            addNext(postfix_head, ' ');
            printf("\n) was found.\n");
            while(true){
                while(temp_head->data==' '){
                    temp_head==temp_head->next; //temp_head 앞쪽의 ' '를 건너뛰기 위한 코드
                }
                while(temp_head->data!='('){
                    addNext(postfix_head, removeNext(temp_head));
                }
                removeNext(temp_head); //temp_head의 (를 지움.(혹은 ' '으로 만듦)
            }
            removeNext(infix);
            signal=0;
        }
        else if((infix->data == '*') || (infix->data == '/')){
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
        else if((infix->data == '+') || (infix->data == '-')){
            addNext(postfix_head, ' ');
            printf("\n+ or - was found.\n");
            if (signal<1){
                addNext(temp_head, removeNext(infix));
            }
            else{
                addNext(postfix_head, removeNext(temp_head));
                addNext(temp_head, removeNext(infix));
            }
            signal=1;
        }
        else if ((infix->data == '0') || (infix->data == '1') || (infix->data == '2') || (infix->data == '3') || (infix->data == '4') || (infix->data == '5') || (infix->data == '6') || (infix->data == '7') || (infix->data == '8') || (infix->data == '9')){
            addNext(postfix_head, removeNext(infix));
            printf("a number was found.");
        } //일단 공백 입력은 처리했음. //오타도 처리했음.
        else if(infix->data == '.'){
            addNext(postfix_head, removeNext(infix));
        } //소숫점도 일단 그대로 가져옴.
        else{//쓸데없는 애들 처리하는 부분 (공백 문자도 포함.)
            removeNext(infix);
            printf("\n trash was found.");
            
            if(infix->data=' '){
                if (infix->next == NULL){
                    break;
                }
            }
        }
    }
    printf("\n2");
    freeLinkedList(&infix);
    freeLinkedList(&temp_head);
    return postfix_head;
}

struct NODE *calculate_postfix(struct NODE *postfix) { //일단 1회 연산만 가능한 상태로 짰음.
    struct NODE *temp1_head = malloc(sizeof(struct NODE));
    temp1_head->next = NULL;
    temp1_head->data = ' ';

    struct NODE *temp2_head = malloc(sizeof(struct NODE));
    temp2_head->next = NULL;
    temp2_head->data = ' ';

    int signal=1;
    while(true){
        if ((postfix->data == '.') ||(postfix->data == '0') || (postfix->data == '1') || (postfix->data == '2') || (postfix->data == '3') || (postfix->data == '4') || (postfix->data == '5') || (postfix->data == '6') || (postfix->data == '7') || (postfix->data == '8') || (postfix->data == '9')){
            if (signal==1){
                while(postfix->data != ' '){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp1_head,removeNext(postfix)); //첫 번째 숫자 덩어리
                    signal=2;
                }
            }
            else if (signal==2){
                while(postfix->data != ' '){ //공백 나오기 전까지. 그러니까 하나의 숫자 덩어리를 처리하는 부분임.
                    addNext(temp2_head,removeNext(postfix)); //두 번째 숫자 덩어리
                    signal=1;
                }
            }
        }
        else if((postfix->data == '+')){ //여기를 못 들어오는 상태임.
            printf("\n3");
            removeNext(postfix);
            struct NODE *addition_result=Addition(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);
            freeLinkedList(&postfix);
            printf("\n3");
            return addition_result;
        }
        else{
            if (postfix->next==NULL){ //들어오자마자 여길 통해서 탈출해버림;
                break;
            }else{
                postfix=postfix->next;
            }
        }
    }
    return NULL;
}

struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2) {
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어와서 괜찮음 이대로 해도 됨.
    int over_ten_num=0;
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    result_head->data = ' ';

    while(true){
        if (NUM1 ->data == ' '){
            removeNext(NUM1);
        }
        else if((NUM1 ->data == '0') || (NUM1 ->data == '1') || (NUM1 ->data == '2') || (NUM1 ->data == '3') || (NUM1 ->data == '4') || (NUM1 ->data == '5') || (NUM1 ->data == '6') || (NUM1 ->data == '7') || (NUM1 ->data == '8') || (NUM1 ->data == '9')){
            if (NUM2 ->data == ' '){
                removeNext(NUM2);
            }
            else if((NUM2 ->data == '0') || (NUM2 ->data == '1') || (NUM2 ->data == '2') || (NUM2 ->data == '3') || (NUM2 ->data == '4') || (NUM2 ->data == '5') || (NUM2 ->data == '6') || (NUM2 ->data == '7') || (NUM2 ->data == '8') || (NUM2 ->data == '9')){
                int temp_result=((NUM1->data)-'0') + ((NUM2->data)-'0') + over_ten_num;
                over_ten_num = temp_result/10; //over_ten_num 이게 if문 밖에서도 바뀐 값이 유지되려나?
                int result = temp_result%10;
                removeNext(NUM1);
                removeNext(NUM2);
                addNext(result_head, result);
            }
        }
        
        if((NUM1 -> next == NULL) && (NUM2 -> next == NULL)){
            return result_head;
        }
    }
}

struct NODE *reverseDataOrder(struct NODE *false_head){
    struct NODE *true_head = malloc(sizeof(struct NODE));
    true_head->next=NULL;
    true_head->data=' ';

    while(true){
        addNext(true_head, removeNext(false_head));
        if (false_head->data==' '){
            if(false_head->next==NULL){
                break;
            }
        }
    } //false_head를 true_head로 순서 바꿔서 복사함.
    freeLinkedList(&false_head);

    return true_head;
}

void freeLinkedList(struct NODE** head) { //받은 부분부터 NULL까지의 연결리스트 부분의 메모리를 반납해줌.(자동으로 맨 처음부터 지우는 것이 아니다. 우리가 맨 처음 부분을 넣어줘야 처음부터 지움.)
    struct NODE* current = *head;
    struct NODE* next;

    while (current != NULL) {
        next = current->next;
        free(current); // 노드를 삭제하고 메모리를 해제합니다.
        current = next;
    }

    *head = NULL; // head를 NULL로 설정하여 리스트를 빈 상태로 만듭니다.
}

//아 설마? 다 더한 다음에 head가 마지막 노드인 상태로 리턴하는 건가?