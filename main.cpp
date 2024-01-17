#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include <cstring>
using namespace std;

int main();
vector<vector<string>> LoadDB(); // Тут я определяю две функции чтобы использовать их потом в тех что стоят выше их обьявлений

vector<string> split(const string &str) { // Функция разделения строки на слова (разделение по пробелу) и возвращающая вектор этих слов
    vector<string> vec;
    string s;
    for (const char i: str) {
        if (i!='_')
            s+=i;
        else {
            vec.push_back(s);
            s="";
        }
    }
    vec.push_back(str);
    return vec;
}

void AddGenre(const string &filename) { // Функция добавления жанра которая записывает жанр в файл genre.txt
    system("cls");
    ofstream os(filename, ios::app);
    string genre;
    cout<<"Enter a genre > ";
    cin>>genre;
    os<<genre+'.';
    os.close();
}

vector<string> LoadGenres(const string &filename) { // Функция загрузки жанров возвращающая вектор строк содержащий все жанры находящиеся в genres.txt, они разделяются точками
    ifstream is(filename);
    vector<string> genres;
    string line;

    string s;
    while(getline(is, line)) {
        for (const char i: line) {
            if (i!='.')
                s+=i;
            else {
                genres.push_back(s);
                s="";
            }
        }
    }
    is.close();
    return genres;
}

void GenreBooksList(const vector<string> &genres) { // Функция выводящая список книг по жанрам принимающая в аргументах неизменяемый вектор строк содержащий жанры, эта функция у меня выводит пронумерованый список жанров а в дальнейшем после выбора жанра выводит все книги этого жанра
    system("cls");
    int i=1;
    for (const string &genre: genres) {
        cout<<i<<") "+genre+"\n";
        i++;
    }

    int choice;
    cout<<"Your choice > ";
    cin>>choice;

    const vector<vector<string>> books = LoadDB();
    int j=1;
    for (auto & book : books) {
        if (book[2]==genres[choice-1]) {
            cout<<j<<"\nName: "<<book[0]<<"\nAuthor: "<<book[1]<<"\n\n";
            j+=1;
        }
    }

    cout<<"Your choice (0 to exit) > ";
    cin>>choice;

    if(choice!=0) {
        system("cls");
        string check;
        cout<<"Name: "<<books[choice-1][0]<<"\nAuthor: "<<books[choice-1][1]<<"\nGenre: "<<books[choice-1][2]<<"\nDescription: "<<books[choice-1][3]<<"\n\n";
        cout<<"Go back? (any char there) > ";
        cin>>check;
    }
}

void Search(const string &keyword) { // Функция поиска по ключевому слову, в качестве аргумента идет константа (ключевое слово), после чего функция загружает список книг из базы и потом разделяет поля название и автор по пробелам и проверяет на наличие ключевого слова в них после чего добавляет айдишники найденных книг в вектор, который в последствии сортирует и удаляет дубликаты, после чего выводит книги с этими айдишниками
    system("cls");

    const vector<vector<string>> books = LoadDB();
    vector<int> bookIDs;
    for (int i=0; i<books.size();i++) {
        for(int j=0; j<=1;j++) {
            for (const string &word: split(books[i][j]))
                if (word==keyword)
                    bookIDs.push_back(i);
        }
    }
    sort(bookIDs.begin(), bookIDs.end());
    bookIDs.resize(unique(bookIDs.begin(), bookIDs.end())-bookIDs.begin());

    cout<<"Found books list\n\n";

    for (const int id: bookIDs) {
        cout<<"ID: "<<id<<"\nName: "<<books[id][0]<<"\nAuthor: "<<books[id][1]<<"\nGenre: "<<books[id][2]<<"Description: "<<books[id][3]<<"\n\n";
    }
    int choice;
    cout<<"Your choice (0 to exit) > ";
    cin>>choice;
    if(choice!=0) {
        system("cls");
        string check;
        cout<<"Name: "<<books[choice-1][0]<<"\nAuthor: "<<books[choice-1][1]<<"\nGenre: "<<books[choice-1][2]<<"\nDescription: "<<books[choice-1][3]<<"\n\n";
        cout<<"Go back (any char there) > ";
        cin>>check;
    }
}


