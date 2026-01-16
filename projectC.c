#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_EMP 150
#define MAX_LOG 600

struct Employee
{
    char empId[20];
    char name[50];
    char position[15];
    double baseSalary;
    int workDays;
};

struct TimeSheet
{
    char logId[20];
    char empId[20];
    char date[20];
    char status[10];
};

struct Employee employees[MAX_EMP];
int empCount = 0;

struct TimeSheet timesheets[MAX_LOG];
int logCount = 0;

/* Khoi tao du lieu mau cho 5 nhan vien ban dau */
void initSampleData();
/* Xoa ky tu newline o cuoi chuoi nhap */
void removeNewline(char *str);
/* Tim chi so nhan vien theo empId, tra ve -1 neu khong co */
int findEmpIndex(char id[]);
/* Kiem tra mot nhan vien da co log cham cong tai ngay cu the chua */
int isCheckedIn(char empId[], char date[]);
/* Tra ve 1 neu chuoi co it nhat 1 chu cai */
int containsLetter(char *str);
/* Kiem tra chuoi so thuc hop le (chi 1 dau '.', khong o dau/cuoi) */
int isNumberString(char *str);
/* Kiem tra chuoi so nguyen hop le */
int isIntegerString(char *str);
/* Kiem tra dinh dang ngay dd/mm/yyyy hop le va so ngay trong thang */
int isValidDateFormat(char *date);

/* Helper functions - Giam code trung lap */
/* Kiem tra chuoi co chua chu so khong */
int hasDigit(char *str);
/* Nhap va validate input y/n, tra ve 1 neu 'y', 0 neu 'n', -1 neu loi */
int getYesNoInput(const char *prompt);
/* Nhap chuoi khong duoc de trong, tra ve 1 neu thanh cong */
int getNonEmptyString(char *buffer, size_t size, const char *prompt);
/* In bang nhan vien (header + data) */
void printEmployeeTable(int startIdx, int endIdx);
/* In header cua bang nhan vien */
void printEmployeeHeader();

/* In menu chinh */
void printfMenu();
/* Them nhan vien moi, validate tung truong */
void addEmployee();
/* Cap nhat chuc vu, luong co ban cua nhan vien theo ID */
void updateEmployee();
/* Xoa nhan vien theo ID sau khi xac nhan */
void deleteEmployee();
/* Liet ke nhan vien phan trang */
void listEmployee();
/* Tim kiem theo ten (so khop mot phan, khong phan biet hoa thuong) */
void searchEmployeeByName();
/* Sap xep danh sach theo luong tang/giam dan */
void sortByBaseSalary();
/* Cham cong: kiem tra ID, dinh dang ngay, trang thai, chong trung log */
void checkIn();
/* Xem bang cham cong cua mot nhan vien */
void viewTimeSheet();

