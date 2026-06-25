#ifdef _WIN32
#include <windows.h>
#endif // Vì việc chạy các lệnh in màu trên máy của PPThinh510 không hiển thị
//Một cách bình thường nên cần các thư viện và hàm đặc thù riêng

#include <iostream>
#include <limits>

#include "hashtable.h"
#include "file.h"
#include "color.h"

using namespace std;

void menu() {

    cout << BOLD << CYAN << "\n=====================================\n";
    cout << "      QUAN LY DANH SACH THI SINH     \n";
    cout << "       Hash Table + Linked List      \n";
    cout << "=====================================\n" << RESET;
    cout << YELLOW << "1." << RESET << " Them thi sinh\n";
    cout << YELLOW << "2." << RESET << " Hien thi danh sach\n";
    cout << YELLOW << "3." << RESET << " Tim kiem theo ma du thi\n";
    cout << YELLOW << "4." << RESET << " Tim kiem theo ho ten\n";
    cout << YELLOW << "5." << RESET << " Tim kiem theo que quan\n";
    cout << YELLOW << "6." << RESET << " Cap nhat thi sinh\n";
    cout << YELLOW << "7." << RESET << " Xoa thi sinh\n";
    cout << YELLOW << "8." << RESET << " Luu du lieu\n";
    cout << YELLOW << "9." << RESET << " Phan phong thi\n";
    cout << YELLOW << "10." << RESET << " Thong ke gioi tinh\n"; 
    cout << RED << "0." << RESET << " Thoat\n";
    cout << CYAN << "=====================================\n" << RESET;
    cout << "Lua chon: ";
}

int main() {
#ifdef _WIN32// Hàm đặc thù hỗ trợ biên dịch cho máy của PPThinh510
    // Kich hoat ho tro ma mau ANSI tren Console Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    HashTable table;
    initTable(table);
    loadFromFile(table, "data.txt");
    int choice;
    do {
        menu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        case 1: {
            Candidate c;
            char autoChoice;
            cout << "Tu dong sinh ma du thi? (y/n): ";
            cin >> autoChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (tolower(autoChoice) == 'y') {
                c.examId = generateExamId(table);
                cout << "Ma du thi tu dong: " << c.examId << endl;
            } else {
                do {
                    cout << "Nhap ma du thi (vd: TS001): ";
                    getline(cin, c.examId);
                    c.examId = normalizeExamId(c.examId);
                    if (c.examId.empty()) {
                        cout << RED << "Ma du thi khong duoc de trong! Vui long nhap lai.\n" << RESET;
                    } else if (findCandidate(table, c.examId) != nullptr) {
                        cout << RED << "Ma du thi da ton tai! Vui long nhap lai.\n" << RESET;
                    } else {
                        break;
                    }
                } while (true);
            }
            cout << "Ho ten: ";
            string nameInput;
            getline(cin, nameInput);
            c.fullName = normalizeName(nameInput);
            string genderInput;
            do {
                cout << "Gioi tinh (Nam/Nu): ";
                getline(cin, genderInput);
                if (isValidGender(genderInput)) {
                    c.gender = genderInput;
                    break;
                }
                cout << RED << "Gioi tinh khong hop le! Vui long nhap lai (Nam/Nu).\n" << RESET;
            } while (true);
            string dateInput;
            do {
                cout << "Ngay sinh (DD/MM/YYYY): ";
                getline(cin, dateInput);
                if (isValidDate(dateInput)) {
                    c.birthDate = dateInput;
                    break;
                }
                cout << RED << "Ngay sinh khong hop le! Vui long nhap lai dung dinh dang DD/MM/YYYY.\n" << RESET;
            } while (true);
            cout << "Que quan: ";
            getline(cin, c.hometown);
            c.hometown = normalizeName(c.hometown);
            insertCandidate(table, c);
            cout << GREEN << "Them thi sinh thanh cong!\n" << RESET;
            break;
        }
        case 2:
            displayAll(table);
            break;
        case 3: {
            string examId;
            cout << "Nhap ma du thi: ";
            getline(cin, examId);
            Node* result = findCandidate(table, examId);
            if (result == nullptr) {
                cout << RED << "Khong tim thay!\n" << RESET;
            }
            else {
                cout << YELLOW << "\nMa du thi : " << RESET << result->data.examId;
                cout << "\nHo ten    : " << result->data.fullName;
                cout << "\nGioi tinh : " << result->data.gender;
                cout << "\nNgay sinh : " << result->data.birthDate;
                cout << "\nQue quan  : " << result->data.hometown << endl;
            }
            break;
        }
        case 4: {
            string fullName;
            cout << "Nhap ho ten can tim: ";
            getline(cin, fullName);
            searchByFullName(table, fullName);
            break;
        }
        case 5: {
            string hometown;
            cout << "Nhap que quan can tim: ";
            getline(cin, hometown);
            searchByHometown(table, hometown);
            break;
        }
        case 6: {
            string examId;
            cout << "Nhap ma du thi can cap nhat: ";
            getline(cin, examId);
            if (updateCandidate(table, examId))
                cout << GREEN << "Cap nhat thanh cong!\n" << RESET;
            else
                cout << RED << "Khong tim thay!\n" << RESET;
            break;
        }
        case 7: {
            string examId;
            cout << "Nhap ma du thi can xoa: ";
            getline(cin, examId);
            if (deleteCandidate(table, examId))
                cout << GREEN << "Xoa thanh cong!\n" << RESET;
            else
                cout << RED << "Khong tim thay!\n" << RESET;

            break;
        }
        case 8:
            saveToFile(table, "data.txt");
            cout << GREEN << "Luu thanh cong!\n" << RESET;
            break;
        case 9: {
            int numRooms;
            cout << "Nhap so luong phong thi: ";
            cin >> numRooms;
            cin.ignore();
            if (numRooms > 0)
                distributeToRooms(table, numRooms);
            else
                cout << RED << "So luong phong phai lon hon 0!\n" << RESET;
            break;
        }
        case 10: {
            int numRooms;
            cout << "Nhap so luong phong thi: ";
            cin >> numRooms;
            cin.ignore();
            thongKeGioiTinh(table, numRooms);
            break;
        }
        case 0:
            saveToFile(table, "data.txt");
            cout << MAGENTA << BOLD << "Tam biet!\n" << RESET;
            break;
        default:
            cout << RED << "Lua chon khong hop le!\n" << RESET;
        }
    } while (choice != 0);
    return 0;
}