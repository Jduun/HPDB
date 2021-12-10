﻿#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>

#define ERROR_INPUT cout << "Неверный ввод!\n"
#define MAX_COUNT_STUDENTS 100
#define COUNT_SUBJECTS 5
#define MAX_LEN_NAME 50
#define IGNORE 32767

using namespace std;

struct Student
{
    char name[MAX_LEN_NAME] = "No name";
    char course[2] = "1";
    char admission_year[5] = "2021";
    int marks[COUNT_SUBJECTS]{2, 2, 2, 2, 2};
};

enum MainCommands
{
    PRINT,
    ADD,
    CHANGE,
    DELETE,
    SORT,
    REQUEST,
    LOAD_IN_TEXT_FILE,
    UNLOAD_FROM_TEXT_FILE,
    LOAD_IN_BIN_FILE,
    UNLOAD_FROM_BIN_FILE,
    EXIT
};

enum ChangeCommands
{
    CHANGE_NAME,
    CHANGE_COURSE,
    CHANGE_ADMISSION_YEAR,
    CHANGE_MARKS,
};

enum SortCommands
{
    SORT_NAME,
    SORT_COURSE,
    SORT_ADMISSION_YEAR,
    SORT_MARKS
};

void menu();
void add(Student*, int&);
void print(Student*, int&);
void request(Student*, int&);
void del(Student*, int&);
void change(Student*, int&);
void sort(Student*, int&);
void load_in_file(Student*, int&, const char*);
void unload_from_file(Student*, int&, const char*);

void input_name(Student*);
void input_course(Student*);
void input_admission_year(Student*);
void input_marks(Student*);

bool compare(char*, char*);
bool is_letter(char);
bool is_num(char);
void swap(Student*, Student*);
int max(int, int);
void get_words(char* text, const char** words, int& count);

int main()
{
    setlocale(LC_ALL, "Ru");
    Student db[MAX_COUNT_STUDENTS];
    int count_students = 0;
    bool exit = false;
    while (!exit)
    {
        menu();
        bool correct = false;
        while (!correct)
        {
            char input[3] = "";
            correct = true;
            cout << "Введите команду: ";
            cin.getline(input, 3);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(IGNORE, '\n');
                correct = false;
            }
            else
            {
                for (int i = 0; i < strlen(input); i++)
                {
                    if (!is_num(input[i]))
                    {
                        correct = false;
                        break;
                    }
                }
            }

            if (correct)
            {
                system("cls");
                switch (atoi(input))
                {
                case PRINT:
                    print(db, count_students);
                    break;
                case ADD:
                    add(db, count_students);
                    break;
                case CHANGE:
                    change(db, count_students);
                    break;
                case DELETE:
                    del(db, count_students);
                    break;
                case SORT:
                    sort(db, count_students);
                    break;
                case REQUEST:
                    request(db, count_students);
                    break;
                case LOAD_IN_TEXT_FILE:
                    load_in_file(db, count_students, "data_base.txt");
                    break;
                case UNLOAD_FROM_TEXT_FILE:
                    unload_from_file(db, count_students, "data_base.txt");
                    break;
                case LOAD_IN_BIN_FILE:
                    load_in_file(db, count_students, "data_base.bin");
                    break;
                case UNLOAD_FROM_BIN_FILE:
                    unload_from_file(db, count_students, "data_base.bin");
                    break;
                case EXIT:
                    exit = true;
                    break;
                default:
                    menu();
                    ERROR_INPUT;
                    correct = false;
                    break;
                }
            }
            else
            {
                system("cls");
                menu();
                ERROR_INPUT;
            }
        }
    }
    return 0;
}

void menu()
{
    cout << PRINT << ". Вывести базу данных\n"
         << ADD << ". Добавление записи\n"
         << CHANGE << ". Изменение записи\n"
         << DELETE << ". Удаление записи\n"
         << SORT << ". Сортировка базы данных\n"
         << REQUEST << ". Запрос\n"
         << LOAD_IN_TEXT_FILE << ". Загрузка базы данных в текстовый файл\n"
         << UNLOAD_FROM_TEXT_FILE << ". Выгрузка базы данных из текстового файла\n"
         << LOAD_IN_BIN_FILE << ". Загрузка базы данных в бинарный файл\n"
         << UNLOAD_FROM_BIN_FILE << ". Выгрузка из бинарного файла\n"
         << EXIT << ". Выход\n";
}

