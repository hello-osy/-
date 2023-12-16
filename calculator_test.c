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
        printf("%c", print_char);
        result_head=result_head->next;        
    }
    printf("\nresult_head printed.");
    return 0;
    
}

char *readFile(void) {
    FILE *file = fopen("math_expression.txt","r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
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
    int is_open = 0, i = 0;
    
    while((chr = fgetc(file)) != EOF) {
        chr_asci = chr - '0';
        if (chr_asci < 0) { // chr가 숫자가 아닐 때
            if (chr_asci == -8) { // 여는 괄호일 때
                is_open++;
                isnum = 2;
            }
            else if (chr_asci == -7) { // 닫는 괄호일 때
                if (is_open != 1) { // 여는 괄호가 전에 나오지 않았을 때
                    printf("Error: 괄호가 다 열리지/닫히지 않았습니다.");
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
                }
            }
            else if (chr_asci == -2) { // 소수점일 때
                if (isnum != 0) {
                    printf("Error: 소수점의 위치가 올바르지 않습니다.");
                    free(function);
                    exit(1);
                }
                else { 
                    isnum = 4;
                }
            }
            else if (chr_asci > -9) { // 사칙연산일 때
                if (isnum != 1 && isnum != 4) {
                    isnum = 1;
                }
                else {
                    printf("Error: 연산자가 연달아 나올 수 없습니다.");
                    free(function);
                    exit(1);
                }
            }
            else { // 공백 등 asci < 40인 것 패스

            }
        }
        else {
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

    // char func[size+3]; //주석처리 부분 곱하기 생략 만들다 실패

    // int n_asci;
    // int n = 0, j = 0;
    // int next_nasci;
    // while (n < size) {
    //     n_asci = function[n+j] - '0';
    //     next_nasci = function[n+j+1] - '0';
    //     if (n_asci == -7 || next_nasci == -8) {
    //         func[n] = '*';
    //         j++;
    //     }
    //     else {
    //         func[n] = *(function + n);
    //     }
    //     n++;
    // }
    // 정상작동 파일 작성
    FILE *f = fopen("normalfile.txt","w");
    fprintf(f, "%s", function);
    free(function);
    fclose(f);
    printf("파일 작성 완료");
    return "normalfile.txt";
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
    //int count=0;
    while ((expression_char = fgetc(file)) != EOF) {
        addNext(input_head, expression_char);
        //printf("%c", expression_char);
        //++count;
    }
    //printf("%d counted.\n", count);
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
            //printf("\n( was found.\n");
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
            //printf("a number was found.");
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

    struct NODE *result;
    struct NODE *current = postfix;

    while (current != NULL) {
        if ((current->data == '.') || (current->data == '0') || (current->data == '1') || (current->data == '2') || (current->data == '3') || (current->data == '4') || (current->data == '5') || (current->data == '6') || (current->data == '7') || (current->data == '8') || (current->data == '9')) {
            if (signal == 1) {
                printf("signal 1 entered.\n");
                while (true) {
                    addNext(temp1_head, removeNext(current));
                    if ((current->data == ' ')){
                        break;
                    }
                }
                signal = 2;
            } else if (signal == 2) {
                printf("signal 2 entered.\n");
                while (true) {
                    addNext(temp2_head, removeNext(current));
                    if ((current->data == ' ')){
                        break;
                    }
                }
                signal = 1;
            }
        } else if (current->data == '+') {
            removeNext(current);
            printf("addition ready.\n");
            struct NODE *addition_result = Addition(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(addition_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
        } else if (current->data == '-') {
            removeNext(current);
            printf("subtraction ready.\n");
            struct NODE *subtraction_result = Subtraction(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(subtraction_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
        } else if (current->data == '*') {
            removeNext(current);
            printf("multiplication ready.\n");
            struct NODE *multiplication_result = Multiplication(temp1_head, temp2_head);
            freeLinkedList(&temp1_head);
            freeLinkedList(&temp2_head);

            temp1_head = reverseDataOrder(multiplication_result);
            temp2_head = malloc(sizeof(struct NODE));
            temp2_head->next = NULL;
            temp2_head->data = ' ';
            signal = 2;
        } else {
            removeNext(current);
        }
        result = temp1_head;
    }
    freeLinkedList(&current);
    freeLinkedList(&postfix);
    result=reverseDataOrder(result); //result는 마지막 temp1_head인데, temp_head는 뒤집힌 상태임. 원래 상태로 만들어준 것임.
    return result;
}

struct NODE *Addition(struct NODE *NUM1, struct NODE *NUM2) {
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어와서 괜찮음 이대로 해도 됨.
    int over_ten_num=0;
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    result_head->data = ' ';

    int signal=0;
    while(true){
        if (signal==0){
            if ((NUM1 ->data != ' ') && (NUM2 ->data != ' ')){
                signal=1;
            }else{
                if (NUM1 ->data == ' '){
                    removeNext(NUM1);
                }
                if (NUM2 ->data == ' '){
                    removeNext(NUM2);
                }
            }
            printf("addition entered.\n");
        }
        else if (signal == 1) {
            int num1 = 0, num2 = 0;
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
    struct NODE *search1_node = NUM1;
    int num1_cnt=0; //연결리스트의 노드 개수를 세는 것임(수의 길이와 조금 차이가 있는 값임.)
    while(search1_node!=NULL){
        ++num1_cnt;
        search1_node=search1_node->next;
    }
    freeLinkedList(&search1_node);

    struct NODE *search2_node = NUM1;
    int num2_cnt=0; //연결리스트의 노드 개수를 세는 것임(수의 길이와 조금 차이가 있는 값임.)
    while(search2_node!=NULL){
        ++num2_cnt;
        search2_node=search2_node->next;
    }
    freeLinkedList(&search2_node);
    
    if (num1_cnt>=num2_cnt){
        int signal=0;
        int subtract_next = 0;
        struct NODE *result_head = malloc(sizeof(struct NODE));
        result_head->next=NULL;
        result_head->data = ' ';
        while(true){
            if (signal==0){
                if ((NUM1 ->data != ' ') && (NUM2 ->data != ' ')){
                    signal=1;
                }else{
                    if (NUM1 ->data == ' '){
                        removeNext(NUM1);
                    }
                    if (NUM2 ->data == ' '){
                        removeNext(NUM2);
                    }
                }
                printf("subtraction entered.\n");
            }
            else if (signal == 1) {
                int num1 = 0, num2 = 0;
                if ((NUM1->data == ' ') && (NUM2->data == ' ')) {
                    return result_head;
                } else if ((NUM1->data != ' ') && (NUM2->data != ' ')) {
                    num1 = NUM1->data - '0';
                    num2 = NUM2->data - '0';
                } else if ((NUM1->data != ' ') && (NUM2->data == ' ')) { //무조건 NUM1 이 NUM2보다 커서 addition에 있던 조건 하나를 삭제함
                    num1 = NUM1->data - '0';
                }

                int temp_result= num1 - num2 + subtract_next; //subtract_next는 음수 또는 0이어서 더한 것임.
                if(temp_result<0){
                    subtract_next=-1;
                    temp_result=10+temp_result; //예를 들자면 윗자리에서 10 빌려와서 -3을 7로 바꾸는 것임.
                }else{
                    subtract_next=0;
                }
                removeNext(NUM1);
                removeNext(NUM2);
                char result_char = temp_result+ '0';
                addNext(result_head, result_char);
            }
        }
    }

    else if (num1_cnt<num2_cnt){
        int signal=0;
        int subtract_next = 0;
        struct NODE *result_head = malloc(sizeof(struct NODE));
        result_head->next=NULL;
        result_head->data = ' ';
        while(true){
            if (signal==0){
                if ((NUM2 ->data != ' ') && (NUM1 ->data != ' ')){
                    signal=1;
                }else{
                    if (NUM2 ->data == ' '){
                        removeNext(NUM2);
                    }
                    if (NUM1 ->data == ' '){
                        removeNext(NUM1);
                    }
                }
                printf("subtraction entered.\n");
            }
            else if (signal == 1) {
                int num2 = 0, num1 = 0;
                if ((NUM2->data == ' ') && (NUM1->data == ' ')) {
                    addNext(result_head, '-'); //위치를 바꿔 계산한 결과에 '-'를 붙임.
                    return result_head;
                } else if ((NUM2->data != ' ') && (NUM1->data != ' ')) {
                    num2 = NUM2->data - '0';
                    num1 = NUM1->data - '0';
                } else if ((NUM2->data != ' ') && (NUM1->data == ' ')) { //무조건 NUM2 이 NUM1보다 커서 addition에 있던 조건 하나를 삭제함
                    num2 = NUM2->data - '0';
                }

                int temp_result= num2 - num1 + subtract_next; //subtract_next는 음수 또는 0이어서 더한 것임.
                if(temp_result<0){
                    subtract_next=-1;
                    temp_result=10+temp_result; //예를 들자면 윗자리에서 10 빌려와서 -3을 7로 바꾸는 것임.
                }else{
                    subtract_next=0;
                }
                removeNext(NUM2);
                removeNext(NUM1);
                char result_char = temp_result+ '0';
                addNext(result_head, result_char);
            }
        }
    }
}

struct NODE *Multiplication(struct NODE *NUM1, struct NODE *NUM2){
    //들어올 때부터 NUM1,NUM2이 뒤집혀서 들어와서 괜찮음 이대로 해도 됨.
    //소수 간의 계산 처리해야 함.
    int over_ten_num=0;
    struct NODE *result_head = malloc(sizeof(struct NODE));
    result_head->next=NULL;
    result_head->data = '0'; //여기서는 특이하게 ' '이 아닌 '0'으로 적음. Addition을 부를 때 잘 작동하게 하기 위해서임.

    int signal=0;
    while(true){
        if (signal == 0){
            if ((NUM1 ->data != ' ') && (NUM2 ->data != ' ')){
                signal=1;
            }else{
                if (NUM1 ->data == ' '){
                    removeNext(NUM1);
                }
                if (NUM2 ->data == ' '){
                    removeNext(NUM2);
                }
            }
            printf("multiplication entered.\n");
        }
        else if (signal == 1) {
            struct NODE *num2_search = NUM2;
            struct NODE *temp_result = malloc(sizeof(struct NODE));
            int over_ten_num=0;
            int multiply1_num = (NUM1->data) - '0'; //num2에 곱할 num1의 가장 작은 자리수
            while(num2_search!=NULL){
                int multiply2_num = (num2_search->data) - '0';
                int temp_frac_result = multiply1_num * multiply2_num + over_ten_num;
                int frac_result= temp_frac_result%10;
                over_ten_num = temp_frac_result/10;
                
                addNext(temp_result, frac_result+'0');
                removeNext(num2_search);
            }
            freeLinkedList(&num2_search);
            if (over_ten_num!=0){
                addNext(temp_result, over_ten_num+'0');
            }
            
            result_head=Addition(reverseDataOrder(result_head), reverseDataOrder(temp_result)); //Addition에 들어가는 애들은 뒤집힌 상태여야 함.

            removeNext(NUM1); //가장 작은 자리수 제거
            if(NUM1==NULL){
                return result_head;
            }
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
