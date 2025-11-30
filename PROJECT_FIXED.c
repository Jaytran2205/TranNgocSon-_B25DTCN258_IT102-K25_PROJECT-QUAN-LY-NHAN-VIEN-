#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_EMP 100
#define PAGE_SIZE 5
#include <stdlib.h> 
#include <ctype.h>
#define MAX_TIMESHEET 1000   

typedef struct {
    char empId[20];        
    char checkInDate[11];  
} TimeSheet;

TimeSheet timeSheets[MAX_TIMESHEET];
int timeSheetCount = 0;   

typedef struct {
    char empId[20];      
    char name[50];       
    char position[15];   
    double baseSalary;   
    int workDays;
	long salary;       
} Employee;


Employee employees[MAX_EMP];
int empCount = 0;


void removeNewLine(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}
int isBlankString(const char *s) {
    if (s == NULL) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}



int findEmployeeIndexById(Employee a[], int n, const char *empId) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(a[i].empId, empId) == 0) return i;
    }
    return -1;
}

int isValidDate(const char *s) {

    if (strlen(s) != 10) return 0;
    if (s[2] != '/' || s[5] != '/') return 0;

    char ddStr[3], mmStr[3], yyStr[5];
    ddStr[0] = s[0]; ddStr[1] = s[1]; ddStr[2] = '\0';
    mmStr[0] = s[3]; mmStr[1] = s[4]; mmStr[2] = '\0';
    yyStr[0] = s[6]; yyStr[1] = s[7]; yyStr[2] = s[8]; yyStr[3] = s[9]; yyStr[4] = '\0';

    int d = atoi(ddStr);
    int m = atoi(mmStr);
    int y = atoi(yyStr);

    if (d <= 0 || m <= 0 || y <= 0) return 0;
    if (m < 1 || m > 12) return 0;

    int maxDay;
    switch (m) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            maxDay = 31; break;
        case 4: case 6: case 9: case 11:
            maxDay = 30; break;
        case 2:
            maxDay = 29; break;
        default:
            return 0;
    }
    if (d < 1 || d > maxDay) return 0;

    return 1;
}

int isDuplicatedTimeSheet(const char *empId, const char *date) {
    for (int i = 0; i < timeSheetCount; ++i) {
        if (strcmp(timeSheets[i].empId, empId) == 0 &&
            strcmp(timeSheets[i].checkInDate, date) == 0) {
            return 1; 
        }
    }
    return 0;
}


void printEmployee(const Employee *e) {
    printf("%-10s | %-20s | %-10s | %10.0lf | %3d\n",
           e->empId, e->name, e->position, e->baseSalary, e->workDays);
}

void addEmployee(Employee a[], int *n) {
    if (*n >= MAX_EMP) {
        printf("Danh sach nhan vien da day, khong the them moi!\n");
        return;
    }

    Employee e;
    char buffer[100];

    while (1) {
        printf("Nhap ma nhan vien (empId): ");
        fgets(e.empId, sizeof(e.empId), stdin);
        removeNewLine(e.empId);

        if (isBlankString(e.empId)) {
            printf(">> empId khong duoc rong. Vui long nhap lai!\n\n");
            continue;
        }

        if (findEmployeeIndexById(a, *n, e.empId) != -1) {
            printf(">> empId da ton tai. Vui long nhap ma khac!\n");
            continue;
        }
        break;
    }


    while (1) {
        printf("Nhap ten nhan vien: ");
        fgets(e.name, sizeof(e.name), stdin);
        removeNewLine(e.name);

        if (isBlankString(e.name)) {
            printf(">> Ten khong duoc rong. Vui long nhap lai!\n");
            continue;
        }
        break;
    }

    
    printf("Nhap chuc vu: ");
    fgets(e.position, sizeof(e.position), stdin);
    removeNewLine(e.position);

    while (1) {
        printf("Nhap luong co ban: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%lf", &e.baseSalary) != 1 || e.baseSalary <= 0) {
            printf(">> Luong khong hop le (phai > 0). Vui long nhap lai!\n");
            continue;
        }
        break;
    }


   e.workDays=0;

    a[*n] = e;
    (*n)++;

    printf(">> Them nhan vien thanh cong!\n");
}


