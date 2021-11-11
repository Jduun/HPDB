#include <iostream>
#include <cstring>
#include <iomanip>

#define WRONG_COMMAND cout << "Такой команды не существует\n";

using namespace std;

const int MAX_COUNT_STUDENTS = 100;
const int COUNT_SUBJECTS = 5;
const int MAX_LEN_NAME = 50;

struct Student
{
    char name[MAX_LEN_NAME] = "No name";
    int course = 1;
    int admission_year = 2021;
    int marks[COUNT_SUBJECTS]{};
    bool is_db = false;
};

enum Commands
{
    PRINT,
    ADD,
    CHANGE,
    DELETE,
    SORT,
    REQUEST,
    EXIT
};

void menu();
void add(Student*);
void print(Student*);
void request(Student*);
void del(Student*);
void change(Student*);

int main()
{
    setlocale(LC_ALL, "Ru");
    Student db[MAX_COUNT_STUDENTS];
    bool exit = false;
    while (!exit)
    {
        menu();
        int command;
        cout << "Введите команду: ";
        cin >> command;
        system("cls");
        switch (command)
        {
        case PRINT:
            print(db);
            break;
        case ADD:
            add(db);
            break;
        case CHANGE:
            cout << "CHANGE\n\n";
            break;
        case DELETE:
            cout << "DELETE\n\n";
            break;
        case SORT:
            cout << "SORT\n\n";
            break;
        case REQUEST:
            request(db);
            break;
        case EXIT:
            exit = true;
            break;
        default:
            WRONG_COMMAND;
            break;
        }
    }
    return 0;
}

void menu()
{
    cout << "0. Вывести базу данных\n"
        << "1. Добавление записи\n"
        << "2. Изменение записи\n"
        << "3. Удаление записи\n"
        << "4. Сортировку массива\n"
        << "5. Запрос\n"
        << "6. Выход\n";
}

void add(Student* db)
{
    for (int i = 0; i < MAX_COUNT_STUDENTS; i++)
    {
        if (!db[i].is_db)
        {
            cout << "Добавить студента по умолчанию?(Да - 1, нет - 0)\n";
            int ans;
            cin >> ans;
            cin.ignore();
            if (ans == 0 || ans == 1)
            {
                db[i].is_db = true;
                if (ans == 0)
                {
                    cout << "Введите информацию о новом студенте:\n";
                    char str[MAX_LEN_NAME] = "";
                    cout << "Введите имя: ";
                    cin.getline(db[i].name, MAX_LEN_NAME);
                    cout << "\nname=" << db[i].name << '\n';
                    cout << "Введите курс: ";
                    cin >> db[i].course;
                    cout << "Введите год поступления: ";
                    cin >> db[i].admission_year;
                    cout << "Введите оценки за:\n";
                    for (int j = 0; j < COUNT_SUBJECTS; j++)
                    {
                        cout << j + 1 << " предмет: ";
                        cin >> db[i].marks[j];
                    }
                }
            }
            else WRONG_COMMAND;
            break;
        }
    }
}

void print(Student* db)
{
    const int SIZE_CELL = 5;
    cout << '+';
    for (int i = 0; i < SIZE_CELL * 8 + MAX_LEN_NAME + 8; i++) cout << '-';
    cout << "+\n";
    for (int i = 0; i < MAX_COUNT_STUDENTS; i++)
    {
        if (db[i].is_db)
        {
            cout << '|' << setw(SIZE_CELL) << i << '|';
            cout << setw(MAX_LEN_NAME) << db[i].name;
            cout << '|' << setw(SIZE_CELL) << db[i].course << '|' << setw(SIZE_CELL) << db[i].admission_year << '|';
            for (int j = 0; j < COUNT_SUBJECTS; j++) cout << setw(SIZE_CELL) << db[i].marks[j] << '|';
            bool last_student = (i + 1 == MAX_COUNT_STUDENTS || !db[i + 1].is_db);
            cout << (last_student ? "\n+" : "\n|");
            for (int j = 0; j < SIZE_CELL; j++) cout << '-';
            cout << (last_student ? '-' : '+');
            for (int j = 0; j < MAX_LEN_NAME; j++) cout << '-';
            for (int j = 0; j < 2 + COUNT_SUBJECTS; j++)
            {
                cout << (last_student ? '-' : '+');
                for (int k = 0; k < SIZE_CELL; k++) cout << '-';
            }
            cout << (last_student ? "+\n" : "|\n");
        }
    }
}

void request(Student* db)
{
    int count_students = 0;
    for (int i = 0; i < MAX_COUNT_STUDENTS; i++)
    {
        if (db[i].is_db)
        {
            const int MARK = 2;
            int count_MARK = 0;
            for (int j = 0; j < COUNT_SUBJECTS; j++)
            {
                if (db[i].marks[j] == MARK) count_MARK++;
            }
            if (count_MARK >= 3)
            {
                count_students++;
                cout << count_students << ") " << db[i].name << '\n';
            }
        }
    }
}

void del(Student* db)
{

}

void change(Student* db)
{

}