void add(Student* db, int& count_students)
{
    if (count_students == MAX_COUNT_STUDENTS)
    {
        cout << "Больше нельзя добавить студентов\n\n";
        return;
    }
	bool correct = false;
	char input[2] = "";
	while (!correct)
	{
		correct = true;
		cout << "Добавить студента по умолчанию?(Да - 1, нет - 0)\n";
		cin.getline(input, 2);
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(IGNORE, '\n');
			correct = false;
		}
		else correct = (input[0] == '0' || input[0] == '1');

		if (correct)
		{
			if (input[0] == '0')
			{
				cout << "Введите информацию о новом студенте:\n";
				input_name(&db[count_students]);
				input_course(&db[count_students]);
				input_admission_year(&db[count_students]);
				input_marks(&db[count_students]);
			}
		}
		else ERROR_INPUT;
	}
    count_students++;
    system("cls");
    print(db, count_students);
}

void print(Student* db, int& count_students)
{
    if (count_students == 0)
    {
        cout << "Студентов еще нет в базе данных\n\n";
        return;
    }
    const int COUNT_FIELDS = 9;
    const char* titles[COUNT_FIELDS]{ "№", "Имя", "Курс", "Год поступления", "Мат. анализ", "Анал. геом.", "Инж. граф.", "Высок. прогр.", "Теор. инф." };
    int size_field[COUNT_FIELDS]{};
    for (int i = 0; i < COUNT_FIELDS; i++) size_field[i] = strlen(titles[i]);
    int count_digits = 0;
    int n = count_students;
    while (n > 0)
    {
        n /= 10;
        count_digits++;
    }
    size_field[0] = max(size_field[0], count_digits);

    for (int i = 0; i < count_students; i++)
    {
        size_field[1] = max(size_field[1], strlen(db[i].name));
        size_field[2] = max(size_field[2], strlen(db[i].course));
        size_field[3] = max(size_field[3], strlen(db[i].admission_year));
        for (int j = 0; j < COUNT_SUBJECTS; j++) size_field[j + 4] = max(size_field[j + 4], 1);
    }
    int width = COUNT_FIELDS - 1;
    for (int i = 0; i < COUNT_FIELDS; i++) width += size_field[i];
    cout << "+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n";
    for (int i = 0; i < COUNT_FIELDS; i++) cout << "|" << setw(size_field[i]) << titles[i];
    cout << "|\n";

    for (int i = 0; i < count_students; i++)
    {
        cout << "|";
        int index_field = 0;
        int curr_index = 0;
        for (int i = 0; i < width; i++)
        {
            curr_index++;
            if (curr_index == size_field[index_field] + 1)
            {
                cout << "+";
                index_field++;
                curr_index = 0;
            }
            else cout << "-";
        }
        cout << "|\n";

        cout << "|";
        cout << setw(size_field[0]) << i << "|" << setw(size_field[1]) << db[i].name << "|" << setw(size_field[2]) << db[i].course << "|"
            << setw(size_field[3]) << db[i].admission_year << "|";
        for (int j = 0; j < COUNT_SUBJECTS; j++) cout << setw(size_field[j + 4]) << db[i].marks[j] << "|";
        cout << "\n";
    }

    cout << "+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n\n";
}