int main()
{
    initSampleData();
    int choice;
    char buffer[20]; /* Tang buffer size de tranh overflow */

    do
    {
        printfMenu();
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0)
        {
            printf(">> Loi: Khong duoc de trong! Vui long nhap lua chon (1-9).\n");
            choice = -1;
        }
        else if (!isIntegerString(buffer))
        {
            printf(">> Loi: Vui long nhap so nguyen!\n");
            choice = -1;
        }
        else
        {
            choice = atoi(buffer);
        }

        switch (choice)
        {
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

void initSampleData()
{
    empCount = 5;
    strcpy(employees[0].empId, "NV001");
    strcpy(employees[0].name, "Thanh Tu");
    strcpy(employees[0].position, "Nhan vien");
    employees[0].baseSalary = 5000000;
    employees[0].workDays = 22;

    strcpy(employees[1].empId, "NV002");
    strcpy(employees[1].name, "Nguyen Van A");
    strcpy(employees[1].position, "Truong phong");
    employees[1].baseSalary = 12000000;
    employees[1].workDays = 25;

    strcpy(employees[2].empId, "NV003");
    strcpy(employees[2].name, "Le Thi A");
    strcpy(employees[2].position, "Pho phong");
    employees[2].baseSalary = 9000000;
    employees[2].workDays = 20;

    strcpy(employees[3].empId, "NV004");
    strcpy(employees[3].name, "Tran Van C");
    strcpy(employees[3].position, "Nhan vien");
    employees[3].baseSalary = 6000000;
    employees[3].workDays = 18;
    
    strcpy(employees[4].empId, "NV005");
    strcpy(employees[4].name, "Nguyen Binh");
    strcpy(employees[4].position, "Nhan vien");
    employees[4].baseSalary = 7000000;
    employees[4].workDays = 24;
}

void removeNewline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

int findEmpIndex(char id[])
{
    /* Duyet tu dau danh sach den khi tim duoc empId trung khop */
    for (int i = 0; i < empCount; i++)
    {
        if (strcmp(employees[i].empId, id) == 0)
        {
            return i;
        }
    }
    return -1;
}

int isCheckedIn(char empId[], char date[])
{
    /* Moi timesheets[i] la mot log da ton tai; so sanh ca empId va ngay */
    for (int i = 0; i < logCount; i++)
    {
        if (strcmp(timesheets[i].empId, empId) == 0 && strcmp(timesheets[i].date, date) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int containsLetter(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if (isalpha((unsigned char)str[i]))
        {
            return 1;
        }
    }
    return 0;
}

int isNumberString(char *str)
{
    size_t len = strlen(str);
    if (len == 0)
    {
        return 0;
    }
    int dotCount = 0;
    int digitCount = 0;

    for (size_t i = 0; i < len; i++)
    {
        /* Neu la chu so thi dem va tiep tuc */
        if (isdigit((unsigned char)str[i]))
        {
            digitCount++;
            continue;
        }

        /* Chi cho phep duy nhat 1 dau cham, khong o dau/cuoi */
        if (str[i] == '.' && dotCount == 0 && i != 0 && i != len - 1)
        {
            dotCount++;
            continue;
        }
        return 0;
    }

    return digitCount > 0;
}

int isIntegerString(char *str)
{
    size_t len = strlen(str);
    if (len == 0)
    {
        return 0;
    }
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit((unsigned char)str[i]))
        {
            return 0;
        }
    }
    return 1;
}

int isValidDateFormat(char *date)
{
    // do dai phai dung 10 ky tu: dd/mm/yyyy
    if (strlen(date) != 10)
        return 0;

    // Vi tri 2 va 5 phai la '/'
    if (date[2] != '/' || date[5] != '/')
        return 0;

    // Cac vi tri khac phai la so
    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
        { // bo qua dau '/'
            continue;
        }
        if (!isdigit((unsigned char)date[i]))
        {
            return 0;
        }
    }

    // Tach ngay, thang, nam
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = atoi(date + 6);

    // Kiem tra thang hop le
    if (month < 1 || month > 12)
    {
        return 0;
    }
    // So ngay toi da theo thang
    int maxDay;
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        maxDay = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        maxDay = 30;
        break;
    case 2:
        // Nam nhuan: chia het cho 400 hoac (chia het cho 4 nhung khong chia het cho 100)
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
        {
            maxDay = 29;
        }
        else
        {
            maxDay = 28;
        }
        break;
    }
    if (day < 1 || day > maxDay)
    {
        return 0;
    }

    return 1; // hop le
}

/* Helper: Kiem tra chuoi co chua chu so khong */
int hasDigit(char *str)
{
    if (str == NULL)
        return 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if (isdigit((unsigned char)str[i]))
        {
            return 1;
        }
    }
    return 0;
}

/* Helper: Nhap va validate input y/n */
int getYesNoInput(const char *prompt)
{
    char buffer[10];
    do
    {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0)
        {
            printf("Loi: Khong duoc de trong!\n");
            continue;
        }

        char choice = tolower(buffer[0]);
        if (choice == 'y')
            return 1;
        if (choice == 'n')
            return 0;
        printf("Loi: Chi duoc nhap y hoac n!\n");
    } while (1);
}

