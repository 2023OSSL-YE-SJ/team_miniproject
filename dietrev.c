#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MENU_SIZE 50
#define ORDER_SIZE 10
#define BUFFER_SIZE 50
#define CATEGORY_SIZE 5

typedef struct
{
    char name[BUFFER_SIZE]; // 이름
    int type;               // 종류 (1.샐러드 2.라이스 3.면 4.빵 5.음료)
    int price;              // 가격
    double calorie;         // 칼로리
} Product;

int adminMenu();  //
int userMenu();   //
int selectMode(); // 관리자 모드인지 사용자 모드인지 입력받을 함수
int selectCategory();
int getCalorie();
Product * selectProduct(Product *p[], int productMenu, int calorie);
int createProduct(Product *p);              // 제품을 추가하는 함수 => 메뉴 선택
void readProduct(Product *p);               // 하나의 제품 출력 함수 => 구매 목록 보기
int updateProduct(Product *p);              // 제품을 수정하는 함수 => 구매 목록 수정
int deleteProduct(Product *p[], int count); // 제품을 삭제하는 함수 => 구매 취소 [환불]
void saveProduct(Product *p[], int count);  //데이터를 파일에 저장 하는 함수
int loadProduct(Product **p);               // 저장된 데이터를 불러오는 함수
int selectDataNo(Product *p[], int count);  // 수정, 삭제 할 번호 선택하는 함수
void listProduct(Product *p[], int count);  // 제품 리스트 출력

char categoryNames[CATEGORY_SIZE][100] = {
    "Salad",
    "Rice",
    "Noodle",
    "Bread",
    "Drink"
};

int main()
{
    Product *sp[MENU_SIZE] = {0};
    Product *orders[ORDER_SIZE] = {0};
    int orderCount = 0;

    int count = 0, menu;
    int result = 0;
    int index = 0;
    int no = 0;
    count = loadProduct(sp);
    index = count;

    if (index == 0)
        printf("=>파일 없음\n");

    int mode = selectMode();
    int done = 0;
    while (!done)
    {
        if (mode == 1)
        { // 사용자모드
            
            while (1) {
                menu = userMenu();
                if (menu == 0)
                    break;
                if (menu == 1)
                { // 메뉴 선택 + 칼로리 검색
                    // 메뉴의 종류를 보여주고 선택하게 한다
                    int category = selectCategory();

                    int input;
                    printf("1. 메뉴 선택\n");
                    printf("2. 칼로리 선택\n\n");
                    printf("원하는 메뉴 선택>> ");
                    scanf("%d", &input);

                    int calorie = 999999;
                    if (input == 2) {
                        calorie = getCalorie();
                    }
                    Product* product = selectProduct(sp, category, calorie);
                    orders[orderCount++] = product;
                }
                else if (menu == 2) // 주문완료
                {
                    //
                    printf("주문이 완료되었습니다.\n\n");
                    printf("=== 주문내역 ===\n");
                    int price = 0;
                    for (int i=0; i<orderCount; i++) {
                        readProduct(orders[i]);
                        price += orders[i]->price;
                    }
                    printf("총 상품 개수: %d\n", orderCount);
                    printf("전체 가격: %d\n", price);
                    printf("================\n\n");
                    done = 1;
                    break;
                }
            }

        }
        else if (mode == 2)
        { // 관리자모드
            menu = adminMenu();
            if (menu == 0)
            {
                break;
            }
            else if (menu == 1)
            { // 메뉴 추가
                sp[index] = (Product *)malloc(sizeof(Product));
                result = createProduct(sp[index]);
                index++;
                printf("=> 추가됨!\n");
            }
            else if (menu == 2)
            { // 메뉴 수정
                no = selectDataNo(sp, index);
                if (no != 0)
                {
                    result = updateProduct(sp[no - 1]);
                    printf("=> 수정됨!\n");
                }
                else
                    printf("수정을 취소함\n");
            }
            else if (menu == 3)
            { // 메뉴 삭제
                int n = 0;
                no = selectDataNo(sp, index);
                printf("정말로 삭제하시겠습니까? (삭제:1)");
                scanf("%d", &n);
                if (n == 1)
                {
                    result = deleteProduct(sp, no);
                }
                else
                    printf("=> 취소됨!\n");
            }
            else if (menu == 4)
            { // 파일에 저장
                saveProduct(sp, index);
            }
        }
        else
        {
            break;
        }
    }
    printf("종료됨!\n");
    return 0;
}

int getCalorie()
{
    int calorie;
    printf("칼로리를 입력해주세요: ");
    scanf("%d", &calorie);
    return calorie;
}

