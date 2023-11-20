# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <stdbool.h>
# include <math.h>

#define masSize 8           // ʳ������ ����
#define snailSize 5         // ������� �������

RECT mas[masSize];          // �����
RECT snail[snailSize];      // ������

HWND bt_exit;               // ������ ������
HWND bt_next;               // ������ ���
HWND bt_build;              // ������ �������� �������
HWND bt_start;              // ������ ������� ������ ��������
HWND bt_save;               // ������ ����������

HWND text_welcome;          // ����� ��� ������ ��������
HWND text_size_1;           // ����� ��� ������ ������ ����� �������
HWND text_size_2;           // ����� ��� ������ ������ ����� �������
HWND text_result;           // ����� ��� ����� ����������
HWND text_save;             // ����� ��� ���������� �����

HWND combobox_matrix;       // ���� ������ ��������
HWND combobox_size_11;      // ���� ������ ������� ����� ����� �������
HWND combobox_size_12;      // ���� ������ ������� �������� ����� �������
HWND combobox_size_21;      // ���� ������ ������� ����� ����� �������
HWND combobox_size_22;      // ���� ������ ������� �������� ����� �������

HWND number_for_5;          // ���� ��� ����� �����, �� ��� ������� ��������� ������� (5)
HWND field_save;            // ���� ��� ����� ��'� ����� ���� ��������
HWND matrix_A[7][7];        // ������� � ��� �����
HWND matrix_B[7][7];        // ������� � ��� �����
HWND matrix_r[7][7];        // ����������� ������� ��� ������

bool game = true,                  // ���� ������ �����
     button_on,                    // �� ������� ������ next
     the_same = false,             // �� ��� ������� (1)
     matrix_degenerate = true;     // �� ������� ��������� (��������� = 0)


int button_build = 0,            // ������ �������� ������� (0 - �� ��������������, 1 - ����������, 2 - ����� �� ���������������)
    program = 1,                 // ��� �������� ��� ��������� �������
    button_check = -1,            // ������ ��� ������ ������ Next
    size_11 = 3,                 // �������� ������� ����� ����� �������
    size_12 = 3,                 // �������� ������� �������� ����� �������
    size_21 = 3,                 // �������� ������� ����� ����� �������
    size_22 = 3,                 // �������� ������� �������� ����� �������
    number_5 = 1,                // �����, �� ��� ������� ��������� ������� (5)
    determinant_7 = 0,           // ����������, ���� ������� ������� (7)
    snail_after_restart = 0;     // �����, ��� ������ ��������� �� ���� ��� ��������

int matrixA[7][7],               // ������� ������� ������� �
    matrixB[7][7],               // ������� ������� ������� �
    matrix[7][7];                // ������� ������� ������ ������� (2-7)

float matrix_8[7][7];            // ������� ������� ������ ������� (8)

char file_name[30] = {'\0'};     // ����� �����


// �������� � ���������
bool Compare_matrix(int n, int m, int arr1[7][7], int arr2[7][7]) {                                      // 1
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(arr1[i][j] != arr2[i][j]) return false;
        }
    }
    return true;
}

void Matrix_sum(int n, int m, int arr1[7][7], int arr2[7][7], int arr[7][7]) {                           // 2
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] = arr1[i][j] + arr2[i][j];
        }
    }
}

void Matrix_difference(int n, int m, int arr1[7][7], int arr2[7][7], int arr[7][7]) {                    // 3
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] = arr1[i][j] - arr2[i][j];
        }
    }
}

void Matrix_transpose(int n, int m, int arr1[7][7], int arr[7][7]) {                                     // 4
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[j][i] = arr1[i][j];
        }
    }
}

void Matrix_multiplication_number(int n, int m, int arr1[7][7], int number, int arr[7][7]) {             // 5
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] = arr1[i][j] * number;
        }
    }
}

void Matrix_multiplication_matrix(int n, int l, int m, int arr1[7][7], int arr2[7][7], int arr[7][7]) {  // 6
    int number;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            number = 0;
            for(int k = 0; k < l; k++) {
                number = number + arr1[i][k]*arr2[k][j];
            }
            arr[i][j] = number;
        }
    }
}

int Matrix_determinant(int n, int arr1[n][n], int number) {                                              // 7
    if(n == 1) return arr1[0][0];
    if(n == 2) return ((arr1[0][0]*arr1[1][1])-(arr1[0][1]*arr1[1][0]));

    for(int i = 0; i < n; i++) {
        int arr_new[n-1][n-1], index = 0;
        for(int j = 1; j < n; j++) {
            for(int k = 0; k < n; k++) {
                if(k == i) continue;
                arr_new[(int)index/n][index - (n-1)*((int) index/n)] = arr1[j][k];
                index++;
            }
        }
        number = pow(-1, 2+i)*arr1[0][i]*Matrix_determinant(n-1, arr_new, 0) + number;
        }
    return number;
}

