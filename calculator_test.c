#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char *readFile(void);
struct NODE *input_to_list(void);
struct NODE *infix_to_postfix(struct NODE *infix);
struct NODE *calculate_postfix(struct NODE *postfix);
struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2);
struct NODE *Subtraction(struct NODE *NUM1, struct NODE *NUM2);
struct NODE *Multiplication(struct NODE *NUM1, struct NODE *NUM2);
struct NODE *reverseDataOrder(struct NODE *false_head);
void freeLinkedList(struct NODE** head);
struct NODE *copyLinkedList(struct NODE *head);
void printLinkedList(struct NODE* head);

int main(void) {
    struct NODE *reversed_infix_head = input_to_list(); //reversed_infix_head는 식의 마지막 문자부터 가리키는 상태임.
    printf("reversed_infix_head generated.\n");
    struct NODE *infix_head = reverseDataOrder(reversed_infix_head); //reversed_infix_head는 메모리 반납됨.
    printf("infix_head generated.\n");

    printf("infix_head : ");
    printLinkedList(infix_head);
    
    struct NODE *reversed_postfix_head = infix_to_postfix(infix_head); //reversed_postfix_head는 식의 마지막 문자부터 가리키는 상태임. infix_head는 메모리 반납됨.
    printf("reversed_postfix_head generated.\n");
    struct NODE *postfix_head = reverseDataOrder(reversed_postfix_head); //reversed_postfix_head는 메모리 반납됨.
    printf("postfix_head generated.\n");

    printf("postfix_head : ");
    printLinkedList(postfix_head);
    
    //이 주석 아래 부분만 해결하면 됨.
    
    struct NODE *result_head = calculate_postfix(postfix_head); //result_head는 head가 가장 큰 자리수를 가리킴. postfix_head는 메모리 반납됨.
    printf("result_head generated.\n");
    
    //결과 프린트해보기
    printf("result_head : ");
    while(result_head!=NULL){
        char print_char = result_head->data;
        printf("%c", print_char);
        result_head=result_head->next;        
    }
    printf("\nresult_head printed.");
    
    return 0;
    
}

char *readFile(void) {
    FILE *file = fopen("math_expression.txt","r");
    if (file == NULL) {
        printf("Error: 파일을 열 수 없습니다.\n");
        exit(0);
    }

    char chr;
    char *function = NULL;
    int size = 1;
    
    while ((chr = fgetc(file)) != EOF) {
        size++;
    }

    function = malloc(size);

    rewind(file); // 파일을 처음부터 다시 읽음
    
    int chr_asci;
    int isnum = 0; // 들어온 chr 값(숫자면 0, 사칙연산 1, 여는 괄호 2, 닫는 괄호 3, 소수점 4)
    int is_open = 0, i = 0, dot = 0, isdiv = 0;
    
    while((chr = fgetc(file)) != EOF) {
        chr_asci = chr - '0';
        if (chr_asci < 0) { // chr가 숫자가 아닐 때
            if (chr_asci == -1) {
                isdiv = 1;
            }
            else if (chr_asci == -8) { // 여는 괄호일 때
                is_open++;
                isnum = 2;
            }
            else if (chr_asci == -7) { // 닫는 괄호일 때
                if (is_open != 1) { // 괄호가 열리지 않았거나 괄호가 다 닫히지 않았을 때
                    printf("Error: 괄호의 개수가 올바르지 않습니다.");
                    free(function);
                    exit(1);
                }
                else if (isnum == 1) { // 연산자 다음 바로 괄호가 나왔을 때
                    printf("Error: 연산자 뒤에 바로 괄호를 닫을 수 없습니다.");
                    free(function);
                    exit(1);
                }
                else { // 여는 괄호 뒤에 나왔을 때
                    is_open--;
                    isnum = 3;
                    dot--;
                }
            }
            else if (chr_asci == -2) { // 소수점일 때
                if (isnum != 0) {
                    printf("Error: 소수점의 위치가 올바르지 않습니다.");
                    free(function);
                    exit(1);
                }
                else { 
                    if (dot >1) { // 숫자 하나에 소수점이 두 개 이상일 때
                        printf("Error: 피연산자에 소수점이 2개 이상일 수 없습니다.");
                        free(function);
                        exit(1);
                    }
                    else {
                        isnum = 4;
                        dot++;
                    }
                }
            }
            else if (chr_asci > -9) { // 사칙연산일 때
                if (isnum != 1 && isnum != 4) {
                    isnum = 1;
                    dot--;
                }
                
                else {
                    printf("Error: 연산자가 연달아 나올 수 없습니다.");
                    free(function);
                    exit(1);
                }
            }
            else {} // 공백 등 연산자 제외 패스
        }
        else { // 숫자가 나온 경우
            if (isdiv) { // 나눗셈인 경우
                if (chr_asci == 0) { // 0으로 나누려 하는 경우
                    printf("Error: 0으로 나눌 수 없습니다.");
                    free(function);
                    exit(1);
                }
                else {
                    printf("Error: 나눗셈이 구현되지 않았습니다.");
                    free(function);
                    exit(1);
                }
            }
            isnum = 0;
        }
        *(function + i) = chr;
        i++;
    }

    if (isnum == 1) { // 연산자로 끝나는 경우
        printf("Error: 식이 완전하지 않습니다.");
        free(function);
        exit(1);
    }
    else if (is_open != 0) { // 괄호가 닫히지 않은 채 식이 끝난 경우
        printf("Error: 괄호가 닫하지 않았습니다.");
        free(function);
        exit(1);
    }
    fclose(file);
    printf("파일 읽기 종료");
    *(function + i) = '\0'; // 파일 마지막에 \0 추가

    char *final_func = malloc(size);
    int n = 0, j = 0;
    while (n < size) {
        if (function[n] == ')' && function[n+1] == '(') {
            final_func[n+j++] = ')';
            final_func[n+j] = '*';
        } else {
            final_func[n+j] = *(function + n);
        }
        n++;
    }

    // 정상작동 파일 작성
    FILE *f = fopen("normalfile.txt","w");
    fprintf(f, "%s", final_func);
    free(function);
    free(final_func);
    fclose(f);
    printf("파일 작성 완료");
    return "normalfile.txt";
}