void updateEmployee(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong, khong co gi de cap nhat.\n");
        return;
    }

    char empId[20];
    char buffer[100];

    printf("Nhap ma nhan vien can cap nhat: ");
    fgets(empId, sizeof(empId), stdin);
    removeNewLine(empId);

    int idx = findEmployeeIndexById(a, n, empId);
    if (idx == -1) {
        printf(">> Khong tim thay nhan vien co ma %s.\n", empId);
        return;
    }

    Employee *e = &a[idx];

    printf("\nThong tin hien tai:\n");
    printf("empId       : %s\n", e->empId);
    printf("Ten         : %s\n", e->name);
    printf("Chuc vu     : %s\n", e->position);
    printf("Luong co ban: %.f\n\n", e->baseSalary);

    printf("Nhap chuc vu moi (bo trong neu giu nguyen): ");
    fgets(buffer, sizeof(buffer), stdin);
    removeNewLine(buffer);
    if (strlen(buffer) > 0) {
        strncpy(e->position, buffer, sizeof(e->position) - 1);
        e->position[sizeof(e->position) - 1] = '\0';
    }


    printf("Nhap luong co ban moi (<=0 neu giu nguyen): ");
    fgets(buffer, sizeof(buffer), stdin);
    double newSalary;
    if (sscanf(buffer, "%lf", &newSalary) == 1 && newSalary > 0) {
        e->baseSalary = newSalary;
    }

    printf(">> Cap nhat ho so thanh cong!\n");
}

void deleteEmployee(Employee a[], int *n) {
    if (*n == 0) {
        printf("Danh sach nhan vien rong, khong co gi de xoa.\n");
        return;
    }

    char empId[20];
    char buffer[100];

   
    while (1) {
        printf("Nhap ma nhan vien can xoa: ");
        fgets(empId, sizeof(empId), stdin);
        removeNewLine(empId);

        if (isBlankString(empId)) {
            printf(">> Ma nhan vien khong duoc rong. Vui long nhap lai!\n");
            continue;
        }

        break;
    }

   
    int idx = findEmployeeIndexById(a, *n, empId);
    if (idx == -1) {
        printf(">> Nhan vien khong ton tai!\n");
        return;
    }

   
    printf("Ban co chac chan muon XOA nhan vien %s? (y/n): ", empId);
    fgets(buffer, sizeof(buffer), stdin);

    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        printf(">> Da huy thao tac xoa.\n");
        return;
    }

    
    for (int i = idx; i < *n - 1; i++) {
        a[i] = a[i + 1];
    }

    (*n)--;

    printf(">> Da xoa nhan vien thanh cong!\n");
}



void printAllEmployees(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong.\n");
        return;
    }
    printf("========================================================================");
    printf("\n|%-10s| %-20s | %-10s |%10s  | %3s|\n",
           "empId","Ten", "Chuc vu", "Luong", "Ngay Cong");
    printf("========================================================================\n");
    for (int i = 0; i < n; ++i) {
        printEmployee(&a[i]);
    }
}


void initSampleEmployees() {
    Employee sample[] = {
        {"E001", "Nguyen Hoang Khang", "NHAN VIEN",     15000000, 12},
        {"E002", "Tran Thi Binh",   "NHAN VIEN",        9000000, 10},
        {"E003", "Le Van Cao",     "NHAN VIEN",        8500000,  9},
        {"E004", "Pham Thi Duyen",   "QUAN LY",  12000000, 11},
        {"E005", "Hoang Van Em",  "BAO VE",        7000000, 12},
        {"E006", "Vu Thi Phuong",     "THUC TAP", 8000000,  8},
        {"E007", "Nguyen Van Giang", "NHAN VIEN",        9500000, 10},
        {"E008", "Tran Thi Huyen",   "TUYEN DUNG",          11000000, 12},
        {"E009", "Le Van Inh",     "LAI XE",       7500000,  9},
        {"E010", "Pham Thi Khang",   "NHAN VIEN",      6000000, 12},
        {"E011", "Le Thi Lan",     "NHAN VIEN",        9000000, 10},
        {"E012", "Nguyen Van Minh", "QUAN LY",     16000000, 11},
        {"E013", "Tran Thi Nguyen",   "QUAN LY",  12500000, 12},
        {"E014", "Hoang Van Phuc",  "NHAN VIEN",        8800000,  9},
        {"E015", "Vu Thi Quyen",     "THUC TAP", 8200000, 10}
    };

    int sampleCount = sizeof(sample) / sizeof(sample[0]);

    for (int i = 0; i < sampleCount && empCount < MAX_EMP; i++) {
        employees[empCount++] = sample[i];
    }

   
}