bool Invertible_matrix(int n, int arr1[7][7], float arr[7][7]) {                                         // 8
    int matrix_part[n][n];                                    // ���� �������, ��� ����� �������� ��� ����������� �����������
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrix_part[i][j] = arr1[i][j];
        }
    }

    determinant_7 = Matrix_determinant(n, matrix_part, determinant_7);
    float determinant = determinant_7;                        // ���������� �������
    if(determinant == 0) return true;

    int arr_new[n-1][n-1], index = 0;
    for(int i = 0; i < n; i++) {                              // ����������� ����������� ����������� ��������� �� ����� �� � �������
        for(int j = 0; j < n; j++) {
            for(int p = 0; p < n; p++) {
                if(p == i) continue;
                for(int q = 0; q < n; q++) {
                    if(q == j)continue;
                    arr_new[(int)index/n][index - (n-1)*((int) index/n)] = arr1[p][q];
                    index++;
                }
            }
            index = 0;
            determinant_7 = 0;
            determinant_7 = Matrix_determinant(n-1, arr_new, determinant_7);
            arr[j][i] = determinant_7*pow(-1, i+j);
        }
    }

    float number_for_round;                                  // �������� ����� ��� ����������
    for(int i = 0; i < n; i++) {                             // ���������� ����� �� ��������
        for(int j = 0; j < n; j++) {
            number_for_round = arr[i][j]/determinant;
            number_for_round = rint(1000*number_for_round);
            number_for_round = number_for_round/1000;
            arr[i][j] = number_for_round;
        }
    }

    return false;
}


// �������� �����
bool Input_check_Z(char str[]) {    // �������� �� ��� ���� ������� ���� ����� �������
    bool no_plus_minus = true;

    for(int i = 0; i<4; i++) {
        if(str[i] == '\0' || str[0] == ' ') continue;

        else if((str[0] == '+' || str[0] == '-') && no_plus_minus) {
            no_plus_minus = false;
            if(str[1] < '0' || str[1] > '9') return false;
        }

        else if(str[i] < '0' || str[i] > '9') return false;
    }

    return true;
}


// ���������
void HomeScreen(HWND hwnd) {        // ��������� ������ ������ ��������
    text_welcome = CreateWindow("static", "Select the operation on the matrix",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            195, 180, 250, 20,
                            hwnd, NULL, NULL, NULL);

    combobox_matrix = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
                            195, 210, 250, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Comparison of matrices");          // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "The sum of matrices");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "The difference of matrices");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Transposed matrix");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Multiplying the matrix by a number");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Multiplication of matrices");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Determinant of the matrix");
        SendMessage(combobox_matrix, CB_ADDSTRING, 0, "Invertible matrix");
        SendMessage(combobox_matrix, CB_SETCURSEL, 0, 0);                              // �� ����������� ���� �������� ������� � �������� 1

}