struct NODE *input_to_list(void) {
    struct NODE *input_head = malloc(sizeof(struct NODE));
    input_head->next = NULL;
    input_head->data = ' ';

    char *fp = readFile();
    FILE *file = fopen(fp, "r");

    char expression_char;
    while ((expression_char = fgetc(file)) != EOF) {
        addNext(input_head, expression_char);
    }
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
    int count =0;
    int trash_cnt=0;
    while (true) {
        ++count;
        if (infix->data == '('){//char을 비교하므로 '으로 감싸야 함.
            addNext(postfix_head, ' '); //숫자가 아닌 애들은 띄어쓰기로 구분하기 위해서 넣은 코드임.
            addNext(temp_head, removeNext(infix));
            signal=0;
        }
        else if (infix->data == ')'){
            while(temp_head->data != '('){
                if (temp_head->data != ' '){
                    addNext(postfix_head, ' ');
                    addNext(postfix_head, removeNext(temp_head));
                    printf("something moved to postfix_head\n");
                }else{
                    removeNext(temp_head);
                }
            }


            if (temp_head->data == '(') {
                removeNext(temp_head); // '('를 지움
            }
            removeNext(infix);
            signal=0;
        }
        else if((infix->data == '*') || (infix->data == '/')){
            addNext(postfix_head, ' ');
            printf("\n* or / was found.\n");
            if (signal<2){
                addNext(temp_head, removeNext(infix));
            }
            else{
                while(temp_head->data==' '&&temp_head->next!=NULL){
                    removeNext(temp_head);
                }
                addNext(postfix_head, removeNext(temp_head));
                addNext(postfix_head,' ');
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
                while(temp_head->data==' '&&temp_head->next!=NULL){
                    removeNext(temp_head);
                }
                addNext(postfix_head, removeNext(temp_head));
                addNext(postfix_head,' ');
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
        else{//쓸데없는 애들 처리하는 부분 (공백 문자도 포함.)
            removeNext(infix);
            ++trash_cnt;
            //printf("\n trash was found.");
            
            if((infix->data==' ') && (infix->next == NULL)){//infix 마지막 까지 간 경우임.
                if (temp_head->next !=NULL){ //temp_head에 데이터가 들어있는 경우 데이터를 postfix_head로 넘겨준다.
                    while (true){
                        if(temp_head->data!=' '){
                            addNext(postfix_head, ' ');
                            addNext(postfix_head, removeNext(temp_head));
                        }else{
                            removeNext(temp_head);
                        }

                        if(temp_head->data==' ' && temp_head->next==NULL){
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    printf("infix_to_postfix iterated %d times.\n",count);
    printf("infix_to_postfix has %d trashes.\n",trash_cnt);
    freeLinkedList(&infix);
    freeLinkedList(&temp_head);
    //printLinkedList(postfix_head);
    return postfix_head;
}

struct NODE *calculate_postfix(struct NODE *postfix) {
    struct NODE *temp1_head = malloc(sizeof(struct NODE));
    temp1_head->next = NULL;
    temp1_head->data = ' ';

    struct NODE *temp2_head = malloc(sizeof(struct NODE));
    temp2_head->next = NULL;
    temp2_head->data = ' ';

    int signal = 1;
    printf("calculate_postfix entered.\n");

    struct NODE *result = malloc(sizeof(struct NODE));
    result->next = NULL;
    result->data = ' ';

    while (true) {
        if ((postfix->data == '.') || (postfix->data == '0') || (postfix->data == '1') || (postfix->data == '2') || (postfix->data == '3') || (postfix->data == '4') || (postfix->data == '5') || (postfix->data == '6') || (postfix->data == '7') || (postfix->data == '8') || (postfix->data == '9')) {
            if (signal == 1) {
                printf("signal 1 entered.\n");
                while (true) {
                    addNext(temp1_head, removeNext(postfix));
                    if ((postfix->data == ' ')){
                        break;
                    }
                }
                signal = 2;
            } else if (signal == 2) {
                printf("signal 2 entered.\n");
                while (true) {
                    addNext(temp2_head, removeNext(postfix));
                    if ((postfix->data == ' ')){
                        break;
                    }
                }
                signal = 1;
            }
        } else if (postfix->data == '+') {
            removeNext(postfix);
            printf("addition ready.\n");
            struct NODE *addition_result = Addition(temp1_head, temp2_head);
            
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(addition_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
            printf("addition completed.\n");
        } else if (postfix->data == '-') {
            removeNext(postfix);
            printf("subtraction ready.\n");
            struct NODE *subtraction_result = Subtraction(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(subtraction_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
            printf("subtraction completed.\n");
        } else if (postfix->data == '*') {
            removeNext(postfix);
            printf("multiplication ready.\n");
            struct NODE *multiplication_result = Multiplication(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(multiplication_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
            printf("multiplication completed.\n");
        } else {
            removeNext(postfix);
        }

        if(postfix->next==NULL && postfix->data==' '){
            result = temp1_head; //이게 문제일 수도?
            break;
        }
    }
    freeLinkedList(&postfix);
    result=reverseDataOrder(result); //result는 마지막 temp1_head인데, temp_head는 뒤집힌 상태임. 원래 상태로 만들어준 것임.
    while((result->data==' ') || (result->data=='0')){
        removeNext(result);
    }
    return result;
}

struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2) {
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어와서 괜찮음 이대로 해도 됨.
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    result_head->data = ' ';
    
    struct NODE *n1 = copyLinkedList(NUM1);
    struct NODE *n2 = copyLinkedList(NUM2);
    while(n1->data==' '){
        removeNext(n1);
    }
    while(n2->data==' '){
        removeNext(n2);
    }

    int cnt1=0;
    int dot_cnt_num1=0;
    while(n1->data!=' '){
        if(n1->data=='.'){
            dot_cnt_num1=cnt1;
        }
        removeNext(n1);
        ++cnt1;
    }
    int cnt2=0;
    int dot_cnt_num2=0;
    while(n2->data!=' '){
        if(n2->data=='.'){
            dot_cnt_num2=cnt2;
        }
        removeNext(n2);
        ++cnt2;
    }
    freeLinkedList(&n1);
    freeLinkedList(&n2);

    if((dot_cnt_num1!=0) || (dot_cnt_num2!=0)){
        if(dot_cnt_num1>dot_cnt_num2){
            int length = dot_cnt_num1-dot_cnt_num2;
            if (dot_cnt_num2==0){
                addNext(NUM2, '.');
            }
            while(length>0){
                addNext(NUM2, '0');
                --length;
            }
        } else if(dot_cnt_num1<dot_cnt_num2){
            int length = dot_cnt_num2-dot_cnt_num1;
            if (dot_cnt_num1==0){
                addNext(NUM1, '.');
            }
            while(length>0){
                addNext(NUM1, '0');
                --length;
            }
        }
    }

    while((NUM1 ->data == ' ') || (NUM2 ->data == ' ')){
        if (NUM1 ->data == ' '){
            removeNext(NUM1);
        }
        if (NUM2 ->data == ' '){
            removeNext(NUM2);
        }
    }

    int over_ten_num=0;
    while(true){
        printf("addition entered.\n");
        int num1 = 0, num2 = 0;
        if ((NUM1->data == '.') && (NUM2->data == '.')) {
            removeNext(NUM1);
            removeNext(NUM2);
            num1 = NUM1->data - '0';
            num2 = NUM2->data - '0';
            int temp_result=num1 + num2 + over_ten_num;
            over_ten_num = temp_result/10; //over_ten_num 이게 if문 밖에서도 바뀐 값이 유지되려나?
            int result = temp_result%10;
            removeNext(NUM1);
            removeNext(NUM2);
            char result_char = result+ '0';
            addNext(result_head, '.');
            addNext(result_head, result_char);
        }else{
            if ((NUM1->data == ' ') && (NUM2->data == ' ')) {
                return result_head;
            } else if ((NUM1->data != ' ') && (NUM2->data != ' ')) {
                num1 = NUM1->data - '0';
                num2 = NUM2->data - '0';
            } else if ((NUM1->data == ' ') && (NUM2->data != ' ')) {
                num2 = NUM2->data - '0';
            } else if ((NUM1->data != ' ') && (NUM2->data == ' ')) {
                num1 = NUM1->data - '0';
            }

            int temp_result=num1 + num2 + over_ten_num;
            over_ten_num = temp_result/10; //over_ten_num 이게 if문 밖에서도 바뀐 값이 유지되려나?
            int result = temp_result%10;
            removeNext(NUM1);
            removeNext(NUM2);
            char result_char = result+ '0';
            addNext(result_head, result_char);
        }
    }
}
struct NODE *Subtraction(struct NODE *NUM1, struct NODE *NUM2){
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어옴.
    //소수 간의 계산 처리해야 함.
    
    struct NODE *n1 = copyLinkedList(NUM1);
    struct NODE *n2 = copyLinkedList(NUM2);
    while(n1->data==' '){
        removeNext(n1);
    }
    while(n2->data==' '){
        removeNext(n2);
    }

    int cnt1=0;
    int dot_cnt_num1=0;
    while(n1->data!=' '){
        if(n1->data=='.'){
            dot_cnt_num1=cnt1;
        }
        removeNext(n1);
        ++cnt1;
    }
    int cnt2=0;
    int dot_cnt_num2=0;
    while(n2->data!=' '){
        if(n2->data=='.'){
            dot_cnt_num2=cnt2;
        }
        removeNext(n2);
        ++cnt2;
    }
    freeLinkedList(&n1);
    freeLinkedList(&n2);

    if((dot_cnt_num1!=0) || (dot_cnt_num2!=0)){
        if(dot_cnt_num1>dot_cnt_num2){
            int length = dot_cnt_num1-dot_cnt_num2;
            if (dot_cnt_num2==0){
                addNext(NUM2, '.');
            }
            while(length>0){
                addNext(NUM2, '0');
                --length;
            }
        } else if(dot_cnt_num1<dot_cnt_num2){
            int length = dot_cnt_num2-dot_cnt_num1;
            if (dot_cnt_num1==0){
                addNext(NUM1, '.');
            }
            while(length>0){
                addNext(NUM1, '0');
                --length;
            }
        }
    }
    
    printLinkedList(NUM1);
    printLinkedList(NUM2);
    struct NODE *search1_node = copyLinkedList(NUM1);
    struct NODE *search2_node = copyLinkedList(NUM2);
    //printLinkedList(search1_node); 
    //printLinkedList(search2_node); //NUM1,NUM2 잘 들어옴.
    int num1_cnt=0; //연결리스트의 노드 개수를 세는 것임(수의 길이와 조금 차이가 있는 값임.)
    int num2_cnt=0;
    while(true){
        ++num1_cnt;
        removeNext(search1_node);
        if (search1_node->next==NULL && search1_node->data==' '){
            break;
        }
    }
    while(true){
        ++num2_cnt;
        removeNext(search2_node);
        if (search2_node->next==NULL && search2_node->data==' '){
            break;
        }
    }
    freeLinkedList(&search1_node);
    freeLinkedList(&search2_node);
    //printLinkedList(NUM1);
    //printLinkedList(NUM2);
    
    bool change_calculation_order = false;
    if (num1_cnt==num2_cnt){
        struct NODE *search1_node = reverseDataOrder(copyLinkedList(NUM1));//copyLinkedList(reverseDataOrder(NUM1)); 하면 오류남. reverseDataOrder하면 넣은 값이 갈려버림.
        struct NODE *search2_node = reverseDataOrder(copyLinkedList(NUM2));
        while(search1_node->data==search2_node->data){
            removeNext(search1_node);
            removeNext(search2_node);
            if (search1_node->next==NULL && search1_node->data==' '){
                break;
            }
        }
        int n1=(search1_node->data)-'0';
        int n2=(search2_node->data)-'0';
        if(n1>n2){//큰 수가 앞에 있을 경우, 자리 안 바꿈
            change_calculation_order=false;
        } else if(n1<n2){ //큰 수가 뒤에 있을 경우, 자리 바꿈
            change_calculation_order=true;
        }
        freeLinkedList(&search1_node);
        freeLinkedList(&search2_node);
    }
    
    while(NUM1->data==' '){
        removeNext(NUM1);
    }
    while(NUM2->data==' '){
        removeNext(NUM2);
    }

    if ((num1_cnt > num2_cnt) || ((!change_calculation_order)&&(num1_cnt==num2_cnt))) { // 자리 안 바꾸는 경우 || (!change_calculation_order)
        printf("calculation_order not changed\n");
        int subtract_next = 0;
        struct NODE *result_head = malloc(sizeof(struct NODE));
        result_head->next = NULL;
        result_head->data = ' ';

        while (true) {
            if ((NUM1->data == '.')&&(NUM2->data == '.')){
                removeNext(NUM1);
                removeNext(NUM2);
                int num1 = 0, num2 = 0;
                if (NUM1->data != ' ') {
                    num1 = NUM1->data - '0';
                }
                if (NUM2->data != ' ') {
                    num2 = NUM2->data - '0';
                }
                int temp_result = num1 - num2 + subtract_next;
                if (temp_result < 0) { //윗 자리에서 10 빌려온다.
                    subtract_next = -1;
                    temp_result = 10 + temp_result;
                } else {
                    subtract_next = 0;
                }
                //printf("result_head added.\n");
                addNext(result_head, '.');
                addNext(result_head, temp_result + '0');

                if (NUM1->data != ' ') {
                    removeNext(NUM1);
                }
                if (NUM2->data != ' ') {
                    removeNext(NUM2);
                }
            } else if ((NUM1->data == ' ') && (NUM2->data == ' ')){
                break;
            } else{
                int num1 = 0, num2 = 0;
                if (NUM1->data != ' ') {
                    num1 = NUM1->data - '0';
                }
                if (NUM2->data != ' ') {
                    num2 = NUM2->data - '0';
                }
                int temp_result = num1 - num2 + subtract_next;
                if (temp_result < 0) { //윗 자리에서 10 빌려온다.
                    subtract_next = -1;
                    temp_result = 10 + temp_result;
                } else {
                    subtract_next = 0;
                }
                //printf("result_head added.\n");
                addNext(result_head, temp_result + '0');

                if (NUM1->data != ' ') {
                    removeNext(NUM1);
                }
                if (NUM2->data != ' ') {
                    removeNext(NUM2);
                }
            }
        }
        return result_head;
    } else if ((num1_cnt < num2_cnt) || ((change_calculation_order)&&(num1_cnt==num2_cnt))) { // 자리 바꾸는 경우
        printf("calculation_order changed\n");
        int subtract_next = 0;
        struct NODE *result_head = malloc(sizeof(struct NODE));
        result_head->next = NULL;
        result_head->data = ' ';

        while (true) {
            if ((NUM2->data == '.')&&(NUM1->data == '.')){
                removeNext(NUM2);
                removeNext(NUM1);
                int num2 = 0, num1 = 0;
                if (NUM2->data != ' ') {
                    num2 = NUM2->data - '0';
                }
                if (NUM2->data != ' ') {
                    num1 = NUM1->data - '0';
                }
                int temp_result = num2 - num1 + subtract_next;
                if (temp_result < 0) { //윗 자리에서 10 빌려온다.
                    subtract_next = -1;
                    temp_result = 10 + temp_result;
                } else {
                    subtract_next = 0;
                }
                //printf("result_head added.\n");
                addNext(result_head, '.');
                addNext(result_head, temp_result + '0');

                if (NUM1->data != ' ') {
                    removeNext(NUM2);
                }
                if (NUM2->data != ' ') {
                    removeNext(NUM1);
                }
            } else if((NUM2->data == ' ') && (NUM1->data == ' ')){
                break;
            } else{
                int num2 = 0, num1 = 0;
                if (NUM2->data != ' ') {
                    num2 = NUM2->data - '0';
                }
                if (NUM1->data != ' ') {
                    num1 = NUM1->data - '0';
                }
                int temp_result = num2 - num1 + subtract_next;
                if (temp_result < 0) { //윗 자리에서 10 빌려온다.
                    subtract_next = -1;
                    temp_result = 10 + temp_result;
                } else {
                    subtract_next = 0;
                }
                //printf("result_head added.\n");
                addNext(result_head, temp_result + '0');

                if (NUM2->data != ' ') {
                    removeNext(NUM2);
                }
                if (NUM1->data != ' ') {
                    removeNext(NUM1);
                }
            }
        }
        
        addNext(result_head,'-'); //순서를 바꿨으니 '-' 붙여준다.
        return result_head;
    }
}

struct NODE *Multiplication(struct NODE *NUM1, struct NODE *NUM2){
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어와서 괜찮음 이대로 해도 됨.
    //소수 간의 계산 처리해야 함.
    //printLinkedList(NUM1);
    //printLinkedList(NUM2); //NUM1,NUM2 잘 들어옴
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    result_head->data = ' '; 

    int count=0;
    while((NUM1->data==' ') || (NUM2->data==' ')){
            if (NUM1 ->data == ' '){
                removeNext(NUM1);
            }
            if (NUM2 ->data == ' '){
                removeNext(NUM2);
            }
        }
    while(true){ //나는 곱셈을 여러 조각으로 나누어서 더하려고 한다.
        printf("multiplication entered.\n");
        struct NODE *num2_search = copyLinkedList(NUM2); //num2_search는 NUM2와 항상 같아야 함.
        struct NODE *temp_result = malloc(sizeof(struct NODE)); //temp_result 초기화 됨.
        temp_result->next=NULL;
        temp_result->data = ' ';

        for (int i=0;i<count;i++){
            addNext(temp_result,'0'); //multiply_num의 자릿수가 올라간 만큼을 temp_result에 반영하기 위한 것임.
        }
        int over_ten_num=0;
        int multiply1_num = (NUM1->data) - '0'; //num2에 곱할 num1의 가장 작은 자리수
        while(true){
            int multiply2_num = (num2_search->data) - '0';
            int temp_frac_result = multiply1_num * multiply2_num + over_ten_num;
            int frac_result= temp_frac_result%10;
            over_ten_num = temp_frac_result/10;
            addNext(temp_result, frac_result+'0'); //한 조각에 해당하는 연산 결과를 temp_result에 저장한다.
            printf("multiply_fragment making...\n");
            removeNext(num2_search);
            if (num2_search->next==NULL && num2_search->data==' '){
                freeLinkedList(&num2_search);
                break;
            }
        }
        if (over_ten_num!=0){
            addNext(temp_result, over_ten_num+'0');
        }
        //printLinkedList(temp_result);
        if (result_head->next==NULL){
            result_head=temp_result;
        }else{
            struct NODE *temp1_head=reverseDataOrder(result_head);
            struct NODE *temp2_head=reverseDataOrder(temp_result);
            struct NODE *addition_result = Addition(temp1_head, temp2_head);//Addition에 들어가는 애들은 뒤집힌 상태여야 함.
            //return addition_result;
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);
            result_head = addition_result;
            //printLinkedList(result_head);
            printf("multiply_fragment added to result_head\n"); //현재 상황을 볼 때, 한 번도 result_head가 업데이트 안 되는 것으로 보임.
        }

        removeNext(NUM1); //가장 작은 자리수 제거
        ++count;
        if(NUM1->next==NULL && NUM1->data==' '){
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
struct NODE *copyLinkedList(struct NODE *head){  //input그대로 output이 만들어짐. 다른 주소를 가리키도록 복사한 것임.
    if (head == NULL) {
        return NULL;
    }

    struct NODE* newHead = malloc(sizeof(struct NODE));
    newHead->data = head->data;
    newHead->next = NULL;

    struct NODE* originalCurrent = head->next;
    struct NODE* newCurrent = newHead;

    while (originalCurrent != NULL) {
        addNext(newCurrent, originalCurrent->data);
        originalCurrent = originalCurrent->next;
        newCurrent = newCurrent->next;
    }

    return newHead;
}

void printLinkedList(struct NODE* head) {
    while (head != NULL) {
        printf("%c", head->data);
        head = head->next;
    }
    printf("\n");
}