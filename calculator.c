#include <stdbool.h>

#include "list.h"
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
            //Add(temp1_head,temp2_head);
            Addition(temp1_head, temp2_head); //이건 내가 작성한 코드
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
        if (NUM1 ->d == " "){
            removeNext(NUM1);
        }
        else if((NUM1 ->d == "0") || (NUM1 ->d == "1") || (NUM1 ->d == "2") || (NUM1 ->d == "3") || (NUM1 ->d == "4") || (NUM1 ->d == "5") || (NUM1 ->d == "6") || (NUM1 ->d == "7") || (NUM1 ->d == "8") || (NUM1 ->d == "9")){
            if (NUM2 ->d == " "){
                removeNext(NUM1);
            }
            else if((NUM2 ->d == "0") || (NUM2 ->d == "1") || (NUM2 ->d == "2") || (NUM2 ->d == "3") || (NUM2 ->d == "4") || (NUM2 ->d == "5") || (NUM2 ->d == "6") || (NUM2 ->d == "7") || (NUM2 ->d == "8") || (NUM2 ->d == "9")){
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

int main(void){
    FILE *file;
    char *buffer;
    long fileSize;
    size_t result;

    // 파일 열기
    file = fopen("math_expression.txt", "rb"); // 읽기 모드로 파일 열기

    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return 1;
    }

    // 파일 크기 계산
    fseek(file, 0, SEEK_END); // 파일 끝으로 이동
    fileSize = ftell(file); // 현재 위치(파일 끝)에서의 오프셋을 가져와서 파일 크기 계산
    rewind(file); // 파일 포인터를 다시 파일의 처음으로 이동

    // 파일 크기만큼의 메모리 할당
    buffer = (char *)malloc(fileSize * sizeof(char));

    if (buffer == NULL) {
        printf("메모리 할당에 실패했습니다.");
        return 1;
    }

    // 파일 내용을 버퍼에 복사
    result = fread(buffer, 1, fileSize, file);
    
    if (result != fileSize) {
        printf("파일 읽기 오류");
        return 1;
    }

    // 파일 닫기
    fclose(file);

    // 읽어온 문자열 출력 또는 사용
    printf("파일 내용: %s\n", buffer);

    // 메모리 해제
    free(buffer);

    return 0;
}