Product* selectProduct(Product *p[], int category, int calorie)
{
    int index = 0;
    int menu;
    int number = 1;

    while (p[index] && index < MENU_SIZE)
    {
        if (p[index]->type == category && p[index]->calorie <= calorie)
        {
            printf("%d. ", index+1);
            readProduct(p[index]);
        }
        index++;
    }

    printf("\n=> 원하는 메뉴는? ");
    scanf("%d", &menu);
    return p[menu - 1];
}

int selectCategory()
{
    int category;
    printf("\n*** 다이어트 식당 메뉴 ***\n");
    for (int i=0; i<CATEGORY_SIZE; i++) {
        printf("%d. %s\n", i+1, categoryNames[i]);
    }
    printf("\n=> 원하는 메뉴는? ");
    scanf("%d", &category);
    return category;
}

int selectMode()
{
    int mode;
    printf("\n*** 모드 선택 ***\n");
    printf("1. 사용자 모드\n");
    printf("2. 관리자 모드\n");
    printf("0. 종료\n\n");
    printf("=> 원하는 메뉴는? ");
    scanf("%d", &mode);
    return mode;
}

int userMenu()
{
    int menu;
    printf("\n*** 사용자 메뉴 ***\n");
    printf("1. 메뉴 종류 + 칼로리 검색\n");
    printf("2. 주문 완료\n");
    printf("0. 종료\n\n");
    printf("=> 원하는 메뉴는? ");
    scanf("%d", &menu);
    return menu;
}

int adminMenu()
{
    int menu;
    printf("\n*** 메뉴 관리 ***\n");
    printf("1. 추가\n");
    printf("2. 수정\n");
    printf("3. 삭제\n");
    printf("4. 저장\n");
    printf("0. 종료\n\n");
    printf("=> 원하는 메뉴는? ");
    scanf("%d", &menu);
    return menu;
}

int selectDataNo(Product *p[], int count)
{
    int no;
    listProduct(p, count);
    printf("번호는 (취소:0)? ");
    scanf("%d", &no);
    return no;
}

int createProduct(Product *p)
{
    printf("메뉴 종류는? (1.샐러드 2.라이스 3.면 4.빵 5.음료) ");
    scanf("%d", &p->type);
    getchar(); // 엔터 제거
    printf("메뉴 이름은? ");
    fgets(p->name, BUFFER_SIZE, stdin);
    p->name[strlen(p->name) - 1] = '\0'; // 엔터 제거
    printf("가격은? ");
    scanf("%d", &p->price);
    printf("칼로리는? ");
    scanf("%lf", &p->calorie);
    return 1;
}
void listProduct(Product *p[], int count)
{
    printf("********************************************\n");
    for (int i = 0; i < count; i++)
    {
        if (p[i] == NULL)
            continue;
        printf("%2d ", i + 1);
        readProduct(p[i]);
    }
    printf("\n");
}

void readProduct(Product *p)
{
    printf("%-20s", p->name);
    printf("%-10s", categoryNames[p->type - 1]);
    printf("%5d", p->price);
    printf("%10.1lf", p->calorie);
    printf("\n");
}

int updateProduct(Product *p)
{
    int result = createProduct(p);
    return 1;
}

int deleteProduct(Product *p[], int count)
{
    if (p[count - 1])
        free(p[count - 1]);
    p[count - 1] = NULL;
    printf("=> 삭제됨!\n");
    return 0;
}

void saveProduct(Product *p[], int count)
{
    FILE *fp;
    fp = fopen("menu.txt", "wt");

    for (int i = 0; i < count; i++)
    {
        if (p[i] == NULL)
            continue;
        fprintf(fp, "%s\n%d %d %.1lf\n",
                p[i]->name, p[i]->type, p[i]->price, p[i]->calorie);
    }
    fclose(fp);
    printf("=> 저장됨! ");
}

int loadProduct(Product **p)
{
    FILE *fp;
    fp = fopen("menu.txt", "rt");
    if (fp == NULL)
        return 0; // 파일이 없는 경우
    int i = 0;
    while (!feof(fp))
    {
        p[i] = (Product *)malloc(sizeof(Product)); // 메모리 할당
        fgets(p[i]->name, BUFFER_SIZE, fp);
        p[i]->name[strlen(p[i]->name) - 1] = '\0';
        fscanf(fp, "%d", &p[i]->type);
        fscanf(fp, "%d", &p[i]->price);
        fscanf(fp, "%lf\n", &p[i]->calorie);
        i++;
    }
    fclose(fp);
    printf("=> 로딩 성공!\n");
    return i;
}