void displayEmployeesByPage(Employee employees[], int empCount) {
    if (empCount == 0) {
        printf("\nEmpty employee list.\n");
        printf("Nhan phim bat ky de quay lai menu...");
        getchar();
        system("cls");
        return;
    }

    int totalPages = (empCount + PAGE_SIZE - 1) / PAGE_SIZE;
    int page = 1;                 
    char input[16];

    while (1) {
        system("cls");

        
        printf("\n============= DANH SACH NHAN VIEN (PAGE %d / %d) ==================\n",
               page, totalPages);
        printf("Id   | %-20s | %-10s | %-10s | %-8s\n",
               "Ten", "Chuc vu", "Luong", "NC");
        printf("-----------------------------------------------------------------\n");

        int start = (page - 1) * PAGE_SIZE;
        int end   = start + PAGE_SIZE;
        if (end > empCount) end = empCount;

        for (int i = start; i < end; ++i) {
            Employee e = employees[i];
            printf("%-4s | %-20s | %-10s | %-10.0lf | %-8d\n",
                   e.empId, e.name, e.position, e.baseSalary, e.workDays);
        }

       
        printf("\n---------------------------------------------------------------");
        printf("\nNhap so trang 1/%d de chon trang", totalPages);
        printf("\nHoac: n = Next, p = Prev, q = Thoat");
        printf("\nLua chon: ");

        if (!fgets(input, sizeof(input), stdin)) {
            
            system("cls");
            return;
        }

        
        if (input[0] == '\n' || input[0] == '\0') {
            continue;
        }

        
        if (input[0] == 'q' || input[0] == 'Q') {
            system("cls");        
            return;
        }

       
        if (input[0] == 'n' || input[0] == 'N') {
            if (page < totalPages) {
                page++;
            } else {
                printf("\nBan dang o trang cuoi cung!");
                printf("\nNhan phim bat ky de tiep tuc...");
                getchar();
            }
            continue;
        }

        
        if (input[0] == 'p' || input[0] == 'P') {
            if (page > 1) {
                page--;
            } else {
                printf("\nBan dang o trang dau tien!");
                printf("\nNhan phim bat ky de tiep tuc...");
                getchar();
            }
            continue;
        }

        if (isdigit((unsigned char)input[0])) {
            int newPage = atoi(input);
            if (newPage >= 1 && newPage <= totalPages) {
                page = newPage;
            } else {
                printf("\nTrang khong hop le!");
                printf("\nNhan phim bat ky de tiep tuc...");
                getchar();
            }
            continue;
        }

        printf("\nLua chon khong hop le!");
        printf("\nNhan phim bat ky de tiep tuc...");
        getchar();
    }
}





void searchEmployeeByName(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong.\n");
        return;
    }

    char name[50];
    int found = 0;

    printf("Nhap ten nhan vien can tra cuu: ");
    fgets(name, sizeof(name), stdin);
    removeNewLine(name);

    if (isBlankString(name)) {
        printf(">> Ten nhan vien khong duoc rong.\n");
        return;
    }

   
    char key[50];
    int lenKey = strlen(name);
    for (int i = 0; i < lenKey; ++i) {
        key[i] = tolower((unsigned char)name[i]);
    }
    key[lenKey] = '\0';

    printf("\nKet qua tra cuu theo ten \"%s\":\n", name);
    printf("%-10s | %-20s | %-10s | %10s | %3s\n",
           "| empId", "Ten", "Chuc vu", "Luong", "NC |");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
       
        char empName[50];
        int lenEmp = strlen(a[i].name);
        for (int j = 0; j < lenEmp; ++j) {
            empName[j] = tolower((unsigned char)a[i].name[j]);
        }
        empName[lenEmp] = '\0';

        
        if (strstr(empName, key) != NULL) {
            printEmployee(&a[i]);
            found = 1;
        }
    }

    if (!found) {
        printf(">> Khong tim thay nhan vien nao co ten chua \"%s\".\n", name);
    }
}




