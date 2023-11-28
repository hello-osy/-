#include "add.h"

addition Add(addition firstNum, addition secondNum) //각 숫자는 addition이라는 구조체로 표현됨. (숫자의 소수점 이전 부분과 이후 부분을 각각 연결 리스트로 저장함.)
{
    pNum before = NULL;
    pNum after = NULL;
    int beforeNum = 0;
    int afterNum = 0;
    addition result = initializeDigit();

    result->positive = true;

    // 첫번째 수 또는 두번째 수가 0일 경우
    // 첫번째 수가 0이면 두번째 수 반환, 두번째 수가 0면 첫번째 수 반환
    if(firstNum == 0) 
    {
        return secondNum;
    }
    if(secondNum == 0) 
    {
        return firstNum;
    }

    int getBeforeNum(addition p){
    return p->beforeNum;
    }

    int getAfterNum(addition p) {
        return p->afterNum;
    }

    int a;
    int firstBeforeNum = getBeforeNum(firstNum); // 첫번째 수의 소수점 이전 부분 저장
    int firstAfterNum = getAfterNum(firstNum); // 첫번째 수의 소수점 이후 부분 저장
    int secondAfterNum = getAfterNum(secondNum); // 두번째 수의 소수점 이후 부분 저장
    int secondBeforeNum = getBeforeNum(secondNum); // 두번째 수의 소수점 이전 부분 저장

    int AddtoBeforeNum = firstBeforeNum + secondBeforeNum;
    int AddtoAfterNum = firstAfterNum + secondAfterNum;

}
