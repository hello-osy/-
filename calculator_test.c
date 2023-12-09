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
void *PrintData(struct NODE *printlist_head);

int main(void) {
    struct NODE *infix_head = input_to_list();
    printf("\ntest_1");
    struct NODE *postfix_head = infix_to_postfix(infix_head);
    printf("\ntest_2");
    struct NODE *result_head = calculate_postfix(postfix_head);
    printf("\ntest_3");
    // Handle result or perform further actions here
    PrintData(result_head);
    // Free memory if needed
    // Remember to free the allocated memory
    free(infix_head);
    printf("\ntest_4");
    free(postfix_head);
    printf("\ntest_5");
    free(result_head);
    printf("\ntest_end");
    return 0;
}

struct NODE *input_to_list(void) {
    struct NODE *input_head = malloc(sizeof(struct NODE));
    input_head->next = NULL;

    FILE *file = fopen("math_expression.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return NULL;
    }

    char expression_char;
    while ((expression_char = fgetc(file)) != EOF) {
        addNext(input_head, expression_char);
        printf("%c", expression_char);
    }
    fclose(file);
    printf("1");
    return input_head;
}

struct NODE *infix_to_postfix(struct NODE *infix) {
    struct NODE *temp_head = malloc(sizeof(struct NODE));
    temp_head->next = NULL;

    struct NODE *postfix_head = malloc(sizeof(struct NODE));
    postfix_head->next = NULL;

    int signal = 0;
    while (true) {
        if (infix->data == '('){//char을 비교하므로 '으로 감싸야 함.
            addNext(postfix_head, ' '); //숫자가 아닌 애들은 띄어쓰기로 구분하기 위해서 넣은 코드임.
            addNext(temp_head, removeNext(infix));
            signal=0;
        }
        else if (infix->data == ')'){
            while (temp_head->data != '('){
                addNext(postfix_head, removeNext(temp_head));
            }
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
        } //일단 공백 입력은 처리했음. //오타도 처리했음.
        else if(infix->data == '.'){
            addNext(postfix_head, removeNext(infix));
        } //소숫점도 일단 그대로 가져옴.
        if (infix->next==NULL){
            break;
        }
        infix=infix->next;
        //printf("\n."); //현재는 무한 루프 상태인 것으로 보임.
    }
    printf("2");
    free(temp_head);
    return postfix_head;
}

struct NODE *calculate_postfix(struct NODE *postfix) {
    struct NODE *temp1_head = malloc(sizeof(struct NODE));
    temp1_head->next = NULL;

    struct NODE *temp2_head = malloc(sizeof(struct NODE));
    temp2_head->next = NULL;

    while(postfix->next!=NULL){//수정해야함. 마지막 문자를 처리할 수 없음.
        int signal=1;
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
        else if((postfix->data == '+')){
            printf("3");
            return Addition(temp1_head, temp2_head);//temp1,temp2 반납 언젠가는 해야 함.
        }
    }
}

struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2) {
    //문제가 있음. 가장 작은 자리부터 더해야하는데 가장 큰 자리부터 더하는 코드임.
    int over_ten_num=0;
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    while(true){
        if((NUM1 -> next == NULL) || (NUM2 -> next == NULL)){ //마지막 문자를 처리할 수 없음. 수정해야 함.
            return result_head;
        }
        else if (NUM1 ->data == ' '){
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
    }
}

void *PrintData(struct NODE *printlist_head){
    struct NODE *search_node = printlist_head;
    while(search_node->next!=NULL){ //마지막 문자를 처리할 수 없음. 수정해야 함.
        if(search_node->data != ' '){
            char print_char = search_node->data;
            printf("%c",print_char);
        }
        search_node=printlist_head->next;
    }
    printf("\n");
}