#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_EMP 150
#define MAX_LOG 600

struct Employee {
    char empId[20];
    char name[50];
    char position[15];
    double baseSalary;
    int workDays;
};

struct TimeSheet {
    char logId[20];
    char empId[20];
    char date[20];
    char status[10];
};

struct Employee employees[MAX_EMP];
int empCount = 0;

struct TimeSheet timesheets[MAX_LOG];
int logCount = 0;

void initSampleData();
void removeNewline(char* str);
int findEmpIndex(char id[]);
int isCheckedIn(char empId[], char date[]);
int containsLetter(char *str);
int isNumberString(char *str);
int isIntegerString(char *str);
int isValidDateFormat(char *date);

void printfMenu();
void addEmployee();
void updateEmployee();
void deleteEmployee();
void listEmployee();
void searchEmployeeByName();
void sortByBaseSalary();
void checkIn();
void viewTimeSheet();

int main() {
    initSampleData();
    int choice;
    char buffer[10];
    
    do {
        printfMenu();
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0) {
            printf(">> Loi: Khong duoc de trong! Vui long nhap lua chon (1-9).\n");
            choice = -1;
        } else if (!isIntegerString(buffer)) {
            printf(">> Loi: Vui long nhap so nguyen!\n");
            choice = -1;
        } else {
            choice = atoi(buffer);
        }

        switch (choice) {
            case 1: 
            {
                addEmployee(); 
                break;
            }
            case 2: 
            {
                updateEmployee(); 
                break;
            }
            case 3: 
            {
                deleteEmployee(); 
                break;
            }
            case 4: 
            {
                listEmployee(); 
                break;
            }
            case 5: 
            {
                searchEmployeeByName(); 
                break;
            }
            case 6: 
            {
                sortByBaseSalary(); 
                break;
            }
            case 7: 
            {
                checkIn(); 
                break;
            }
            case 8: 
            {
                viewTimeSheet(); 
                break;
            }
            case 9: 
            {
                printf("Ket thuc chuong trinh!\n"); 
                break;
            }
            case -1: 
            {
                break;
            }
            default: 
            {
                printf(">> Lua chon khong hop le! .\n");
            }
        }
    } while (choice != 9);

    return 0;
}

void initSampleData() {
    empCount = 5;
    strcpy(employees[0].empId, "NV001"); strcpy(employees[0].name, "Thanh Tu");       strcpy(employees[0].position, "Nhan vien");    employees[0].baseSalary = 5000000;  employees[0].workDays = 22;
    strcpy(employees[1].empId, "NV002"); strcpy(employees[1].name, "Nguyen Van A");   strcpy(employees[1].position, "Truong phong"); employees[1].baseSalary = 12000000; employees[1].workDays = 25;
    strcpy(employees[2].empId, "NV003"); strcpy(employees[2].name, "Le Thi A");       strcpy(employees[2].position, "Pho phong");    employees[2].baseSalary = 9000000;  employees[2].workDays = 20;
    strcpy(employees[3].empId, "NV004"); strcpy(employees[3].name, "Tran Van C");     strcpy(employees[3].position, "Nhan vien");    employees[3].baseSalary = 6000000;  employees[3].workDays = 18;
    strcpy(employees[4].empId, "NV005"); strcpy(employees[4].name, "Nguyen Binh");    strcpy(employees[4].position, "Nhan vien");    employees[4].baseSalary = 7000000;  employees[4].workDays = 24;
}

void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int findEmpIndex(char id[]) {
    for (int i = 0; i < empCount; i++) {
        if (strcmp(employees[i].empId, id) == 0) {
            return i;
        }
    }
    return -1;
}

int isCheckedIn(char empId[], char date[]) {
    for (int i = 0; i < logCount; i++) {
        if (strcmp(timesheets[i].empId, empId) == 0 && strcmp(timesheets[i].date, date) == 0) {
            return 1; 
        }
    }
    return 0; 
}

int containsLetter(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (isalpha(str[i])) {
            return 1;
        }
    }
    return 0; 
}

int isNumberString(char *str) {
    if (strlen(str) == 0) {
        return 0;
    }
    int dotCount = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            if (str[i] == '.' && dotCount == 0) {
                dotCount++;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

int isIntegerString(char *str) {
    if (strlen(str) == 0) {
        return 0;
    }
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0; 
        }
    }
    return 1; 
}