void ButtonExitNext(HWND hwnd) {    // ��������� ������ Exit i Next
    bt_exit = CreateWindow("button", "Exit",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            400, 390, 100, 40,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd

    if(button_check != 2) {
        bt_next = CreateWindow("button", "Next",
                            WS_VISIBLE | WS_CHILD,
                            510, 390, 100, 40,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd
    } else {
        bt_next = CreateWindow("button", "Restart",
                            WS_VISIBLE | WS_CHILD,
                            510, 390, 100, 40,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd
    }
}

void FillMas() {                     // ���������� ������� �������
    srand(5);
    for(int i = 0; i < masSize; i++) {
        mas[i].left = rand() %  680;
        mas[i].top = rand() % 110;

    }
    snail[0].left = 670 - snail_after_restart;
    snail[1].left = 663 - snail_after_restart;
    snail[2].left = 653 - snail_after_restart;
    snail[3].left = 644 - snail_after_restart;
    snail[4].left = 612 - snail_after_restart;
}

void WinMove() {                    // ��� �������
    for(int i = 0; i < masSize; i++) {
        mas[i].left += 1 + rand() % 2;
        if(mas[i].left > 700) mas[i].left = -100;
        mas[i].right = mas[i].left + 30;
        mas[i].bottom = mas[i].top + 20;
    }
    int speed;
    speed = rand() % 2;
    for(int j = 0; j < 5; j++) {
        snail[j].left -= speed;
        if(snail[j].left < -10) snail[j].left = 700;
    }
    snail[0].top = 330;
    snail[0].right = snail[0].left - 60;
    snail[0].bottom = snail[0].top + 10;

    snail[1].top = 301;
    snail[1].right = snail[1].left - 40;
    snail[1].bottom = snail[1].top + 30;

    snail[2].top = 311;
    snail[2].right = snail[2].left - 10;
    snail[2].bottom = snail[2].top + 20;

    snail[3].top = 311;
    snail[3].right = snail[3].left - 10;
    snail[3]. bottom = snail[3].top + 10;

    snail[4].top = 311;
    snail[4].right = snail[4].left - 10;
    snail[4]. bottom = snail[4].top + 20;




}

void WinShow(HDC dc, HDC snail_body, HDC snail_circle, HDC snail_black) {   // �������
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(103, 232, 235));
    Rectangle(dc, -1, 0, 640, 140);

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(122, 245, 147));
    Rectangle(dc, -1, 280, 640, 345);

    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCBrushColor(dc, RGB(255, 255, 255));

    SelectObject(snail_body, GetStockObject(DC_BRUSH));
    SetDCBrushColor(snail_body, RGB(133, 73, 33));
    SelectObject(snail_circle, GetStockObject(DC_BRUSH));
    SetDCBrushColor(snail_circle, RGB(224, 184, 123));
    SelectObject(snail_black, GetStockObject(DC_BRUSH));
    SetDCBrushColor(snail_black, RGB(77, 53, 18));

    for(int i = 0; i < masSize; i++)
        Rectangle(dc, mas[i].left, mas[i].top, mas[i].right, mas[i].bottom);

    Rectangle(snail_body, snail[0].left, snail[0].top, snail[0].right, snail[0].bottom);
    Rectangle(snail_circle, snail[1].left, snail[1].top, snail[1].right, snail[1].bottom);
    Rectangle(snail_black, snail[2].left, snail[2].top, snail[2].right, snail[2].bottom);
    Rectangle(snail_black, snail[3].left, snail[3].top, snail[3].right, snail[3].bottom);
    Rectangle(snail_body, snail[4].left, snail[4].top, snail[4].right, snail[4].bottom);
}

void Background(HDC bg) {           // ���
    SelectObject(bg, GetStockObject(DC_BRUSH));
    SetDCBrushColor(bg, RGB(134, 225, 151));
    Rectangle(bg, -1, -1, 640, 480);
}

void EnterMatrixSize(HWND hwnd) {  // ���� ������ �������
    if(program == 1 || program == 2 || program == 3) {
    text_size_1 = CreateWindow("static", "Size of the matrix A and B",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            240, 20, 180, 20,
                            hwnd, NULL, NULL, NULL);
    combobox_size_11 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            // CBS_DROPDOWN - �� ����, �� CBS_DROPDOWN, ����� �� ��������� ������� ���
                            240, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_11, CB_SETCURSEL, 2, 0);

    combobox_size_12 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            315, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_12, CB_SETCURSEL, 2, 0);

    bt_build = CreateWindow("button", "OK",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            390, 50, 30, 24,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd
        return;
    }

    if(program != 7 && program != 8) {
    combobox_size_12 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            130, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_12, CB_SETCURSEL, 2, 0);
    }

    if(program == 4 || program == 5 || program == 6) {
        combobox_size_11 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            50, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_11, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_11, CB_SETCURSEL, 2, 0);
        }

    if(program == 4 || program == 5) {
        text_size_1 = CreateWindow("static", "Size of the matrix A",
                            WS_VISIBLE | WS_CHILD | ES_CENTER  | WS_BORDER,
                            50, 20, 140, 20,
                            hwnd, NULL, NULL, NULL);

        bt_build = CreateWindow("button", "OK",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            200, 50, 30, 24,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd
    }

    if(program == 7 || program == 8) {
        text_size_1 = CreateWindow("static", "Size of the matrix A",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            50, 20, 140, 20,
                            hwnd, NULL, NULL, NULL);

        bt_build = CreateWindow("button", "OK",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            160, 50, 30, 24,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd

        combobox_size_12 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            90, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_12, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_12, CB_SETCURSEL, 2, 0);
    }

    if(program == 6) {
        text_size_1 = CreateWindow("static", "Size of the matrix A",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            50, 20, 140, 20,
                            hwnd, NULL, NULL, NULL);

        text_size_2 = CreateWindow("static", "Size of the matrix B",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            433, 20, 140, 20,
                            hwnd, NULL, NULL, NULL);

        bt_build = CreateWindow("button", "OK",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            302, 50, 30, 24,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd

    combobox_size_21 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            433, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_21, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_21, CB_SETCURSEL, 2, 0);

    combobox_size_22 = CreateWindow("combobox", "",
                            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,    // ������ ���� ������ | ������� ������� ���� | ������ ���� ����� ����� ���, ���� ������� �� ������
                            513, 50, 60, 200,
                            hwnd, NULL, NULL, NULL);
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "1");            // ������� ��������� ����������� � ������ ���� ��� ��������� �������
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "2");
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "3");
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "4");
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "5");
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "6");
        SendMessage(combobox_size_22, CB_ADDSTRING, 0, "7");
        SendMessage(combobox_size_22, CB_SETCURSEL, 2, 0);
    }

}