void request(Student* db, int& count_students)
{
    if (count_students == 0)
    {
        cout << "Студентов еще нет в базе данных\n\n";
        return;
    }

    const int bad_mark = 2;
    bool exist_bads = false;
    cout << "Студенты, у которых не менее трех оценок 2:\n";
    for (int i = 0; i < count_students; i++)
    {
        int count_bad_mark = 0;
        for (int j = 0; j < COUNT_SUBJECTS; j++)
        {
            if (db[i].marks[j] == bad_mark) count_bad_mark++;
        }
        if (count_bad_mark >= 3)
        {
            exist_bads = true;
            break;
        }
    }
    if (!exist_bads)
    {
        cout << "Нет таких студентов\n\n";
        return;
    }

    const int COUNT_FIELDS = 9;
    const char* titles[COUNT_FIELDS]{ "№", "Имя", "Курс", "Год поступления", "Мат. анализ", "Анал. геом.", "Инж. граф.", "Высок. прогр.", "Теор. инф." };
    int size_field[COUNT_FIELDS]{};
    for (int i = 0; i < COUNT_FIELDS; i++) size_field[i] = strlen(titles[i]);
    int count_digits = 0;
    int n = count_students;
    while (n > 0)
    {
        n /= 10;
        count_digits++;
    }
    size_field[0] = max(size_field[0], count_digits);

    for (int i = 0; i < count_students; i++)
    {
        size_field[1] = max(size_field[1], strlen(db[i].name));
        size_field[2] = max(size_field[2], strlen(db[i].course));
        size_field[3] = max(size_field[3], strlen(db[i].admission_year));
        for (int j = 0; j < COUNT_SUBJECTS; j++) size_field[j + 4] = max(size_field[j + 4], 1);
    }
    int width = COUNT_FIELDS - 1;
    for (int i = 0; i < COUNT_FIELDS; i++) width += size_field[i];
    cout << "+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n";
    for (int i = 0; i < COUNT_FIELDS; i++) cout << "|" << setw(size_field[i]) << titles[i];
    cout << "|\n";

    for (int i = 0; i < count_students; i++)
    {
        int count_bad_mark = 0;
        for (int j = 0; j < COUNT_SUBJECTS; j++)
        {
            if (db[i].marks[j] == bad_mark)
            {
                count_bad_mark++;
            }
        }
        if (count_bad_mark >= 3)
        {
            cout << "|";
            int index_field = 0;
            int curr_index = 0;
            for (int i = 0; i < width; i++)
            {
                curr_index++;
                if (curr_index == size_field[index_field] + 1)
                {
                    cout << "+";
                    index_field++;
                    curr_index = 0;
                }
                else cout << "-";
            }
            cout << "|\n";

            cout << "|";
            cout << setw(size_field[0]) << i << "|" << setw(size_field[1]) << db[i].name << "|" << setw(size_field[2]) << db[i].course << "|"
                << setw(size_field[3]) << db[i].admission_year << "|";
            for (int j = 0; j < COUNT_SUBJECTS; j++) cout << setw(size_field[j + 4]) << db[i].marks[j] << "|";
            cout << "\n";
        }
    }
    cout << "+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n\n";
}

void del(Student* db, int& count_students)
{
    print(db, count_students);
    if (count_students == 0)
    {
        cout << "Студентов нет в базе данных, удаление невозможно\n";
        return;
    }
    bool correct = false;
    int n = 0;
    while (!correct)
    {
        correct = true;
        char input[4] = "";
        cout << "Введите номер записи, которую нужно удалить:\n";
        cin.getline(input, 4);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else
        {
            correct = ((atoi(input) >= 0) && (atoi(input) < count_students) && strlen(input) != 0);
            for (int i = 0; i < strlen(input); i++)
            {
                if (!is_num(input[i]))
                {
                    correct = false;
                    break;
                }
            }
        }
        if (correct) n = atoi(input);
        else ERROR_INPUT;
    }
    for (int i = n + 1; i < count_students; i++) db[i - 1] = db[i];
    count_students--;
    system("cls");
    print(db, count_students);
}