void sortEmployeesBySalary(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong.\n");
        return;
    }

    int choice;
    char buffer[10];

    printf("Chon kieu sap xep:\n");
    printf("1. Tang dan theo luong\n");
    printf("2. Giam dan theo luong\n");
    printf("Chon: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Lua chon khong hop le.\n");
        return;
    }

    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            int condition;
            if (choice == 1) {
               
                condition = (a[j].baseSalary > a[j + 1].baseSalary);
            } else {
                
                condition = (a[j].baseSalary < a[j + 1].baseSalary);
            }
            if (condition) {
                Employee tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }

    printf(">> Da sap xep danh sach nhan vien theo luong (luu vao bo nho).\n");
}



int getDaysInMonth(int month, int year) {
    if (month < 1 || month > 12) return 0;

    switch (month) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2: {
            int isLeap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
            return isLeap ? 29 : 28;
        }
        default:
            return 0;
    }
}





void recordWorkDays(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong.\n");
        return;
    }

    char empId[20];
    char date[20];

    
    printf("Nhap ID nhan vien can cham cong: ");
    fgets(empId, sizeof(empId), stdin);
    removeNewLine(empId);

    if (isBlankString(empId)) {
        printf(">> Ma nhan vien khong duoc rong.\n");
        return;
    }

    int idx = findEmployeeIndexById(a, n, empId);
    if (idx == -1) {
        printf(">> Khong tim thay nhan vien co ma %s.\n", empId);
        return;
    }

  
    while (1) {
        printf("Nhap ngay cham cong (dd/mm/yyyy): ");
        fgets(date, sizeof(date), stdin);
        removeNewLine(date);

        if (isBlankString(date)) {
            printf(">> Ngay cham cong khong duoc rong.\n");
            continue;
        }

        

        break; 
    }

    
    if (isDuplicatedTimeSheet(empId, date)) {
        printf(">> Nhan vien %s da duoc cham cong vao ngay %s.\n", empId, date);
        return;
    }

   
    a[idx].workDays++;

    
    if (timeSheetCount >= MAX_TIMESHEET) {
        printf(">> Bang cham cong da day, khong the luu them.\n");
        printf("   (Ngay cong cua nhan vien van da duoc tang).\n");
    } else {
        strcpy(timeSheets[timeSheetCount].empId, empId);
        strcpy(timeSheets[timeSheetCount].checkInDate, date);
        timeSheetCount++;
    }

    printf(">> Cham cong thanh cong cho nhan vien %s vao ngay %s.\n", empId, date);
    printf("   Tong ngay cong hien tai: %d\n", a[idx].workDays);
}








