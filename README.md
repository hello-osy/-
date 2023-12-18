# 시스템 프로그래밍 기초 팀 프로젝트

## 프로젝트 주제
- 무한소수 계산기

## 개발 기간
- 2023.10.13 ~ 2023.12.21

## 팀원
- 팀장 오상영
- 팀원 백혜경
- 팀원 안예영

### 바뀐 코드에 대한 설명
- 12/02(calculator.c 변경사항) : removeNext 함수 수정. addition.c 파일의 Add함수를 사용할 수 있도록 수정함. Add에 2개의 연결리스트를 인자로 줬음.
- 12/02(calculator.c 변경사항) : addition.c 코드가 잘 이해되지 않아서 일단 같은 역할을 하는 코드를 calculator.c에 추가함.
- 12/08(calculator.c 변경사항) : 모든 코드를 이 파일로 합쳐놓음. 이 파일에 있는 내용만 수정하면 됨. 처음부터 식을 연결리스트에 저장하도록 함.
- 12/09(calculator_test.c 변경사항) : 함수들을 약간 수정함. LINK, NODE가 둘 다 있었는데 NODE로 통일함.
- 12/10(calculator_test.c 변경사항) : 진짜 아주 조금만 수정하면 덧셈은 성공할 수도?
- 12/10(calculator_test.c 변경사항) : 덧셈 완성!(물론 식이 달라지면 조금 오류가 생길 수 있지만 그건 내가 고칠 수 있음.)
- 12/16(calculator_test.c 변경사항) : 예외처리(연산자로 끝나는 식, 연산자 2개가 붙어있는 경우, 괄호의 여닫힘이 올바르지 않은 경우) 완료. 소수점이 2개 이상인 경우, 곱셈 생략은 추가로 해야 함. 메모리 할당 후 free() 완료.
- 12/16(calculator_test.c 변경사항) : 덧셈, 곱셈 다 오류 없이 구현함. 뺄셈은 결과가 음수일 때 오류 발생.
- 12/17(calculator_test.c 변경사항) : 덧셈, 곱셈, 뺄셈 완성(내가 모르는 오류가 있을 수도 있음.)
- 12/17(calculator_test.c 변경사항) : 소수 간의 덧셈 완성.
- 12/18(calculator_test.c 변경사항) : 소수 간의 뺄셈 완성. 소수 간의 덧셈 연달아 할 수 있음. 정수와 소수 간의 계산 오류 해결
### 앞으로 해결해야 할 문제
- 12/02 : addition.c파일에서 덧셈을 처리해야 함. calculator.c파일에 아직 해결해야할 문제들이 있음.(코드 내 주석 참고)
- 12/08 : 일단 덧셈까지 완벽하게 처리해야 함. 사실 덧셈이 되면 그냥 뺄셈, 곱셈은 아주 쉽게 구현할 수 있음.
- 12/09 : 메모리 관련 문제를 해결해야 함. 일단 프로그램 실행 과정을 추적하면서 뭐가 문제인지 찾는 중임.
- 12/10 : 메모리 반납까지는 잘 된 것 같은데, 연산이 제대로 되고 있는건지 확인해봐야 함. 일단 정상적으로 프로그램 종료는 되는 듯?
- 12/10 : 뺄셈, 곱셈 만들기.
- 12/16 : 소수 계산 추가. 식 예외처리. (A)(B) 를 (A*B)로 바꿔주기.
- 12/17 : 소수 간의 뺄셈, 곱셈 구현.
- 12/18 : 소수 간의 곱셈 구현. 소수간의 뺄셈, 곱셈을 연달아 할 수 있어야 함.