int isValidDateFormat(char *date) {
    // do dai phai danug 10 ky tu: dd/mm/yyyy
    if (strlen(date) != 10) return 0;

    // Vi tri 2 và 5 phai là '/'
    if (date[2] != '/' || date[5] != '/') return 0;

    // Các vi trí khác phai là so
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5){ // bo qua dau '/'
        	continue;
		}		
        if (!isdigit((unsigned char)date[i])){
        	return 0;
		} 
    }

    // Tach ngay, thang, nam
    int day = (date[0]-'0')*10 + (date[1]-'0');
    int month = (date[3]-'0')*10 + (date[4]-'0');
    int year = atoi(date+6);

    // Kiem tra thang hop le
    if (month < 1 || month > 12){
	    return 0;	
	}
    // So ngày toi da theo thang
    int maxDay;
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDay = 31; break;
        case 4: case 6: case 9: case 11:
            maxDay = 30; break;
        case 2:
            // Nam nhuan: chia het cho 400 hoac (chia het cho 4 nhung khong chia het cho 100)
            if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)){
            	maxDay = 29;
			}
            else{
            	maxDay = 28;
			}
            break;
    }
    if (day < 1 || day > maxDay){
    	return 0;
	} 

    return 1; // hop le
}

void printfMenu() {
    printf("\n+------------------------------------------+\n");
    printf("|   HE THONG QUAN LY NHAN VIEN & CHAM CONG |\n");
    printf("+------------------------------------------+\n");
    printf("| %-40s |\n", "1. Them nhan vien moi.");
    printf("| %-40s |\n", "2. Cap nhat ho so nhan vien.");
    printf("| %-40s |\n", "3. Quan li nhan su (Sa thai).");
    printf("| %-40s |\n", "4. Hien thi danh sach.");
    printf("| %-40s |\n", "5. Tra cuu (Tim kiem).");
    printf("| %-40s |\n", "6. Sap xep danh sach.");
    printf("| %-40s |\n", "7. Cham cong.");
    printf("| %-40s |\n", "8. Xem bang cong.");
    printf("| %-40s |\n", "9. Thoat.");
    printf("+------------------------------------------+\n");
    printf(">> Lua chon cua ban: ");
}