void viewPersonalTimeSheet(Employee a[], int n) {
    if (n == 0) {
        printf("Danh sach nhan vien rong, khong co bang cong de xem.\n");
        return;
    }

    if (timeSheetCount == 0) {
        printf("Chua co du lieu cham cong nao trong he thong.\n");
        printf("Nhan phim bat ky de quay lai menu...");
        getchar();
        system("cls");
        return;
    }

    char empId[20];
    char buffer[32];
    int month, year;

    
    printf("Nhap ma nhan vien can xem bang cong: ");
    fgets(empId, sizeof(empId), stdin);
    removeNewLine(empId);

    if (isBlankString(empId)) {
        printf(">> Ma nhan vien khong duoc rong.\n");
        return;
    }

    int idx = findEmployeeIndexById(a, n, empId);
    if (idx == -1) {
        printf(">> Khong tim thay nhan vien co ma %s trong danh sach.\n", empId);
        return;
    }

    
    while (1) {
        printf("Nhap thang/nam can xem (mm/yyyy): ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewLine(buffer);

        if (sscanf(buffer, "%d/%d", &month, &year) != 2) {
            printf(">> Dinh dang khong dung. Vui long nhap lai (vd: 11/2025)!\n");
            continue;
        }
        if (month < 1 || month > 12 || year < 1900 || year > 2100) {
            printf(">> Thang/nam khong hop le. Vui long nhap lai!\n");
            continue;
        }
        break;
    }

    int daysInMonth = getDaysInMonth(month, year);
    if (daysInMonth == 0) {
        printf(">> Thang/nam khong hop le.\n");
        return;
    }

    
    int worked[32] = {0}; 
    for (int i = 0; i < timeSheetCount; ++i) {
        if (strcmp(timeSheets[i].empId, empId) != 0) continue;

        int d, m, y;
        if (sscanf(timeSheets[i].checkInDate, "%d/%d/%d", &d, &m, &y) == 3) {
            if (m == month && y == year && d >= 1 && d <= daysInMonth) {
                worked[d] = 1;
            }
        }
    }

    system("cls");
    printf("=========== BANG CONG CA NHAN THANG %02d/%04d ===========\n", month, year);
    printf("Ma NV  : %s\n", empId);
    printf("Ten    : %s\n", a[idx].name);
    printf("Chuc vu: %s\n", a[idx].position);
    printf("---------------------------------------------------------\n");
    printf("%-12s | %-10s\n", "Ngay", "Trang thai");
    printf("---------------------------------------------------------\n");

    int totalWork = 0;
    for (int d = 1; d <= daysInMonth; ++d) {
        const char *status = worked[d] ? "Di lam" : "Nghi";
        if (worked[d]) totalWork++;
        printf("%02d/%02d/%04d | %-10s\n", d, month, year, status);
    }

    int totalOff = daysInMonth - totalWork;
    printf("---------------------------------------------------------\n");
    printf("Tong ngay di lam : %d\n", totalWork);
    printf("Tong ngay nghi   : %d\n", totalOff);
    printf("---------------------------------------------------------\n");
    printf("Nhan phim bat ky de quay lai menu...");
    getchar();
    system("cls");
}



void pauseAndClear() {
    printf("\nNhan phim bat ky de quay lai menu...");
    getchar();   
    getchar();   
    system("cls");
}





int main() {
    int choice;
    char buffer[10];
    
	initSampleEmployees();

    while (1) {
        printf("\033[31m=========================================================\033[0m\n");
printf("\033[33m|                 \033[1mMENU QUAN LY NHAN VIEN\033[0m\033[33m                |\033[0m\n");
printf("\033[31m=========================================================\033[0m\n");

printf("\033[35m|  \033[0m1.Them nhan vien moi \033[35m                                |\n");
printf("\033[35m|  \033[0m2.Cap nhat ho so nhan vien\033[35m                           |\n");
printf("\033[35m|  \033[0m3.Nhap vao ID nhan vien can xoa\033[35m                      |\n");
printf("\033[35m|  \033[0m4.Hien thi danh sach nhan vien\033[35m                       |\n");
printf("\033[35m|  \033[0m5.Nhap vao ten nhan vien can tra cuu\033[35m                 |\n");
printf("\033[35m|  \033[0m6.Xep hang nhan vien theo muc luong\033[35m                  |\n");
printf("\033[35m|  \033[0m7.Cham cong ngay theo ID\033[35m                             |\n");
printf("\033[35m|  \033[0m8.Xem bang cong \033[35m                                     |\n");
printf("\033[35m|  \033[0m9.Thoat\033[35m                                              |\n");

printf("\033[36m=========================================================\033[0m\n");
printf("\033[33mChon: ");


        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &choice) != 1) {
    printf("Lua chon khong hop le!\n");

    printf("Bam ENTER de tiep tuc...");  
    getchar();

    system("cls");   
    continue;
}


        switch (choice) {
        case 1:
            addEmployee(employees, &empCount);
            pauseAndClear();
            break;
        case 2:
            updateEmployee(employees, empCount);
            pauseAndClear();
            break;
        case 3:
            deleteEmployee(employees, &empCount);
            pauseAndClear();
            break;

       case 4:
            displayEmployeesByPage(employees, empCount);
            pauseAndClear();
            break;

        case 5:
		    searchEmployeeByName(employees, empCount);
		    pauseAndClear();
            break;
		case 6:
		    sortEmployeesBySalary(employees, empCount);
		    pauseAndClear();
            break;
		case 7:
		    recordWorkDays(employees, empCount);
		    pauseAndClear();
            break;
		case 8:
		    viewPersonalTimeSheet(employees, empCount);
		    pauseAndClear();
            break;;
        case 9:
             printf("============================================================\n");
             printf("                   CAM ON DA SU DUNG HE THONG               \n");
             printf("                          HEN GAP LAI                       \n");
             printf("============================================================\n");
            return 0;
        default:
            printf("Lua chon khong hop le!\n");
        }
    }
return 0;
}