void change(Student* db, int& count_students)
{
    if (count_students == 0)
    {
        cout << "В базе данных еще нет студентов!\n\n";
        return;
    }
    print(db, count_students);
    int note;
    bool correct = false;
    while (!correct)
    {
        correct = true;
        char input[4] = "";
        cout << "Введите номер записи, которую вы хотите изменить:\n";
        cin.getline(input, 4);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else
        {
            correct = (atoi(input) >= 0 && atoi(input) < count_students && strlen(input) != 0);
            for (int i = 0; i < strlen(input); i++)
            {
                if (!is_num(input[i]))
                {
                    correct = false;
                    break;
                }
            }
        }
        if (correct) note = atoi(input);
        else ERROR_INPUT;
    }
    int command;
    correct = false;
    while (!correct)
    {
        correct = true;
        char input[4] = "";
        cout << CHANGE_NAME << ". Изменить имя\n"
             << CHANGE_COURSE << ". Изменить курс\n"
             << CHANGE_ADMISSION_YEAR << ". Изменить год поступления\n"
             << CHANGE_MARKS << ". Изменить оценки\n";
        cin.getline(input, 4);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else
        {
            correct = (atoi(input) == CHANGE_NAME || atoi(input) == CHANGE_COURSE ||
                       atoi(input) == CHANGE_ADMISSION_YEAR || atoi(input) == CHANGE_MARKS);
            for (int i = 0; i < strlen(input); i++)
            {
                if (!is_num(input[i]))
                {
                    correct = false;
                    break;
                }
            }
        }
        if (correct) command = atoi(input);
        else ERROR_INPUT;
    }
    switch (command)
    {
    case CHANGE_NAME:
        input_name(&db[note]);
        break;
    case CHANGE_COURSE:
        input_course(&db[note]);
        break;
    case CHANGE_ADMISSION_YEAR:
        input_admission_year(&db[note]);
        break;
    case CHANGE_MARKS:
        input_marks(&db[note]);
        break;
    default:
        ERROR_INPUT;
        break;
    }
    system("cls");
    print(db, count_students);
}

void sort(Student* db, int& count_students)
{
    int command;
    bool correct = false;
    while (!correct)
    {
        correct = true;
        cout << SORT_NAME << ". Имя\n"
             << SORT_COURSE << ". Курс\n"
             << SORT_ADMISSION_YEAR << ". Год поступления\n"
             << SORT_MARKS << ". Средний балл\n"
             << "Введите по какому из полей вы хотите отсортировать базу данных: ";
        char input[2] = "";
        cin.getline(input, 2);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else
        {
            correct = (atoi(input) == SORT_NAME || atoi(input) == SORT_COURSE ||
                       atoi(input) == SORT_MARKS || atoi(input) == SORT_ADMISSION_YEAR);
            for (int i = 0; i < strlen(input); i++)
            {
                if (!is_num(input[i]))
                {
                    correct = false;
                    break;
                }
            }
        }

        if (correct) command = atoi(input);
        else ERROR_INPUT;
    }
    
    for (int i = 0; i < count_students - 1; i++)
    {
        for (int j = 0; j < count_students - i - 1; j++)
        {
            bool comp = false;
            switch (command)
            {
            case SORT_NAME:
                comp = compare(db[j].name, db[j + 1].name);
                break;
            case SORT_COURSE:
                comp = compare(db[j].course, db[j + 1].course);
                break;
            case SORT_ADMISSION_YEAR:
                comp = compare(db[j].admission_year, db[j + 1].admission_year);
                break;
            case SORT_MARKS:
                int sum1 = 0;
                int sum2 = 0;
                for (int k = 0; k < COUNT_SUBJECTS; k++)
                {
                    sum1 += db[j].marks[k];
                    sum2 += db[j + 1].marks[k];
                }
                comp = (sum1 > sum2);
                break;
            }
            if (comp) swap(db[j], db[j + 1]);
        }
    }
    print(db, count_students);
}

void input_name(Student* s)
{
    bool correct = false;
    while (!correct)
    {
        correct = true;
        char input[MAX_LEN_NAME] = "";
        cout << "Введите имя: ";
		cin.getline(input, MAX_LEN_NAME);

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else
        {
            for (int i = 0; i < strlen(input); i++)
            {
                if (!(is_letter(input[i]) || input[i] == ' '))
                {
                    correct = false;
                    break;
                }
            }
        }

        if (correct) strcpy_s(s->name, input);
        else ERROR_INPUT;
    }
}