vector<vector<string>> LoadDB() { // Функция загрузки книг из базы данных (booksDB.txt), которая считывает оттуда каждую строку и потом разделяет ее по точкам (шаблон: название.автор.жанр.описание), добавляет эти айтемы в вектор, который в последствии добавляет в главный вектор и так для каждой строки, возвращает главный вектор
    vector<vector<string>> books;

    ifstream is("booksDB.txt");
    string line;
    while(getline(is, line)) {
        vector<string> book;
        string bookItem;
        for (char i: line) {
            if (i!='.')
                bookItem += i;
            else {
                book.push_back(bookItem);
                bookItem="";
            }
        }
        books.push_back(book);
    }
    is.close();
    return books;
}

void DeleteBook(const int &ID, const string &filename){ // Функция удаления книги, принимает на вход ID книги и имя файла базы книг, загружает книги из базы через функцию LoadDB и создает новый файл temp.txt, в который записывает все строки кроме строки с введенным ID (путем сверки)
    const vector<vector<string>> books = LoadDB();
    ofstream temp("temp.txt", ios::app);
    for (int i=0;i<books.size();i++){
        if (i!=ID){
            string s;
            for (const string &bookItem:books[i])
                s+=bookItem+'.';
            temp<<s+"\n";
        }
    }
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
    temp.close();
}

void AddBook(const string &filename) { // Функция добавления книги, открывает файл на запись и записывает туда книгу с ввода из клавиатуры
    system("cls");
    ofstream os(filename, ios::app);
    string tempBook;
    cout<<"Enter a name.author.genre.description. there > ";
    cin>>tempBook;
    os<<tempBook+"\n";
    os.close();
}

void BooksList(const vector<vector<string>> &books) { // Функция вывода книг на экран (вообще всех) после чего можно открыть книгу и посмотреть жанр и описание
    system("cls");
    int choice;
    cout<<"Books list\n\n";
    for (int i=0;i<books.size();i++) {
        cout<<to_string(i+1)<<"\nName: "<<books[i][0]<<"\nAuthor: "<<books[i][1]<<"\nID: "<<i<<"\n\n";
    }
    cout<<"Your choice (0 to exit) > ";
    cin>>choice;
    if(choice!=0) {
        system("cls");
        string check;
        cout<<"Name: "<<books[choice-1][0]<<"\nAuthor: "<<books[choice-1][1]<<"\nGenre: "<<books[choice-1][2]<<"\nDescription: "<<books[choice-1][3]<<"\n\n";
        cout<<"Go back (any char there) > ";
        cin>>check;
    }
}

int main() { // Главная функция которая предлагает нам различные действия которые можно сделать с базой книг, и обрабатывает наш выбор в конструкции switch-case, а если там не нашлось значения для обработки или значение не верно то просто функция перезапускается
    while (true) {
        system("cls");
        int choice;

        cout<<"Books catalog\n\n"
        "1) Books list\n"
        "2) Add a book\n"
        "3) Book list by genre\n"
        "4) Add a genre\n"
        "5) Search a book by keyword\n"
        "6) Delete\n\n";
        cout<<"Your choice (0 to exit) > ";
        cin>>choice;

        switch (choice) {
            case 0: {
                exit(0);
                continue;
            }
            case 1: {
                const vector<vector<string>> books = LoadDB();
                BooksList(books);
                continue;
            }
            case 2: {
                AddBook("booksDB.txt");
                continue;
            }
            case 3: {
                const vector<string> genres = LoadGenres("genres.txt");
                GenreBooksList(genres);
                continue;
            }
            case 4: {
                AddGenre("genres.txt");
                continue;
            }
            case 5: {
                string keyword;
                cout<<"Enter a keyword > ";
                cin>>keyword;
                Search(keyword);
                continue;
            }
            case 6: {
                int ID;
                cout<<"Enter a book ID > ";
                cin>>ID;
                DeleteBook(ID, "booksDB.txt");
            }
            default:
                continue;
        }
    }
    return 0;
}