void addEmployee() {
    char continueInput;
    char buffer[100];
    do {
        if (empCount >= MAX_EMP) {
            printf("Danh sach da day!\n");
            break;
        }
        
        struct Employee emp;
        int valid = 0;
        
        printf("\n----- THEM NHAN VIEN MOI -----\n");
        
        do {
            printf("Nhap Ma Nhan Vien (ID): ");
            fgets(emp.empId, sizeof(emp.empId), stdin);
            removeNewline(emp.empId);
            
            if (strlen(emp.empId) == 0) {
                printf("Loi: Khong duoc de trong! Vui long nhap lai.\n");
                valid = 0;
            } else if (findEmpIndex(emp.empId) != -1) {
                printf("Loi: Ma ID da ton tai! Vui long nhap lai.\n");
                valid = 0;
            } else {
                valid = 1;
            }
        } while (!valid);
        
        do {
            printf("Nhap Ho Ten: ");
            fgets(emp.name, sizeof(emp.name), stdin);
            removeNewline(emp.name);
            
            if (strlen(emp.name) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            } else if (containsLetter(emp.name) == 0) { 
                 printf("Loi: Ten phai chua chu cai!\n"); 
                 valid = 0; 
            } else {
                int hasDigit = 0;
                for (int i = 0; i < strlen(emp.name); i++) {
                    if (isdigit(emp.name[i])) {
                        hasDigit = 1;
                    }
                }
                
                if (hasDigit) { 
                    printf("Loi: Ten khong duoc chua so!\n"); 
                    valid = 0; 
                } else {
                    valid = 1;
                }
            }
        } while (!valid);
        
        do {
            printf("Nhap Chuc Vu: ");
            fgets(emp.position, sizeof(emp.position), stdin);
            removeNewline(emp.position);
            
            if (strlen(emp.position) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            } else {
                 int hasDigit = 0;
                 for (int i = 0; i < strlen(emp.position); i++) {
                     if (isdigit(emp.position[i])) {
                         hasDigit = 1;
                     }
                 }
                 
                 if (hasDigit) { 
                     printf("Loi: Chuc vu khong duoc chua so!\n"); 
                     valid = 0; 
                 } else {
                     valid = 1;
                 }
            }
        } while (!valid);
        
        do {
            printf("Nhap Luong co ban: ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);
            
            if (strlen(buffer) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            } else if (!isNumberString(buffer)) {
                printf("Loi: Vui long nhap so!\n");
                valid = 0;
            } else {
                emp.baseSalary = atof(buffer);
                if (emp.baseSalary <= 0) {
                    printf("Loi: Luong phai > 0!\n");
                    valid = 0;
                } else {
                    valid = 1;
                }
            }
        } while (!valid);
        
        emp.workDays = 0;       
        employees[empCount] = emp;
        empCount++;
        
        printf("Them Thanh Cong !\n");
        
        do {
            printf("-Tiep tuc them ? (y/n): ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);
            
            if (strlen(buffer) == 0) {
                printf("Loi: Khong Duoc De Trong!\n");
                valid = 0;
            } else {
                continueInput = buffer[0];
                if (tolower(continueInput) == 'y' || tolower(continueInput) == 'n') {
                    valid = 1;
                } else {
                    printf("Loi: Chi Nhap y hoac n!\n");
                    valid = 0;
                }
            }
        } while (!valid);
    
    } while (tolower(continueInput) == 'y');
}

void updateEmployee() {
    char targetId[20];
    int valid = 0;
    printf("\n--------- CAP NHAT HO SO NHAN VIEN ---------\n");
    
    do {
        printf("Nhap Ma Nhan Vien can sua (ID): ");
        fgets(targetId, sizeof(targetId), stdin);
        removeNewline(targetId);
        if (strlen(targetId) == 0) {
            printf("Loi: Khong duoc de trong!\n");
            valid = 0;
        } else {
            valid = 1;
        }
    } while (!valid);
    
    int index = findEmpIndex(targetId);
    
    if (index == -1) {
        printf("Khong Tim Thay Ho So Nhan Vien!\n");
    } else {
        char line[] = "+------------+----------------------+-----------------+-----------------+\n";
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15.0f |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary);
        printf("%s", line);
        
        char buffer[10];
        char confirm;
        
        do {
            printf("\nBan Co Muon Thay Doi Chuc Vu ? (y/n): ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);
            
            if (strlen(buffer) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }
            confirm = buffer[0];
            
            if (tolower(confirm) == 'y') {
                do {
                    printf("Nhap Chuc Vu Moi: ");
                    fgets(employees[index].position, sizeof(employees[index].position), stdin);
                    removeNewline(employees[index].position);
                    
                    if (strlen(employees[index].position) == 0) {
                        printf("Loi: Khong Duoc De Trong!\n");
                        valid = 0;
                    } else {
                        int hasDigit = 0;
                        for (int i = 0; i < strlen(employees[index].position); i++) {
                            if (isdigit(employees[index].position[i])) {
                                hasDigit = 1;
                            }
                        }
                        
                        if (hasDigit) { 
                            printf("Loi: Khong duoc chua so!\n"); 
                            valid = 0; 
                        } else {
                            valid = 1;
                        }
                    }
                } while (!valid);
                printf("Cap Nhat Thanh Cong!\n");
                break;
            } else if (tolower(confirm) == 'n') {
                break;
            } else {
                printf("Loi: Chi Nhap y hoac n!\n");
            }
        } while (1);
        
        do {
            printf("Ban Co Muon Thay Doi Luong CB ? (y/n): ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);
            
            if (strlen(buffer) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }
            confirm = buffer[0];
            
            if (tolower(confirm) == 'y') {
                char salBuffer[50];
                double newSalary;
                do {
                    printf("Nhap Luong CB Moi: ");
                    fgets(salBuffer, sizeof(salBuffer), stdin);
                    removeNewline(salBuffer);
                    
                    if (strlen(salBuffer) == 0) {
                        printf("Loi: Khong Duoc De Trong!\n");
                        valid = 0;
                    } else if (!isNumberString(salBuffer)) {
                        printf("Loi: Luong CB Phai La So!\n");
                        valid = 0;
                    } else {
                        newSalary = atof(salBuffer);
                        if (newSalary <= 0) {
                            printf("Loi: Luong phai > 0!\n");
                            valid = 0;
                        } else {
                            employees[index].baseSalary = newSalary;
                            valid = 1;
                        }                        
                    }                    
                } while (!valid);
                printf("Cap Nhat Thanh Cong!\n");
                break;
            } else if (tolower(confirm) == 'n') {
                break;
            } else {
                printf("Loi: Chi Nhap y hoac n!\n");
            }
        } while (1);
        
        printf("\n--- THONG TIN SAU CAP NHAT ---\n");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15.0f |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary);
        printf("%s", line);
    }
}

void deleteEmployee() {
    char targetId[20];
    int valid = 0;

    printf("\n-------- XOA HO SO NHAN VIEN --------\n");

    do {
        printf("Nhap Ma Nhan Vien can xoa (ID): ");
        fgets(targetId, sizeof(targetId), stdin);
        removeNewline(targetId);

        if (strlen(targetId) == 0) {
            printf("Loi: Khong duoc de trong!\n");
        } else {
            valid = 1;
        }
    } while (!valid);

    int index = findEmpIndex(targetId);

    if (index == -1) {
        printf("Khong tim thay nhan vien co ID \"%s\"!\n", targetId);
        return;
    }

    char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
    printf("%s", line);
    printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
    printf("%s", line);
    printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary, employees[index].workDays);
    printf("%s", line);

    char buffer[10];
    char confirm;

    do {
        printf("Xac Nhan Xoa Nhan Vien (y/n): ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);
        if (strlen(buffer) == 0) {
            printf("Loi: Khong duoc de trong!\n");
            continue;
        }
        confirm = buffer[0];

        if (tolower(confirm) == 'y'){
            for (int i = index; i < empCount - 1; i++) {
                employees[i] = employees[i + 1];
            }
            empCount--;
            printf("Da xoa nhan vien thanh cong!\n");
            break;
        } else if (tolower(confirm) == 'n') {
            printf("Da huy thao tac xoa.\n");
            break;
        } else {
            printf("Loi: Chi duoc nhap y hoac n!\n");
        }
    } while (1);
}

void listEmployee() {
    int perPage = 3; 
    int totalPage;
    int page;
    char buffer[10];
    
    if (empCount == 0) {
        printf("\n--- DANH SACH NHAN VIEN ---\n");
        printf("Danh sach trong!\n");
        return;
    }
    
    totalPage = (empCount + perPage - 1) / perPage;

    do {
        printf("\nTong so trang: %d\n", totalPage);
        printf("Nhap So Trang Muon Xem (1-%d, 0 De Thoat): ", totalPage);
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);
        
        if (strlen(buffer) == 0) {
            printf("Loi: Khong duoc de trong!\n");
            continue;
        }

        if (!isIntegerString(buffer)) {
            printf("Loi: So trang phai la so nguyen!\n");
            continue;
        }

        page = atoi(buffer);
        
        if (page == 0) {
            printf("Thoat Hien Thi Danh Sach!\n");
            break;
        }
        if (page < 1 || page > totalPage) {
            printf("Loi: Trang Khong Hop Le (1-%d)!\n", totalPage);
            continue;
        }
        
        int start = (page - 1) * perPage;
        int end = start + perPage;
        if (end > empCount) {
            end = empCount;
        }
        
        printf("TRANG %d/%d\n", page, totalPage);
        printf("---------- DANH SACH NHAN VIEN ----------\n");
        char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
        printf("%s", line);
    
        for (int i = start; i < end; i++) {
            printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n", employees[i].empId, employees[i].name, employees[i].position, employees[i].baseSalary, employees[i].workDays);
            printf("%s", line);
        }
    } while (1);
}

void searchEmployeeByName() {
    if (empCount == 0) {
        printf("Danh sach nhan vien hien dang trong.\n");
        return;
    }

    char buffer[50];
    char again[5];

    do {
        do {
            printf("\nNhap ten nhan vien can tim: ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);

            if (strlen(buffer) == 0) {
                printf("Loi: Khong duoc de trong! Vui long nhap lai.\n");
                continue;
            }
            
            int validChar = 1;
            for (int i = 0; i < strlen(buffer); i++) {
                if (isdigit(buffer[i])) {
                    validChar = 0;
                }
            }
            
            if (!validChar) {
                 printf("Loi: Ten khong duoc chua so!\n");
                 buffer[0] = '\0'; 
            }
            

        } while (strlen(buffer) == 0);

        char keyword[50];
        strcpy(keyword, buffer);
        for (int i = 0; keyword[i]; i++) {
            keyword[i] = tolower(keyword[i]);
        }

        int found = 0;
        printf("\n---------- KET QUA TIM KIEM ----------\n");
        char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
        printf("%s", line);

        for (int i = 0; i < empCount; i++) {
            char temp[50];
            strcpy(temp, employees[i].name);
            for (int j = 0; temp[j]; j++) {
                temp[j] = tolower(temp[j]);
            }

            if (strstr(temp, keyword) != NULL) {
                printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n",
                       employees[i].empId,
                       employees[i].name,
                       employees[i].position,
                       employees[i].baseSalary,
                       employees[i].workDays);
                printf("%s", line);
                found = 1;
            }
        }

        if (!found) {
            printf("| %-80s |\n", "Khong tim thay ket qua nao!");
            printf("%s", line);
        }

        do {
            printf("\nBan co muon tim kiem tiep khong? (y/n): ");
            fgets(again, sizeof(again), stdin);
            removeNewline(again);

            if (strlen(again) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }

            if (!(tolower(again[0]) == 'y' || tolower(again[0]) == 'n')) {
                printf("Loi: Chi duoc nhap y hoac n!\n");
                again[0] = '\0';
            }

        } while (strlen(again) == 0);

    } while (tolower(again[0]) == 'y');
}

void sortByBaseSalary() {
    if (empCount == 0) {
        printf("Danh sach nhan vien hien dang trong.\n");
        return;
    }

    char buffer[20];
    char again[5];
    int sortOrder;

    do {
        sortOrder = -1;

        do {
            printf("\nChon thu tu sap xep theo luong:\n");
            printf("1. Tang dan\n");
            printf("2. Giam dan\n");
            printf("Nhap lua chon (1 hoac 2): ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);

            if (strlen(buffer) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }

            if (!isIntegerString(buffer)) {
                printf("Loi: Lua chon phai la so nguyen!\n");
                continue;
            }

            sortOrder = atoi(buffer);

            if (sortOrder != 1 && sortOrder != 2) {
                printf("Loi: Chi duoc chon 1 hoac 2!\n");
                sortOrder = -1;
            }

        } while (sortOrder == -1);

        for (int i = 0; i < empCount - 1; i++) {
            for (int j = 0; j < empCount - i - 1; j++) {
                int condition = 0;

                if (sortOrder == 1) {
                    if (employees[j].baseSalary > employees[j + 1].baseSalary) {
                        condition = 1;
                    }
                } else { 
                    if (employees[j].baseSalary < employees[j + 1].baseSalary) {
                        condition = 1;
                    }
                }

                if (condition) {
                    struct Employee temp = employees[j];
                    employees[j] = employees[j + 1];
                    employees[j + 1] = temp;
                }
            }
        }

        printf("Da sap xep thanh cong theo thu tu %s!\n", sortOrder == 1 ? "tang dan" : "giam dan");
        
        printf("---------- DANH SACH SAU KHI SAP XEP ----------\n");
        char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
        printf("%s", line);
    
        for (int i = 0; i < empCount; i++) {
            printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n", employees[i].empId, employees[i].name, employees[i].position, employees[i].baseSalary, employees[i].workDays);
            printf("%s", line);
        }

        do {
            printf("\nBan co muon sap xep tiep khong? (y/n): ");
            fgets(again, sizeof(again), stdin);
            removeNewline(again);

            if (strlen(again) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }

            if (!(tolower(again[0]) == 'y' || tolower(again[0]) == 'n')) {
                printf("Loi: Chi duoc nhap y hoac n!\n");
                again[0] = '\0';
            }

        } while (strlen(again) == 0);

    } while (tolower(again[0]) == 'y');
}

void checkIn() {
    char choice[5];
    do {
        char targetId[20];
        char date[20];
        char statusChoice[10];
        int valid = 0;

        printf("\n--------- CHAM CONG NGAY ---------\n");

        do {
            printf("Nhap Ma Nhan Vien (ID): ");
            fgets(targetId, sizeof(targetId), stdin);
            removeNewline(targetId);

            if (strlen(targetId) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            } else {
                valid = 1;
            }
        } while (!valid);

        int index = findEmpIndex(targetId);
        if (index == -1) {
            printf("Loi: Ma Nhan Vien Khong Ton Tai!\n");
        } else {

            do {
                printf("Nhap Ngay Cham Cong (DD/MM/YYYY): ");
                fgets(date, sizeof(date), stdin);
                removeNewline(date);

                if (strlen(date) == 0) {
                    printf("Loi: Khong duoc de trong!\n");
                    valid = 0;
                } else if (!isValidDateFormat(date)) {
                    printf("Loi: Dinh dang ngay khong hop le!\n");
                    valid = 0;
                } else {
                    valid = 1;
                }
            } while (!valid);

            if (isCheckedIn(targetId, date)) {
                printf("Loi: Nhan vien %s da co ban ghi ngay %s!\n", employees[index].name, date);
            } else {

                do {
                    printf("Nhap Trang Thai (1: Di lam, 2: Nghi lam): ");
                    fgets(statusChoice, sizeof(statusChoice), stdin);
                    removeNewline(statusChoice);
                    
                    if(strlen(statusChoice) == 0){
                    	printf("Loi: khong duoc de trong !\n");
					}
                    if (strcmp(statusChoice, "1") == 0 || strcmp(statusChoice, "2") == 0) {
                        valid = 1;
                    } else {
                        printf("Loi: Chi duoc nhap 1 hoac 2!\n");
                        valid = 0;
                    }
                } while (!valid);

                struct TimeSheet newLog;
                sprintf(newLog.logId, "L%04d", logCount + 1);
                strcpy(newLog.empId, targetId);
                strcpy(newLog.date, date);

                if (strcmp(statusChoice, "1") == 0) {
                    strcpy(newLog.status, "Di lam");
                    employees[index].workDays++;
                    printf("Cham Cong Thanh Cong! Tong ngay cong hien tai: %d\n",employees[index].workDays);
                } else {
                    strcpy(newLog.status, "Nghi lam");
                    printf("Da ghi nhan trang thai NGHI LAM cho nhan vien %s\n", employees[index].name);
                }

                timesheets[logCount] = newLog;
                logCount++;
            }
        }
        printf("\nBan co muon cham cong tiep khong? (y/n): ");
        fgets(choice, sizeof(choice), stdin);
        removeNewline(choice);

    } while (strcmp(choice, "y") == 0 || strcmp(choice, "Y") == 0);
}

void viewTimeSheet() {
    char targetId[20];
    char again[5];
    int valid = 0;

    do {
        printf("\n--------- XEM LICH SU CHAM CONG ---------\n");
        do {
            printf("Nhap Ma Nhan Vien Can Xem: ");
            fgets(targetId, sizeof(targetId), stdin);
            removeNewline(targetId);
            if (strlen(targetId) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            } else {
                valid = 1;
            }
        } while (!valid);

        if (findEmpIndex(targetId) == -1) {
            printf("Loi: Ma Nhan Vien Khong Ton Tai!\n");
        } else {
            char line[] = "+------------+----------------------+-----------------+\n";
            printf("\nBANG CONG CUA NHAN VIEN: %s\n", targetId);
            printf("%s", line);
            printf("| %-10s | %-20s | %-15s |\n", "Ma Log", "Ngay", "Trang Thai");
            printf("%s", line);

            int found = 0;
            for (int i = 0; i < logCount; i++) {
                if (strcmp(timesheets[i].empId, targetId) == 0) {
                    printf("| %-10s | %-20s | %-15s |\n", timesheets[i].logId, timesheets[i].date, timesheets[i].status);
                    printf("%s", line);
                    found = 1;
                }
            }
            if (!found) {
                printf("| %-51s |\n", "Chua co du lieu cham cong!");
                printf("%s", line);
            }
        }

        do {
            printf("\nXem tiep nhan vien khac? (y/n): ");
            fgets(again, sizeof(again), stdin);
            removeNewline(again);
            if (strlen(again) == 0) {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }
            if (tolower(again[0]) == 'y' || tolower(again[0]) == 'n') {
                break;
            }
            printf("Loi: Chi nhap y hoac n!\n");
        } while (1);

    } while (tolower(again[0]) == 'y');
}