void BuildMatrix(HWND hwnd) {      // ��������� �������

    if(program == 1 || program == 2 || program == 3) {
        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_A[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            20+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }

        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_B[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            334+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }
    }

    else if(program == 4) {
        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_A[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            50+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }
    }

    else if(program == 5) {
        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_A[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            50+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }

        text_size_2 = CreateWindow("static", "number",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            50, 160+30*size_11, 80, 20,
                            hwnd, NULL, NULL, NULL);
        number_for_5 = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            140, 160+30*size_11, 40, 20,
                            hwnd, NULL, NULL, NULL);
    }

    else if(program == 7 || program == 8) {
        for(int i = 0; i < size_12; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_A[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            50+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }
    }

    else {//program == 6
        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_A[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            20+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }

        for(int i = 0; i < size_21; i++) {
            for(int j = 0; j < size_22; j++) {
                matrix_B[i][j] = CreateWindow("edit", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                            334+40*j, 150+30*i, 30, 20,
                            hwnd, NULL, NULL, NULL);
            }
        }
    }

}

void BuildResult(HWND hwnd) {      // ��������� ����������
    if(program == 1) {
        text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            207, 170, 240, 20,
                            hwnd, NULL, NULL, NULL);

        if(the_same) {
            matrix_r[0][0] = CreateWindow("static", "Matrices are equal",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                    238, 200, 180, 20,
                    hwnd, NULL, NULL, NULL);
        } else {
            matrix_r[0][0] = CreateWindow("static", "Matrices are not equal",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                    238, 200, 180, 20,
                    hwnd, NULL, NULL, NULL);

        }
    }

    else if(program == 8) {
        if(matrix_degenerate) {
            text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            227, 170, 180, 20,
                            hwnd, NULL, NULL, NULL);

            matrix_r[0][0] = CreateWindow("static", "The Invertible matrix does not exist",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                    188, 200, 280, 20,
                    hwnd, NULL, NULL, NULL);
        }

        else {
            text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            37, 50, 180, 20,
                            hwnd, NULL, NULL, NULL);

            for(int i = 0; i < size_12; i++) {
                for(int j = 0; j < size_12; j++) {
                    matrix_r[i][j] = CreateWindow("static", "",
                                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                                37+80*j, 80+30*i, 70, 20,
                                hwnd, NULL, NULL, NULL);
                    char s[10] = {0};
                    sprintf(s,"%.3f", matrix_8[i][j]);
                    SetWindowText(matrix_r[i][j], s);


                }
            }
        }
    }

    else if(program == 7) {
        text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            227, 170, 180, 20,
                            hwnd, NULL, NULL, NULL);

        matrix_r[0][0] = CreateWindow("static", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                272, 200, 90, 20,
                hwnd, NULL, NULL, NULL);
        char s[10] = {0};
        sprintf(s,"%d", determinant_7);
        SetWindowText(matrix_r[0][0], s);
    }

    else if(program == 6) {
        text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            37, 50, 180, 20,
                            hwnd, NULL, NULL, NULL);

        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_22; j++) {
                matrix_r[i][j] = CreateWindow("static", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                            37+80*j, 80+30*i, 70, 20,
                            hwnd, NULL, NULL, NULL);
                char s[10] = {0};
                sprintf(s,"%d", matrix[i][j]);
                SetWindowText(matrix_r[i][j], s);


            }
        }
    }

    else if(program == 4) {
        text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            37, 50, 180, 20,
                            hwnd, NULL, NULL, NULL);

        for(int i = 0; i < size_12; i++) {
            for(int j = 0; j < size_11; j++) {
                matrix_r[i][j] = CreateWindow("static", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                            37+80*j, 80+30*i, 70, 20,
                            hwnd, NULL, NULL, NULL);
                char s[10] = {0};
                sprintf(s,"%d", matrix[i][j]);
                SetWindowText(matrix_r[i][j], s);


            }
        }
    }

    else {         //2/3/5
        text_result = CreateWindow("static", "The result of the program:",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            37, 50, 180, 20,
                            hwnd, NULL, NULL, NULL);

        for(int i = 0; i < size_11; i++) {
            for(int j = 0; j < size_12; j++) {
                matrix_r[i][j] = CreateWindow("static", "",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
                            37+80*j, 80+30*i, 70, 20,
                            hwnd, NULL, NULL, NULL);
                char s[10] = {0};
                sprintf(s,"%d", matrix[i][j]);
                SetWindowText(matrix_r[i][j], s);
            }
        }
    }
}

