#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;


struct User {

    int userId;
    string login, password;
};

void readUsersFromExternalFile(vector<User> &users);
int measureUserVectorLength(vector<User> &users);
int checkMaxId(vector<User> &users, int sizeOfVector);
bool SignUp(vector<User> &users, int occupiedMaxId, string login, string password);
bool SignIn (vector<User> &users, string login, string password);

int main() {

    int LoggedUserId = 0;
    int numberOfUsers = 0;
    int sizeOfVector = 0;
    int occupiedMaxId = 0;
    char programOption;
    int newUserId = 0;
    char test;
    string login, password;
    vector<User> users;

    readUsersFromExternalFile(users);
    sizeOfVector = measureUserVectorLength(users);
    occupiedMaxId = checkMaxId(users, sizeOfVector);

    while (true) {
        system("cls");
        cout << "----- KSIAZKA ADRESOWA -----" << endl;
        cout << "1. Rejestracja" << endl;
        cout << "2. Logowanie" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << endl << "Twoj wybor: ";
        cin >> programOption;

        switch (programOption) {
        case '1':
            for (int i=0; i <users.size(); i++) {
                cout << users[i].userId << endl;
                cout << users[i].login << endl;
                cout << users[i].password << endl;
                cout << endl;
            }

            cout << "Wprowadz login: ";
            cin >> login;
            cout << "Wprowadz haslo: ";
            cin >> password;

            if ( SignUp(users, occupiedMaxId, login, password) == true) {
                occupiedMaxId++;
            }
            break;
        case '2':
            cout << "Wprowadz login: ";
            cin >> login;
            cout << "Wprowadz haslo: ";
            cin >> password;

            if (SignIn (users, login, password) == true) {
                while (true) {

                    system("cls");
                    cout << "----- KSIAZKA ADRESOWA -----" << endl;
                    cout << "1. Dodaj kontakt" << endl;
                    cout << "2. Wyszukaj po imieniu" << endl;
                    cout << "3. Wyszukaj po nazwisku" << endl;
                    cout << "4. Wyswietl wszystkie kontakty" << endl;
                    cout << "5. Usun kontakt" << endl;
                    cout << "6. Edytuj kontakt" << endl;
                    cout << "9. Zamknij program" << endl;
                    cout << endl << "Twoj wybor: ";

                    cin >> programOption;

                    switch (programOption) {

                    case '9':
                        exit(0);
                        break;
                    default:
                        cout << "Wybrano nieprawidlowa opcje menu. Sprobuj ponownie!" << endl;
                        Sleep(2000);
                        break;
                    }
                }
            }

            else {
                cout <<"Wprowadzone bledne parametry logowania! Logowanie nie powiodlo sie!" << endl;
                Sleep(2000);
            }
            break;
        case '9':
            exit(0);
            break;
        default:
            cout << "Wybrano nieprawidlowa opcje menu. Sprobuj ponownie!" << endl;
            Sleep(2000);
            break;
        }
    }
    return 0;
}


void readUsersFromExternalFile(vector<User> &users) {
    fstream externalFile;
    string line;
    vector <string> userComponent;
    externalFile.open("Users.txt", ios::in);

    if (externalFile.good() == true) {
        while (!externalFile.eof()) {
            getline(externalFile, line);
            stringstream splitContact(line);
            string intermediate;
            while(getline(splitContact, intermediate, '|')) {
                userComponent.push_back(intermediate);
            }
        }
        for(int i = 0; i < userComponent.size(); i++) {
            string fraction = userComponent[i];
            stringstream composeUser(fraction);
            int convertedId = 0;
            composeUser >> convertedId;
            users.push_back ({convertedId, userComponent[i+1], userComponent[i+2]});
            i+=2;
        }
    }
    externalFile.close();
}

int measureUserVectorLength (vector<User> &users) {
    return users.size();
}


int checkMaxId (vector<User> &users, int sizeOfVector) {
    int maxId = 0;
    if (sizeOfVector >= 1) {
        for (int i = 0; i < sizeOfVector; i++) {
            if (maxId < users[i].userId) {
                maxId = users[i].userId;
            }
        }
    }
    return maxId;
}

bool SignUp(vector<User> &users, int occupiedMaxId, string login, string password) {
    int sizeOfVector = measureUserVectorLength(users);
    for (int i = 0; i < sizeOfVector; i++) {
        if (users[i].login == login) {
            cout << "Wprowadzony login jest juz zarezerwany! Rejestracja nie powiodla sie!"  ;
            Sleep(2000);
            return false;
            break;
        } else if ((users[i].login != login) && (i == sizeOfVector - 1)) {
            users.push_back( {occupiedMaxId + 1, login, password} );
            fstream externalFile;
            externalFile.open("Users.txt", ios::out | ios::app);
            externalFile << occupiedMaxId+1 << "|" << login << "|" << password << "|" << endl;
            externalFile.close();
            cout << "Rejestracja zakonczona sukcesem! Przejdz do opcji logowania!" << endl;
            Sleep(2000);
            return true;
        }
    }
}


bool SignIn (vector<User> &users, string login, string password) {
    int sizeOfVector = measureUserVectorLength(users);
    for (int i = 0; i < sizeOfVector; i++) {
        if ((users[i].login == login) && (users[i].password == password)) {
            cout << "Autoryzacja poprawna!" << endl;
            Sleep(2000);
            return true;
            break;
        }
    }
    return false;
}