/* Helper: Nhap chuoi khong duoc de trong */
int getNonEmptyString(char *buffer, size_t size, const char *prompt)
{
    do
    {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0)
        {
            printf("Loi: Khong duoc de trong!\n");
            return 0;
        }
        return 1;
    } while (0);
}

/* Helper: In header cua bang nhan vien */
void printEmployeeHeader()
{
    char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
    printf("%s", line);
    printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
    printf("%s", line);
}

/* Helper: In bang nhan vien tu startIdx den endIdx */
void printEmployeeTable(int startIdx, int endIdx)
{
    char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
    printEmployeeHeader();
    for (int i = startIdx; i < endIdx; i++)
    {
        printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n",
               employees[i].empId, employees[i].name, employees[i].position,
               employees[i].baseSalary, employees[i].workDays);
        printf("%s", line);
    }
}

void printfMenu()
{
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

void addEmployee()
{
    char continueInput = 'n';
    char buffer[100];
    do
    {
        if (empCount >= MAX_EMP)
        {
            printf("Danh sach da day!\n");
            break;
        }

        struct Employee emp;
        int valid = 0;

        printf("\n----- THEM NHAN VIEN MOI -----\n");

        /* Buoc 1: Nhap va kiem tra ma nhan vien khong trung lap */
        do
        {
            printf("Nhap Ma Nhan Vien (ID): ");
            fgets(emp.empId, sizeof(emp.empId), stdin);
            removeNewline(emp.empId);

            if (strlen(emp.empId) == 0)
            {
                printf("Loi: Khong duoc de trong! Vui long nhap lai.\n");
                valid = 0;
            }
            else if (findEmpIndex(emp.empId) != -1)
            {
                printf("Loi: Ma ID da ton tai! Vui long nhap lai.\n");
                valid = 0;
            }
            else
            {
                valid = 1;
            }
        } while (!valid);

        /* Buoc 2: Nhap ten, dam bao co chu cai va khong chua so */
        do
        {
            printf("Nhap Ho Ten: ");
            fgets(emp.name, sizeof(emp.name), stdin);
            removeNewline(emp.name);

            if (strlen(emp.name) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            }
            else if (containsLetter(emp.name) == 0)
            {
                printf("Loi: Ten phai chua chu cai!\n");
                valid = 0;
            }
            else
            {
                /* Su dung ham helper de kiem tra chu so */
                if (hasDigit(emp.name))
                {
                    printf("Loi: Ten khong duoc chua so!\n");
                    valid = 0;
                }
                else
                {
                    valid = 1;
                }
            }
        } while (!valid);

        /* Buoc 3: Nhap chuc vu, chi cho phep chu cai/khoang trang */
        do
        {
            printf("Nhap Chuc Vu: ");
            fgets(emp.position, sizeof(emp.position), stdin);
            removeNewline(emp.position);

            if (strlen(emp.position) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            }
            else
            {
                /* Su dung ham helper de kiem tra chu so */
                if (hasDigit(emp.position))
                {
                    printf("Loi: Chuc vu khong duoc chua so!\n");
                    valid = 0;
                }
                else
                {
                    valid = 1;
                }
            }
        } while (!valid);

        /* Buoc 4: Nhap luong co ban > 0, cho phep so thuc */
        do
        {
            printf("Nhap Luong co ban: ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);

            if (strlen(buffer) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            }
            else if (!isNumberString(buffer))
            {
                printf("Loi: Vui long nhap so!\n");
                valid = 0;
            }
            else
            {
                emp.baseSalary = atof(buffer);
                if (emp.baseSalary <= 0)
                {
                    printf("Loi: Luong phai > 0!\n");
                    valid = 0;
                }
                else
                {
                    valid = 1;
                }
            }
        } while (!valid);

        emp.workDays = 0;
        employees[empCount] = emp;
        empCount++;

        printf("Them Thanh Cong !\n");

        /* Hoi tiep tuc nhap nhan vien khac - su dung ham helper */
        int continueChoice = getYesNoInput("-Tiep tuc them ? (y/n): ");
        continueInput = continueChoice ? 'y' : 'n';

    } while (continueInput == 'y');
}

void updateEmployee()
{
    char targetId[20];
    int valid = 0;
    printf("\n--------- CAP NHAT HO SO NHAN VIEN ---------\n");

    do
    {
        printf("Nhap Ma Nhan Vien can sua (ID): ");
        fgets(targetId, sizeof(targetId), stdin);
        removeNewline(targetId);
        if (strlen(targetId) == 0)
        {
            printf("Loi: Khong duoc de trong!\n");
            valid = 0;
        }
        else
        {
            valid = 1;
        }
    } while (!valid);

    int index = findEmpIndex(targetId);

    if (index == -1)
    {
        printf("Khong Tim Thay Ho So Nhan Vien!\n");
    }
    else
    {
        char line[] = "+------------+----------------------+-----------------+-----------------+\n";
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15.0f |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary);
        printf("%s", line);

        /* Buoc 1: Hoi co muon doi chuc vu khong - su dung ham helper */
        if (getYesNoInput("\nBan Co Muon Thay Doi Chuc Vu ? (y/n): "))
        {
            do
            {
                printf("Nhap Chuc Vu Moi: ");
                fgets(employees[index].position, sizeof(employees[index].position), stdin);
                removeNewline(employees[index].position);

                if (strlen(employees[index].position) == 0)
                {
                    printf("Loi: Khong Duoc De Trong!\n");
                    valid = 0;
                }
                else
                {
                    /* Su dung ham helper de kiem tra chu so */
                    if (hasDigit(employees[index].position))
                    {
                        printf("Loi: Khong duoc chua so!\n");
                        valid = 0;
                    }
                    else
                    {
                        valid = 1;
                    }
                }
            } while (!valid);
            printf("Cap Nhat Thanh Cong!\n");
        }

        /* Buoc 2: Hoi co muon doi luong co ban khong - su dung ham helper */
        if (getYesNoInput("Ban Co Muon Thay Doi Luong CB ? (y/n): "))
        {
            char salBuffer[50];
            double newSalary;
            do
            {
                printf("Nhap Luong CB Moi: ");
                fgets(salBuffer, sizeof(salBuffer), stdin);
                removeNewline(salBuffer);

                if (strlen(salBuffer) == 0)
                {
                    printf("Loi: Khong Duoc De Trong!\n");
                    valid = 0;
                }
                else if (!isNumberString(salBuffer))
                {
                    printf("Loi: Luong CB Phai La So!\n");
                    valid = 0;
                }
                else
                {
                    newSalary = atof(salBuffer);
                    if (newSalary <= 0)
                    {
                        printf("Loi: Luong phai > 0!\n");
                        valid = 0;
                    }
                    else
                    {
                        employees[index].baseSalary = newSalary;
                        valid = 1;
                    }
                }
            } while (!valid);
            printf("Cap Nhat Thanh Cong!\n");
        }

        printf("\n--- THONG TIN SAU CAP NHAT ---\n");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB");
        printf("%s", line);
        printf("| %-10s | %-20s | %-15s | %-15.0f |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary);
        printf("%s", line);
    }
}

void deleteEmployee()
{
    char targetId[20];
    int valid = 0;

    printf("\n-------- XOA HO SO NHAN VIEN --------\n");

    do
    {
        printf("Nhap Ma Nhan Vien can xoa (ID): ");
        fgets(targetId, sizeof(targetId), stdin);
        removeNewline(targetId);

        if (strlen(targetId) == 0)
        {
            printf("Loi: Khong duoc de trong!\n");
        }
        else
        {
            valid = 1;
        }
    } while (!valid);

    int index = findEmpIndex(targetId);

    if (index == -1)
    {
        printf("Khong tim thay nhan vien co ID \"%s\"!\n", targetId);
        return;
    }

    char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
    printf("%s", line);
    printf("| %-10s | %-20s | %-15s | %-15s | %-10s |\n", "ID", "Ho Ten", "Chuc Vu", "Luong CB", "Ngay Cong");
    printf("%s", line);
    printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n", employees[index].empId, employees[index].name, employees[index].position, employees[index].baseSalary, employees[index].workDays);
    printf("%s", line);

    /* Su dung ham helper de xac nhan xoa */
    if (getYesNoInput("Xac Nhan Xoa Nhan Vien (y/n): "))
    {
        for (int i = index; i < empCount - 1; i++)
        {
            employees[i] = employees[i + 1];
        }
        empCount--;
        printf("Da xoa nhan vien thanh cong!\n");
    }
    else
    {
        printf("Da huy thao tac xoa.\n");
    }
}

void listEmployee()
{
    int perPage = 3;
    int totalPage;
    int page;
    char buffer[10];

    if (empCount == 0)
    {
        printf("\n--- DANH SACH NHAN VIEN ---\n");
        printf("Danh sach trong!\n");
        return;
    }

    totalPage = (empCount + perPage - 1) / perPage;

    do
    {
        printf("\nTong so trang: %d\n", totalPage);
        printf("Nhap So Trang Muon Xem (1-%d, 0 De Thoat): ", totalPage);
        fgets(buffer, sizeof(buffer), stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0)
        {
            printf("Loi: Khong duoc de trong!\n");
            continue;
        }

        if (!isIntegerString(buffer))
        {
            printf("Loi: So trang phai la so nguyen!\n");
            continue;
        }

        page = atoi(buffer);

        if (page == 0)
        {
            printf("Thoat Hien Thi Danh Sach!\n");
            break;
        }
        if (page < 1 || page > totalPage)
        {
            printf("Loi: Trang Khong Hop Le (1-%d)!\n", totalPage);
            continue;
        }

        /* Tinh chi so bat dau/ket thuc cho trang hien tai */
        int start = (page - 1) * perPage;
        int end = start + perPage;
        if (end > empCount)
        {
            end = empCount;
        }

        printf("TRANG %d/%d\n", page, totalPage);
        printf("---------- DANH SACH NHAN VIEN ----------\n");
        /* Su dung ham helper de in bang */
        printEmployeeTable(start, end);
    } while (1);
}

void searchEmployeeByName()
{
    if (empCount == 0)
    {
        printf("Danh sach nhan vien hien dang trong.\n");
        return;
    }

    char buffer[50];

    do
    {
        do
        {
            printf("\nNhap ten nhan vien can tim: ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);

            if (strlen(buffer) == 0)
            {
                printf("Loi: Khong duoc de trong! Vui long nhap lai.\n");
                continue;
            }

            /* Su dung ham helper de kiem tra chu so */
            if (hasDigit(buffer))
            {
                printf("Loi: Ten khong duoc chua so!\n");
                buffer[0] = '\0';
            }

        } while (strlen(buffer) == 0);

        /* Chuyen keyword ve lowercase de so khop khong phan biet hoa thuong */
        char keyword[50];
        strcpy(keyword, buffer);
        for (int i = 0; keyword[i]; i++)
        {
            keyword[i] = tolower(keyword[i]);
        }

        int found = 0;
        printf("\n---------- KET QUA TIM KIEM ----------\n");
        char line[] = "+------------+----------------------+-----------------+-----------------+------------+\n";
        printEmployeeHeader();

        /* Toi uu: tao ban sao nho hon va su dung strstr */
        for (int i = 0; i < empCount; i++)
        {
            /* Tao ban sao ten de chuyen ve lowercase (can thiet cho strstr) */
            char temp[50];
            size_t nameLen = strlen(employees[i].name);
            if (nameLen >= sizeof(temp))
                nameLen = sizeof(temp) - 1;

            /* Chuyen doi ten ve lowercase */
            for (size_t j = 0; j < nameLen; j++)
            {
                temp[j] = tolower((unsigned char)employees[i].name[j]);
            }
            temp[nameLen] = '\0';

            /* So khop chuoi con: tim thay neu keyword nam trong ten */
            if (strstr(temp, keyword) != NULL)
            {
                printf("| %-10s | %-20s | %-15s | %-15.0f | %-10d |\n",
                       employees[i].empId, employees[i].name, employees[i].position,
                       employees[i].baseSalary, employees[i].workDays);
                printf("%s", line);
                found = 1;
            }
        }

        if (!found)
        {
            printf("| %-80s |\n", "Khong tim thay ket qua nao!");
            printf("%s", line);
        }

        /* Su dung ham helper de hoi tiep tuc */
    } while (getYesNoInput("\nBan co muon tim kiem tiep khong? (y/n): "));
}

void sortByBaseSalary()
{
    if (empCount == 0)
    {
        printf("Danh sach nhan vien hien dang trong.\n");
        return;
    }

    char buffer[20];
    int sortOrder;

    do
    {
        sortOrder = -1;

        do
        {
            printf("\nChon thu tu sap xep theo luong:\n");
            printf("1. Tang dan\n");
            printf("2. Giam dan\n");
            printf("Nhap lua chon (1 hoac 2): ");
            fgets(buffer, sizeof(buffer), stdin);
            removeNewline(buffer);

            if (strlen(buffer) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                continue;
            }

            if (!isIntegerString(buffer))
            {
                printf("Loi: Lua chon phai la so nguyen!\n");
                continue;
            }

            sortOrder = atoi(buffer);

            if (sortOrder != 1 && sortOrder != 2)
            {
                printf("Loi: Chi duoc chon 1 hoac 2!\n");
                sortOrder = -1;
            }

        } while (sortOrder == -1);

        /* Bubble sort toi uu: dung som neu da sap xep xong */
        int swapped;
        for (int i = 0; i < empCount - 1; i++)
        {
            swapped = 0;
            for (int j = 0; j < empCount - i - 1; j++)
            {
                int needSwap = 0;

                if (sortOrder == 1)
                {
                    /* Tang dan: doi cho neu luong truoc lon hon luong sau */
                    if (employees[j].baseSalary > employees[j + 1].baseSalary)
                    {
                        needSwap = 1;
                    }
                }
                else
                {
                    /* Giam dan: doi cho neu luong truoc nho hon luong sau */
                    if (employees[j].baseSalary < employees[j + 1].baseSalary)
                    {
                        needSwap = 1;
                    }
                }

                if (needSwap)
                {
                    struct Employee temp = employees[j];
                    employees[j] = employees[j + 1];
                    employees[j + 1] = temp;
                    swapped = 1;
                }
            }
            /* Neu khong co swap nao trong lan quet nay, da sap xep xong */
            if (!swapped)
                break;
        }

        printf("Da sap xep thanh cong theo thu tu %s!\n", sortOrder == 1 ? "tang dan" : "giam dan");

        printf("---------- DANH SACH SAU KHI SAP XEP ----------\n");
        /* Su dung ham helper de in bang */
        printEmployeeTable(0, empCount);

        /* Su dung ham helper de hoi tiep tuc */
    } while (getYesNoInput("\nBan co muon sap xep tiep khong? (y/n): "));
}

void checkIn()
{
    do
    {
        char targetId[20];
        char date[20];
        char statusChoice[10];
        int valid = 0;

        printf("\n--------- CHAM CONG NGAY ---------\n");

        do
        {
            printf("Nhap Ma Nhan Vien (ID): ");
            fgets(targetId, sizeof(targetId), stdin);
            removeNewline(targetId);

            if (strlen(targetId) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            }
            else
            {
                valid = 1;
            }
        } while (!valid);

        int index = findEmpIndex(targetId);
        if (index == -1)
        {
            printf("Loi: Ma Nhan Vien Khong Ton Tai!\n");
        }
        else
        {

            /* Buoc 1: Nhap ngay dd/mm/yyyy va validate dinh dang */
            do
            {
                printf("Nhap Ngay Cham Cong (DD/MM/YYYY): ");
                fgets(date, sizeof(date), stdin);
                removeNewline(date);

                if (strlen(date) == 0)
                {
                    printf("Loi: Khong duoc de trong!\n");
                    valid = 0;
                }
                else if (!isValidDateFormat(date))
                {
                    printf("Loi: Dinh dang ngay khong hop le!\n");
                    valid = 0;
                }
                else
                {
                    valid = 1;
                }
            } while (!valid);

            if (isCheckedIn(targetId, date))
            {
                printf("Loi: Nhan vien %s da co ban ghi ngay %s!\n", employees[index].name, date);
            }
            else
            {

                valid = 0;
                /* Buoc 2: Chon trang thai 1 (Di lam) / 2 (Nghi lam) */
                do
                {
                    printf("Nhap Trang Thai (1: Di lam, 2: Nghi lam): ");
                    fgets(statusChoice, sizeof(statusChoice), stdin);
                    removeNewline(statusChoice);

                    if (strlen(statusChoice) == 0)
                    {
                        printf("Loi: khong duoc de trong !\n");
                        valid = 0;
                        continue;
                    }
                    if (strcmp(statusChoice, "1") == 0 || strcmp(statusChoice, "2") == 0)
                    {
                        valid = 1;
                    }
                    else
                    {
                        printf("Loi: Chi duoc nhap 1 hoac 2!\n");
                        valid = 0;
                    }
                } while (!valid);

                if (logCount >= MAX_LOG)
                {
                    printf("Loi: Danh sach cham cong da day!\n");
                }
                else
                {
                    /* Buoc 3: Tao log moi, cap nhat ngay cong neu di lam */
                    struct TimeSheet newLog;
                    sprintf(newLog.logId, "L%04d", logCount + 1);
                    strcpy(newLog.empId, targetId);
                    strcpy(newLog.date, date);

                    if (strcmp(statusChoice, "1") == 0)
                    {
                        strcpy(newLog.status, "Di lam");
                        /* Neu di lam thi tang workDays de dung cho hien thi & tinh luong */
                        employees[index].workDays++;
                        printf("Cham Cong Thanh Cong! Tong ngay cong hien tai: %d\n", employees[index].workDays);
                    }
                    else
                    {
                        strcpy(newLog.status, "Nghi lam");
                        printf("Da ghi nhan trang thai NGHI LAM cho nhan vien %s\n", employees[index].name);
                    }

                    timesheets[logCount] = newLog;
                    logCount++;
                }
            }
        }
        /* Su dung ham helper de hoi tiep tuc */
    } while (getYesNoInput("\nBan co muon cham cong tiep khong? (y/n): "));
}

void viewTimeSheet()
{
    char targetId[20];
    int valid = 0;

    do
    {
        printf("\n--------- XEM LICH SU CHAM CONG ---------\n");
        do
        {
            printf("Nhap Ma Nhan Vien Can Xem: ");
            fgets(targetId, sizeof(targetId), stdin);
            removeNewline(targetId);
            if (strlen(targetId) == 0)
            {
                printf("Loi: Khong duoc de trong!\n");
                valid = 0;
            }
            else
            {
                valid = 1;
            }
        } while (!valid);

        if (findEmpIndex(targetId) == -1)
        {
            printf("Loi: Ma Nhan Vien Khong Ton Tai!\n");
        }
        else
        {
            char line[] = "+------------+----------------------+-----------------+\n";
            printf("\nBANG CONG CUA NHAN VIEN: %s\n", targetId);
            printf("%s", line);
            printf("| %-10s | %-20s | %-15s |\n", "Ma Log", "Ngay", "Trang Thai");
            printf("%s", line);

            int found = 0;
            for (int i = 0; i < logCount; i++)
            {
                if (strcmp(timesheets[i].empId, targetId) == 0)
                {
                    printf("| %-10s | %-20s | %-15s |\n", timesheets[i].logId, timesheets[i].date, timesheets[i].status);
                    printf("%s", line);
                    found = 1;
                }
            }
            if (!found)
            {
                printf("| %-51s |\n", "Chua co du lieu cham cong!");
                printf("%s", line);
            }
        }

        /* Su dung ham helper de hoi tiep tuc */
    } while (getYesNoInput("\nXem tiep nhan vien khac? (y/n): "));
}