void Save(HWND hwnd) {
    text_save = CreateWindow("static", " If you want to save the file - enter its name\n If the file does not exist, it will be created",
                            WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER,
                            20, 340, 290, 38,
                            hwnd, NULL, NULL, NULL);

    field_save = CreateWindow("edit", "Operations on matrices.txt",
                            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | WS_TABSTOP,
                            20, 390, 220, 20,
                            hwnd, NULL, NULL, NULL);


    bt_save = CreateWindow("button", "Save",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            250, 390, 60, 20,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd

}


// �������� � �����������
LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
    if(message == WM_DESTROY) {            // �������� �������� ���� �������� ����
        game = false;
        PostQuitMessage(0);
    }

    else if(message == WM_COMMAND) {       // ��������� ������

        if(bt_exit == lparam) {                         // ���������� �������� ��� ������ ������
            game = false;
            PostQuitMessage(0);
        }

        if(bt_start == lparam) button_check = 0;

        if(bt_next == lparam) {                         // ��������� �� next
            button_on = true;

            if(button_check == 1 && button_build != 2) {            // �������� �� ������� ��������
                MessageBox(GetActiveWindow(), "Select the size of the matrices and create them", "Warning!", MB_ICONASTERISK|MB_OK);
                button_on = false;
            }

            else if(button_check == 1 && button_build == 2) {       // �������� ������� ����� �� ����������� �����
                if(program == 7 || program == 8) {
                    for(int i = 0; i < size_12; i++) {                   // ���� ���� ������� �
                        for(int j = 0; j < size_12; j++) {
                            char a[5] = {0};
                            if(GetWindowText(matrix_A[i][j], a, 5) == 0) {
                                    MessageBox(GetActiveWindow(), "Fill in all the cells", "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                            }
                        }
                        if(!button_on) break;
                    }

                    if(button_on) {                                      // ���������� ����� ������� �
                        for(int i = 0; i < size_12; i++) {
                            for(int j = 0; j < size_12; j++) {
                                char c[4] = {'\0'};
                                GetWindowText(matrix_A[i][j], c, 4);
                                if(Input_check_Z(c) == false) {
                                    char buf[40] = {'\0'};
                                    sprintf(buf, "Input error in element (%d;%d)", i+1, j+1);
                                    MessageBox(GetActiveWindow(), buf, "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                                }
                                else matrixA[i][j] = atoi(c);
                            }
                            if(!button_on) break;
                        }
                    }
                }

                else if(program == 6) {
                    for(int i = 0; i < size_11; i++) {                   // ���� ���� ������� �
                        for(int j = 0; j < size_12; j++) {
                            char a[5] = {0};
                            if(GetWindowText(matrix_A[i][j], a, 5) == 0) {
                                    MessageBox(GetActiveWindow(), "Fill in all the cells", "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                            }
                        }
                        if(!button_on) break;
                    }

                    if(button_on) {                                      // ���� ���� ������� �
                        for(int i = 0; i < size_21; i++) {
                            for(int j = 0; j < size_22; j++) {
                                char b[5] = {0};
                                if(GetWindowText(matrix_B[i][j], b, 5) == 0) {
                                        MessageBox(GetActiveWindow(), "Fill in all the cells", "Warning!", MB_ICONASTERISK|MB_OK);
                                        button_on = false;
                                        break;
                                }
                            }
                            if(!button_on) break;
                        }
                    }

                    if(button_on) {                                      // ���������� ����� ������� �
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_12; j++) {
                                char c[4] = {'\0'};
                                GetWindowText(matrix_A[i][j], c, 4);
                                if(Input_check_Z(c) == false) {
                                    char buf[40] = {'\0'};
                                    sprintf(buf, "Input error in element A(%d;%d)", i+1, j+1);
                                    MessageBox(GetActiveWindow(), buf, "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                                }
                                else matrixA[i][j] = atoi(c);
                            }
                            if(!button_on) break;
                        }
                    }

                    if(button_on) {                                      // ���������� ����� ������� �
                        for(int i = 0; i < size_21; i++) {
                            for(int j = 0; j < size_22; j++) {
                                char c[4] = {'\0'};
                                GetWindowText(matrix_B[i][j], c, 4);
                                if(Input_check_Z(c) == false) {
                                    char buf[40] = {'\0'};
                                    sprintf(buf, "Input error in element B(%d;%d)", i+1, j+1);
                                    MessageBox(GetActiveWindow(), buf, "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                                }
                                else matrixB[i][j] = atoi(c);
                            }
                            if(!button_on) break;
                        }
                    }
                }

                else {//program == 1/2/3/4/5
                    for(int i = 0; i < size_11; i++) {                   // ���� ���� ������� � � � �� ���� ����� ����� (5)
                        for(int j = 0; j < size_12; j++) {
                            char a[5] = {0};
                            char b[5] = {0};
                            char c[5] = {0};

                            if((GetWindowText(matrix_A[i][j], a, 5) == 0) ||
                                ((program == 1 || program == 2 || program == 3) && GetWindowText(matrix_B[i][j], b, 5) == 0) ||
                                (program == 5 && GetWindowText(number_for_5, c, 5) == 0)) {
                                    MessageBox(GetActiveWindow(), "Fill in all the cells", "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                            }
                        }
                        if(!button_on) break;
                    }

                    if(button_on) {                                      // ���������� ����� ������� �
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_12; j++) {
                                char c[4] = {'\0'};
                                GetWindowText(matrix_A[i][j], c, 4);
                                if(Input_check_Z(c) == false) {
                                    char buf[40] = {'\0'};
                                    sprintf(buf, "Input error in element A(%d;%d)", i+1, j+1);
                                    MessageBox(GetActiveWindow(), buf, "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                                }
                                else matrixA[i][j] = atoi(c);
                            }
                            if(!button_on) break;
                        }
                    }

                    if(button_on && program != 4 && program != 5) {      // ���������� ����� ������� � (1/2/3)
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_12; j++) {
                                char c[4] = {'\0'};
                                GetWindowText(matrix_B[i][j], c, 4);
                                if(Input_check_Z(c) == false) {
                                    char buf[40] = {'\0'};
                                    sprintf(buf, "Input error in element B(%d;%d)", i+1, j+1);
                                    MessageBox(GetActiveWindow(), buf, "Warning!", MB_ICONASTERISK|MB_OK);
                                    button_on = false;
                                    break;
                                }
                                else matrixB[i][j] = atoi(c);
                            }
                            if(!button_on) break;
                        }
                    }

                    if(button_on && program == 5) {                      // ���������� ����� ����� (5)
                            char c[4] = {'\0'};
                            GetWindowText(number_for_5, c, 4);
                            if(Input_check_Z(c) == false) {
                                MessageBox(GetActiveWindow(), "Input error in the number", "Warning!", MB_ICONASTERISK|MB_OK);
                                button_on = false;
                            }
                            else number_5 = atoi(c);
                    }
                }
            }

            if(button_on) {                                         // ���� ��� �����, �� ������ ����� �����������
                button_check++;
            }
        }

        if(bt_build == lparam) {                        // ������ �������� ������� ���������
            button_build = 1;

            if(program == 6 && size_12 != size_21) {               // �������� ��� �������� �������
                MessageBox(GetActiveWindow(),
                "The number of columns in the first matrix must be equal to the number of rows in the second matrix!",
                "Warning!", MB_ICONASTERISK|MB_OK);
                button_build = 0;
            }
        }

        if(bt_save == lparam) {                         // ������ ���������� �����
            FILE *file;
            if(GetWindowText(field_save, file_name, 30) == 0)
                MessageBox(GetActiveWindow(), "Filename is empty", "Warning!", MB_ICONASTERISK|MB_OK);
            else {
                file = fopen(file_name, "a");
                switch(program) {
                    case 1:
                        fprintf(file,"Operation: \"Comparison of matrices\"\n\n");
                        break;

                    case 2:
                        fprintf(file,"Operation: \"The sum of matrices\"\n\n");
                        break;

                    case 3:
                        fprintf(file,"Operation: \"The difference of matrices\"\n\n");
                        break;

                    case 4:
                        fprintf(file,"Operation: \"Transposed matrix\"\n\n");
                        break;

                    case 5:
                        fprintf(file,"Operation: \"Multiplying the matrix by a number\"\n\n");
                        break;

                    case 6:
                        fprintf(file,"Operation: \"Multiplication of matrices\"\n\n");
                        break;

                    case 7:
                        fprintf(file,"Operation: \"Determinant of the matrix\"\n\n");
                        break;

                    case 8:
                        fprintf(file,"Operation: \"Invertible matrix\"\n\n");
                        break;
                }

                if(program == 1 || program == 2 || program == 3 || program == 4 || program == 5 || program == 6) {
                    fprintf(file,"Matrix A:\n");
                    for(int i = 0; i < size_11; i++) {  // ������� � ��� (1-6)
                        for(int j = 0; j < size_12; j++) {
                            fprintf(file,"%4d ", matrixA[i][j]);
                        }
                        fprintf(file,"\n");
                    }

                    if(program == 6) {                  // ������� � ��� (6)
                        fprintf(file,"\nMatrix B:\n");
                        for(int i = 0; i < size_21; i++) {
                            for(int j = 0; j < size_21; j++) {
                                fprintf(file,"%4d ", matrixB[i][j]);
                            }
                            fprintf(file,"\n");
                        }
                    }

                    else if(program == 5) {             // ����� ��� (5)
                        fprintf(file,"\nNumber: %d\n", number_5);
                    }

                    else if(program != 4) {             // ������� � ��� (1-3)
                        fprintf(file,"\nMatrix B:\n");
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_12; j++) {
                                fprintf(file,"%4d ", matrixB[i][j]);
                            }
                            fprintf(file,"\n");
                        }
                    }

                    fprintf(file,"\nResult:\n");

                    if(program == 1) {                                        // ��������� (1)
                        if(the_same) fprintf(file,"Matrices are equal\n");
                        else fprintf(file,"Matrices are not equal\n");
                    }

                    else if(program == 2 || program == 3 || program == 5) {   // ��������� (2/3/5)
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_12; j++) {
                                fprintf(file,"%4d ", matrix[i][j]);
                            }
                            fprintf(file,"\n");
                        }
                    }

                    else if(program == 4) {                                   // ��������� (4)
                        for(int i = 0; i < size_12; i++) {
                            for(int j = 0; j < size_11; j++) {
                                fprintf(file,"%4d ", matrix[i][j]);
                            }
                            fprintf(file,"\n");
                        }
                    }

                    else {                                                    // ��������� (6)
                        for(int i = 0; i < size_11; i++) {
                            for(int j = 0; j < size_22; j++) {
                                fprintf(file,"%4d ", matrix[i][j]);
                            }
                            fprintf(file,"\n");
                        }
                    }
                }

                else { //program == 7 || program == 8
                    fprintf(file,"Matrix A:\n");
                    for(int i = 0; i < size_12; i++) {  // ������� � ��� (7/8)
                        for(int j = 0; j < size_12; j++) {
                            fprintf(file,"%4d ", matrixA[i][j]);
                        }
                        fprintf(file,"\n");
                    }

                    fprintf(file,"\nResult:\n");

                    if(program == 7) {
                        fprintf(file,"Determinant of matrix: %d\n", determinant_7);
                    }

                    else {//program == 8
                        if(matrix_degenerate) fprintf(file,"The Invertible matrix does not exist\n");

                        else {
                            for(int i = 0; i < size_12; i++) {
                                for(int j = 0; j < size_12; j++) {
                                    fprintf(file,"%6.3f ", matrix_8[i][j]);
                                }
                                fprintf(file,"\n");
                            }
                            fprintf(file,"\n");
                        }
                    }
                }

                fprintf(file, "---------------------------------------------------\n\n");
                fclose(file);
            }
        }

        if(combobox_matrix == lparam) {                 // ���� ������ �� ��������� � ���������
            if(HIWORD(wparam) == CBN_SELCHANGE) {                  // ������� �����, �� ���� ������
                int id = SendMessage(lparam, CB_GETCURSEL, 0, 0);
                program = id+1;
            }
        }

        // ������ �������
        if(combobox_size_11 == lparam) if(HIWORD(wparam) == CBN_SELCHANGE) size_11 = SendMessage(lparam, CB_GETCURSEL, 0, 0) + 1;
        if(combobox_size_12 == lparam) if(HIWORD(wparam) == CBN_SELCHANGE) size_12 = SendMessage(lparam, CB_GETCURSEL, 0, 0) + 1;
        if(combobox_size_21 == lparam) if(HIWORD(wparam) == CBN_SELCHANGE) size_21 = SendMessage(lparam, CB_GETCURSEL, 0, 0) + 1;
        if(combobox_size_22 == lparam) if(HIWORD(wparam) == CBN_SELCHANGE) size_22 = SendMessage(lparam, CB_GETCURSEL, 0, 0) + 1;

        }

    else return DefWindowProcA(hwnd, message, wparam, lparam);
}


// ���� ��������
int main() {
    WNDCLASS wcl;
        memset(&wcl, 0, sizeof(WNDCLASS)); // ���������� �񳺿 ���'�� ������
        wcl.lpszClassName = "my Window";   // ���� ����
        wcl.lpfnWndProc = WndProc;         // ������� ����������
    RegisterClassA(&wcl);                  // ��������� ����� ����


    HWND hwnd;
    hwnd = CreateWindow("my Window", "Operations on matrices", WS_OVERLAPPEDWINDOW, 10, 10, 640, 480, NULL, NULL, NULL, NULL);
    //                                              ����� ����, ���� ������ ��� ����(10,10), �����(640, 480)

    HDC dc = GetDC(hwnd);                  // ������� ��� ����������� ������
    HDC snail_body = GetDC(hwnd);          // ҳ�� �������
    HDC snail_circle = GetDC(hwnd);        // ������� �������
    HDC snail_black = GetDC(hwnd);         // ���� �������
    HDC bg = GetDC(hwnd);                  // ���

    ShowWindow(hwnd, SW_SHOWNORMAL);       // ����� ����
    Background(bg);

    HWND text_start = CreateWindow("static", "\nTerm Paper\n\n\"OPERATIONS ON MATRICES\"\n\n\nCompleted by a student of the KM-01 group\nLyubomyr Sholop\n\n\n2021",
                            WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER,
                            177, 60, 290, 185,
                            hwnd, NULL, NULL, NULL);

    bt_start = CreateWindow("button", "START",
                            WS_VISIBLE | WS_CHILD,    // ������ ���� ������ | ������� ������� ����
                            275, 260, 100, 40,
                            hwnd, NULL, NULL, NULL);  // ������� ������ - hwnd

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) && button_check == -1) {
        DispatchMessage(&msg);                 // ������ ����������� � ���� ����
    }
    DestroyWindow(text_start);
    DestroyWindow(bt_start);


    do {
    // ������ ����
    button_build = 0;
    program = 1;
    size_11 = 3;
    size_12 = 3;
    size_21 = 3;
    size_22 = 3;
    game = true;
    button_check = 0;

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(58, 240, 94));
    Rectangle(dc, -1, 140, 640, 480);

    FillMas();

    ButtonExitNext(hwnd);                         // ��������� ������  Exit i Next
    HomeScreen(hwnd);                             // ��������� ������ ��� ���� �������� � ���� ��������

    while(game) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);             // ������ ����������� � ���� ����
        }
        else if(button_check == 0) {
            WinMove();                          // ��� �������
            WinShow(dc, snail_body, snail_circle, snail_black);
            Sleep(50);                          // ��������
        }
        else break;
    }

    // ��������� ����
    DestroyWindow(combobox_matrix);   // ��������� ������ ��������
    DestroyWindow(text_welcome);
    DestroyWindow(bt_exit);           // ��������� Exit
    DestroyWindow(bt_next);           // ��������� Next

    Background(bg);                   // ������� ������ ����
    ButtonExitNext(hwnd);             // ��������� Next i Exit

    EnterMatrixSize(hwnd);            // ���� ������ �������

    while(GetMessage(&msg, NULL, 0, 0) && game) {
        if(!IsDialogMessage(hwnd, &msg)){
            DispatchMessage(&msg);                 // ������ ����������� � ���� ����
        }
        if(button_build == 1) {
            DestroyWindow(bt_build);           // ������� ������
            DestroyWindow(combobox_size_11);   // �������� ���� ������
            DestroyWindow(combobox_size_12);   // -||-||-
            DestroyWindow(combobox_size_21);   // -||-||-
            DestroyWindow(combobox_size_22);   // -||-||-
            BuildMatrix(hwnd);                 // ����� ������
            button_build = 2;                  // ����� ��� ������ �� ���������
            SetFocus(matrix_A[0][0]);
        }
        if(button_check == 2) {
            int matrix_7[size_12][size_12];                                     // ����������� ������� ��� ������ �����������
            for(int i = 0; i < size_12; i++) {
                for(int j = 0; j < size_12; j++) matrix_7[i][j] = matrixA[i][j];
            }

            switch(program) {
            case 1:
                the_same = Compare_matrix(size_11, size_12, matrixA, matrixB);
                break;

            case 2:
                Matrix_sum(size_11, size_12, matrixA, matrixB, matrix);
                break;

            case 3:
                Matrix_difference(size_11, size_12, matrixA, matrixB, matrix);
                break;

            case 4:
                Matrix_transpose(size_11, size_12, matrixA, matrix);
                break;

            case 5:
                Matrix_multiplication_number(size_11, size_12, matrixA, number_5, matrix);
                break;

            case 6:
                Matrix_multiplication_matrix(size_11, size_12, size_22, matrixA, matrixB, matrix);
                break;

            case 7:
                determinant_7 = Matrix_determinant(size_12, matrix_7, determinant_7);
                break;

            case 8:
                matrix_degenerate = Invertible_matrix(size_12, matrixA, matrix_8);
                break;
            }
            break;
        }
    }

    // ��������� ���������� �����
    DestroyWindow(bt_exit);           // ��������� Exit
    DestroyWindow(bt_next);           // ��������� Next
    for(int i = 0; i < 7; i++) for(int j = 0; j < 7; j++) DestroyWindow(matrix_A[i][j]);
    for(int i = 0; i < 7; i++) for(int j = 0; j < 7; j++) DestroyWindow(matrix_B[i][j]);
    DestroyWindow(text_size_1);
    DestroyWindow(text_size_2);

    // ��������� ������
    Background(bg);
    ButtonExitNext(hwnd);
    BuildResult(hwnd);
    Save(hwnd);

    while(GetMessage(&msg, NULL, 0, 0) && game && button_check != 3) {
        if(!IsDialogMessage(hwnd, &msg)){
            DispatchMessage(&msg);                 // ������ ����������� � ���� ����
        }
    }
    DestroyWindow(bt_exit);           // ��������� Exit
    DestroyWindow(bt_next);           // ��������� Next
    DestroyWindow(text_result);
    DestroyWindow(bt_save);
    DestroyWindow(text_save);
    DestroyWindow(field_save);

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 7; j++) DestroyWindow(matrix_r[i][j]);
    }

    if(snail_after_restart < 600) snail_after_restart = snail_after_restart + 50;
    else snail_after_restart = 0;

    } while(button_check == 3);

    return 0;
}

