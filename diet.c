#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
char name[20]; // 이름
int type; // 종류 (1.샐러드 2.라이스 3.면,빵 4.음료)
int price; // 가격
double calorie; // 칼로리
}Product;

int selectMenu(); // 데이터를 다룰 때, 원하는 메뉴를 선택하는 함수 -> crud 등 기능 선택

int createProduct(Product *p); // 제품을 추가하는 함수 => 메뉴 선택
void readProduct(Product *p); // 하나의 제품 출력 함수 => 구매 목록 보기
int updateProduct(Product *p); // 제품을 수정하는 함수 => 구매 목록 수정
int deleteProduct(Product *p[], int count); // 제품을 삭제하는 함수 => 구매 취소 [환불]
void saveProduct(Product *p[], int count); //데이터를 파일에 저장 하는 함수
int loadProduct(Product **p); // 저장된 데이터를 불러오는 함수
int selectDataNo(Product *p[], int count); // 수정, 삭제 할 번호 선택하는 함수
void listProduct(Product *p[], int count); // 제품 리스트 출력

// void searchProductTypeAndCalories(Product *p[], int count); // 메뉴 종류, 칼로리 검색

int main(){
    Product *sp[50];
    int count = 0, menu;
    int result = 0;
    int index = 0;
    int no = 0;
    count = loadProduct(sp);
    index = count;
    if (index == 0)
        printf("=>파일 없음\n");
    while (1) {
        menu = selectMenu();
        if (menu == 0) break;
        if (menu == 1) {
            listProduct(sp, index);
        } else if (menu == 2) {
            sp[index] = (Product *)malloc(sizeof(Product));
            result = createProduct(sp[index]);
            index++;
            printf("=> 추가됨!\n");
        } else if (menu == 3) {
            no = selectDataNo(sp, index);
            if (no != 0) {
                result = updateProduct(sp[no - 1]);
                printf("=> 수정됨!\n");
            } else
                printf("수정을 취소함\n");
        } else if (menu == 4) {
            int n = 0;
            no = selectDataNo(sp, index);
            printf("정말로 삭제하시겠습니까? (삭제:1)");
            scanf("%d", &n);
            if (n == 1) {
                result = deleteProduct(sp, no);
            } else
                printf("=> 취소됨!\n");
        } else if (menu == 5) {
            saveProduct(sp, index);
        } else if (menu == 6) {
            // searchProductType(sp, index);
            // searchProductTypeAndCalories(sp, index);
        }
    }
    printf("종료됨!\n");
    return 0;
    }

int selectMenu(){
    int menu;
    printf("\n*** 제품 관리 ***\n");
    printf("1. 조회\n");
    printf("2. 추가\n");
    printf("3. 수정\n");
    printf("4. 삭제\n");
    printf("5. 저장\n");
    printf("6. 메뉴 종류, 칼로리 검색\n");
    printf("0. 종료\n\n");
    printf("=> 원하는 메뉴는? ");
    scanf("%d", &menu);
    return menu;
}
int selectDataNo(Product *p[], int count){
    int no;
    listProduct(p, count);
    printf("번호는 (취소:0)? ");
    scanf("%d", &no);
    return no;
}

int createProduct(Product *p){
    getchar(); 
    printf("메뉴 이름은? ");
    fgets(p->name, 20, stdin);
    p->name[strlen(p->name) - 1] = '\0';
    printf("메뉴 종류는? (1.샐러드 2.라이스 3.면,빵 4.음료) ");
    scanf("%d", &p->type);
    printf("가격은? ");
    scanf("%d", &p->price);
    printf("칼로리는? ");
    scanf("%lf", &p->calorie);

  return 1;
}
void listProduct(Product *p[], int count){
    printf("********************************************\n");
    for(int i=0; i<count; i++){
        if(p[i] == NULL) continue;
        printf("%2d ", i+1);
        readProduct(p[i]);
    }
    printf("\n");
}

void readProduct(Product *p){
    printf("   %s |", p->name);
    if(p->type == 1) printf(" 샐러드 |");
    else if(p->type == 2) printf(" 라이스 |");
    else if(p->type == 3) printf(" 면, 빵 |");
    else if(p->type == 4) printf(" 음료 |");
    printf(" %d | %.1lf ", p->price, p->calorie);
    printf("\n");
}

int updateProduct(Product *p){
    int result = createProduct(p);
    return 1;
}

int deleteProduct(Product *p[], int count){
    if(p[count-1]) free(p[count-1]);
    p[count-1] = NULL;
    printf("=> 삭제됨!\n");
    return 0;
}

void saveProduct(Product *p[], int count){
    FILE *fp;
    fp = fopen("menu.txt", "wt");

    for(int i=0; i<count; i++){
        if(p[i] == NULL) continue;
        fprintf(fp, "   %s |", p[i]->name);
        if(p[i]->type == 1) fprintf(fp, " 샐러드 |");
        else if(p[i]->type == 2) fprintf(fp, " 라이스 |");
        else if(p[i]->type == 3) fprintf(fp, " 면, 빵 |");
        else if(p[i]->type == 4) fprintf(fp, " 음료 |");
        fprintf(fp, " %d | %.1lf \n", p[i]->price, p[i]->calorie);
    }
    fclose(fp);
    printf("=> 저장됨! ");
}

int loadProduct(Product **p){ // 이 함수 수정 필요
    FILE* fp;
    fp = fopen("menu.txt", "rt");
    if(fp == NULL) return 0; // 파일이 없는 경우
    int i = 0;
    while(!feof(fp)) {
        p[i] = (Product *) malloc(sizeof(Product)); // 메모리 할당
        fgets(p[i]->name, 20, fp);
        fscanf(fp, "%d", &p[i]->type);
        fscanf(fp, "%d", &p[i]->price);
        fscanf(fp, "%lf\n", &p[i]->calorie);
        fgetc(fp); 
        i++;
    }
    fclose(fp);
    printf("=> 로딩 성공!\n");
    return i;
}
