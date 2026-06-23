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
    cout << YELLOW << "4." << RESET << " Cap nhat thi sinh\n";
    cout << YELLOW << "5." << RESET << " Xoa thi sinh\n";
    cout << YELLOW << "6." << RESET << " Luu du lieu\n";
    cout << YELLOW << "7." << RESET << " Phan phong thi (To uu chu cai dau)\n";
    cout << YELLOW << "8." << RESET << " Thong ke gioi tinh\n"; 
    cout << RED << "0." << RESET << " Thoat\n";
    cout << CYAN << "=====================================\n" << RESET;
    cout << "Lua chon: ";
}

int main() {

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

            c.examId = generateExamId(table);

            cout << "Ma du thi tu dong: "
                 << c.examId << endl;

            cout << "Ho ten: ";
            getline(cin, c.fullName);

            cout << "Gioi tinh: ";
            getline(cin, c.gender);

            cout << "Ngay sinh: ";
            getline(cin, c.birthDate);

            cout << "Que quan: ";
            getline(cin, c.hometown);

            insertCandidate(table, c);

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

            string examId;

            cout << "Nhap ma du thi can cap nhat: ";
            getline(cin, examId);

            if (updateCandidate(table, examId))
                cout << GREEN << "Cap nhat thanh cong!\n" << RESET;
            else
                cout << RED << "Khong tim thay!\n" << RESET;

            break;
        }

        case 5: {

            string examId;

            cout << "Nhap ma du thi can xoa: ";
            getline(cin, examId);

            if (deleteCandidate(table, examId))
                cout << GREEN << "Xoa thanh cong!\n" << RESET;
            else
                cout << RED << "Khong tim thay!\n" << RESET;

            break;
        }

        case 6:

            saveToFile(table, "data.txt");

            cout << GREEN << "Luu thanh cong!\n" << RESET;

            break;

        case 7: {

            int numRooms;
            cout << "Nhap so luong phong thi: ";
            cin >> numRooms;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (numRooms > 0)
                distributeToRooms(table, numRooms);
            else
                cout << RED << "So luong phong phai lon hon 0!\n" << RESET;

            break;
        }
        case 8: {

            int numRooms;

            cout << "Nhap so luong phong thi: ";
            cin >> numRooms;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

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