void input_course(Student* s)
{
    bool correct = false;
    while (!correct)
    {
        correct = true;
        cout << "Введите курс: ";
        char input[2];
        cin.getline(input, 2);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else correct = (input[0] >= '1' && input[0] <= '6');

        if (correct) strcpy_s(s->course, input);
        else ERROR_INPUT;
    }
}

void input_admission_year(Student* s)
{
    bool correct = false;
    while (!correct)
    {
        correct = true;
        cout << "Введите год поступления: ";
        char input[5];
        cin.getline(input, 5);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(IGNORE, '\n');
            correct = false;
        }
        else correct = (atoi(input) >= 1980 && atoi(input) <= 2021);

        if (correct) strcpy_s(s->admission_year, input);
        else ERROR_INPUT;
    }
}

void input_marks(Student* s)
{
    cout << "Введите оценки за:\n";
    for (int j = 0; j < COUNT_SUBJECTS; j++)
    {
        bool correct = false;
        while (!correct)
        {
            cout << j + 1 << " предмет: ";
            char input[2] = "";
            cin.getline(input, 2);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(IGNORE, '\n');
                correct = false;
            }
            else correct = (atoi(input) >= 2 && atoi(input) <= 5);

            if (correct)
            {
                s->marks[j] = atoi(input);
                //strcpy_s(s->marks[j], input);
                //for (int i = 0; i < strlen(input); i++) s->marks[j][i] = input[i];
            }
            else ERROR_INPUT;
        }
    }
}

void load_in_file(Student* db, int& count_students, const char* file_name)
{
    const char* delim = "*";
    ofstream fout(file_name, ios::out);
    if (fout.is_open())
    {
        for (int i = 0; i < count_students; i++)
        {
            fout << db[i].name << delim << db[i].course << delim << db[i].admission_year;
            for (int j = 0; j < COUNT_SUBJECTS; j++) fout << delim << db[i].marks[j];
            fout << "\n";
        }
        fout.close();
    }
    else
    {
        cout << "Произошла ошибка!\n\n";
        return;
    }
    cout << "Запись произошла успешно!\n\n";
}

void unload_from_file(Student* db, int& count_students, const char* file_name)
{
    count_students = 0;
    ifstream fin(file_name, ios::in);
    if (fin.is_open())
    {
        char input[100] = "";
        const char* words[8];
        while (fin.getline(input, 100))
        {
            int cnt = 0;
            get_words(input, words, cnt);
            strcpy_s(db[count_students].name, words[0]);
            strcpy_s(db[count_students].course, words[1]);
            strcpy_s(db[count_students].admission_year, words[2]);
            for (int i = 0; i < COUNT_SUBJECTS; i++) db[count_students].marks[i] = atoi(words[i + 3]);
            count_students++;
        }
        fin.close();
    }
    else
    {
        cout << "Произошла ошибка!\n\n";
        return;
    }
    cout << "Выгрузка произошла успешно\n\n";
    print(db, count_students);
}

bool compare(char* s1, char* s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int min_len = (len1 < len2 ? len1 : len2);
    for (int i = 0; i < min_len; i++)
    {
        if (s1[i] > s2[i]) return true;
        else if (s1[i] != s2[i]) return false;
    }
    return len1 > len2;
}

bool is_letter(char c)
{
    bool eng_lower = (c >= 'a' && c <= 'z');
    bool eng_upper = (c >= 'A' && c <= 'Z');
    bool rus_lower = (c >= 'а' && c <= 'я');
    bool rus_upper = (c >= 'А' && c <= 'Я');
    return eng_lower || eng_upper || rus_lower || rus_upper;
}

bool is_num(char c)
{
    return ('0' <= c && c <= '9');
}

void swap(Student& s1, Student& s2)
{
    Student temp = s1;
    s1 = s2;
    s2 = temp;
}

void get_words(char* text, const char** words, int& count)
{
    const char* delim = "*";
    char* next_token = nullptr;
    char* token = strtok_s(text, delim, &next_token);
    count = 0;
    while (token)
    {
        words[count++] = token;
        token = strtok_s(nullptr, delim, &next_token);
    }
}

int max(int x, int y)
{
    return (x > y ? x : y);
}