#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <conio.h>


using namespace std;


struct User {

    int userId;
    string login, password;
};

struct Contact {

    int contactId, userId;
    string firstName, lastName, phoneNumber, emailAddress, address;
};


void readFromExternalFile(vector<User> &users);
void readFromExternalFile(vector<Contact> &contacts);
int measureVectorLength (vector<User> &users);
int measureVectorLength (vector<Contact> contacts);
int checkMaxId (vector<User> &users, int userVectorSize);
int checkMaxId (vector<Contact> &contacts, int contactVectorSize);
bool signUp(vector<User> &users, int occupiedMaxUserId, string login, string password);
int signIn (vector<User> &users, string login, string password);
int readContactsFromExternalFile(vector<User> &users, vector<Contact> &contacts, int currentUserId);
int addContact(vector<Contact> &contacts, int currentUserId, int occupiedMaxContactId);
void findContactByFirstName(vector<Contact> contacts);
void findContactByLastName(vector<Contact> contacts);
void displayAllContacts(vector<Contact> contacts);
void editContactById(vector<Contact> &contacts);
void rewriteContactsAfterEditing(vector<Contact> &contacts);
void copyContactsFromTempToOrigin();
void deleteContactById(vector<Contact> &contacts);
void rewriteContactsAfterRemoval(vector<Contact> &contacts, int identifier);


int main() {

    int userVectorSize = 0;
    int contactVectorSize = 0;
    int occupiedMaxUserId = 0;
    int occupiedMaxContactId = 0;
    int currentUserId = 0;
    char programOption;
    string login, password;
    vector<User> users;
    vector<Contact> contacts;

    readFromExternalFile(users);

    while (true) {
        system ("cls");
        cout << "----- KSIAZKA ADRESOWA -----" << endl;
        cout << "1. Rejestracja" << endl;
        cout << "2. Logowanie" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << endl << "Twoj wybor: ";
        cin >> programOption;

        switch (programOption) {
        case '1':
            userVectorSize = measureVectorLength(users);
            occupiedMaxUserId = checkMaxId(users, userVectorSize);
            cout << "Wprowadz login: ";
            cin >> login;
            cout << "Wprowadz haslo: ";
            cin >> password;
            if (signUp(users, occupiedMaxUserId, login, password) == true) {
                occupiedMaxUserId++;
            }
            break;
        case '2':
            cout << "Wprowadz login: ";
            cin >> login;
            cout << "Wprowadz haslo: ";
            cin >> password;
            currentUserId = signIn(users, login, password);
            if (currentUserId != 0) {
                occupiedMaxContactId = readContactsFromExternalFile(users, contacts, currentUserId);
                while (true) {
                    system ("cls");
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
                    case '1':
                        occupiedMaxContactId = addContact(contacts, currentUserId, occupiedMaxContactId);
                        break;
                    case '2':
                        findContactByFirstName(contacts);
                        break;
                    case '3':
                        findContactByLastName(contacts);
                        break;
                    case '4':
                        displayAllContacts(contacts);
                        break;
                    case '5':
                        deleteContactById(contacts);
                        break;
                    case '6':
                        editContactById(contacts);
                        break;
                    case '9':
                        exit(0);
                        break;
                    default:
                        cout << "Wybrano nieprawidlowa opcje menu. Sprobuj ponownie!" << endl;
                        Sleep(1500);
                        break;
                    }
                }
            } else {
                cout <<"Wprowadzone bledne parametry logowania! Logowanie nie powiodlo sie!" << endl;
                Sleep(1500);
            }
            break;
        case '9':
            exit(0);
            break;
        default:
            cout << "Wybrano nieprawidlowa opcje menu. Sprobuj ponownie!" << endl;
            Sleep(1500);
            break;
        }
    }
    return 0;
}


void readFromExternalFile(vector<User> &users) {
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


int measureVectorLength(vector<User> &users) {
    return users.size();
}


int checkMaxId (vector<User> &users, int userVectorSize) {
    int maxId = 0;
    if (userVectorSize >= 1) {
        for (int i = 0; i < userVectorSize; i++) {
            if (maxId < users[i].userId) {
                maxId = users[i].userId;
            }
        }
    }
    return maxId;
}


bool signUp(vector<User> &users, int occupiedMaxUserId, string login, string password) {
    int userVectorSize = measureVectorLength(users);
    for (int i = 0; i < userVectorSize; i++) {
        if (users[i].login == login) {
            cout << "Wprowadzony login jest juz zarezerwany! Rejestracja nie powiodla sie!"  ;
            Sleep(1500);
            return false;
            break;
        } else if ((users[i].login != login) && (i == userVectorSize - 1)) {
            users.push_back( {occupiedMaxUserId + 1, login, password} );
            fstream externalFile;
            externalFile.open("Users.txt", ios::out | ios::app);
            externalFile << occupiedMaxUserId+1 << "|" << login << "|" << password << "|" << endl;
            externalFile.close();
            cout << "Rejestracja zakonczona sukcesem! Przejdz do opcji logowania!" << endl;
            Sleep(1500);
            return true;
        }
    }
}


int signIn (vector<User> &users, string login, string password) {
    int userVectorSize = measureVectorLength(users);
    for (int i = 0; i < userVectorSize; i++) {
        if ((users[i].login == login) && (users[i].password == password)) {
            cout << "Autoryzacja poprawna!" << endl;
            Sleep(1500);
            return users[i].userId;
            break;
        }
    }
    return -1;
}


int readContactsFromExternalFile(vector<User> &users, vector<Contact> &contacts, int currentUserId ) {
    int occupiedMaxContactId =0;
    fstream externalFile;
    string line;
    vector <string> userComponent;
    externalFile.open("AddressBook.txt", ios::in);

    if (externalFile.good() == true) {
        while (!externalFile.eof()) {
            getline(externalFile, line);
            stringstream splitContact(line);
            string intermediate;
            while(getline(splitContact, intermediate, '|')) {
                userComponent.push_back(intermediate);
            }
        }
    }
    for(int i = 0; i < userComponent.size(); i++) {
        string contactIdFraction = userComponent[i];
        string contactUserIdFraction = userComponent[i+1];
        stringstream composeContactId(contactIdFraction);
        stringstream composeUserId(contactUserIdFraction);
        int convertedContactId = 0;
        int convertedUserId = 0;
        composeContactId >> convertedContactId;
        composeUserId >> convertedUserId;

        if (occupiedMaxContactId < convertedContactId) {
            occupiedMaxContactId = convertedContactId;
        }

        if (convertedUserId == currentUserId) {
            contacts.push_back ({convertedContactId, convertedUserId, userComponent[i+2], userComponent[i+3], userComponent[i+4], userComponent[i+5], userComponent[i+6]} );
            i+=6;
        } else {
            i+=6;
        }
    }
    externalFile.close();
    return occupiedMaxContactId;
}


int addContact(vector<Contact> &contacts, int currentUserId, int occupiedMaxContactId) {

    int sizeOfContacts, id, maxId;
    string firstName, lastName, phoneNumber, emailAddress, address;
    sizeOfContacts = measureVectorLength(contacts);
    maxId = checkMaxId (contacts, sizeOfContacts);
    cout << "Podaj imie: ";
    cin >> firstName;
    cout << "Podaj nazwisko: ";
    cin >> lastName;
    cout << "Podaj numer telefonu: ";
    cin.sync();
    getline(cin, phoneNumber);
    cout << "Podaj adres email: ";
    cin >> emailAddress;
    cout << "Podaj adres zamieszkania: ";
    cin.sync();
    getline(cin, address);
    id = occupiedMaxContactId + 1;
    contacts.push_back( {id, currentUserId, firstName, lastName, phoneNumber, emailAddress, address} );
    fstream externalFile;
    externalFile.open("AddressBook.txt", ios::out | ios::app);
    externalFile << id << "|" << currentUserId << "|" << firstName << "|" << lastName << "|" << phoneNumber << "|" <<emailAddress << "|" << address << "|" << endl;
    externalFile.close();
    cout << "Kontakt zostal dodany!" << endl;
    Sleep(1500);
    return occupiedMaxContactId + 1;
}


int measureVectorLength (vector<Contact> contacts) {
    return contacts.size();
}


int checkMaxId (vector<Contact> &contacts, int contactVectorSize) {
    int maxId = 0;
    if (contactVectorSize >= 1) {
        for (int i = 0; i < contactVectorSize; i++) {
            if (maxId < contacts[i].contactId) {
                maxId = contacts[i].contactId;
            }
        }
    }
    return maxId;
}


void findContactByFirstName(vector<Contact> contacts) {
    string searchedFirstName;
    int hitCount = 0;
    int i = 0;
    int sizeOfContacts = measureVectorLength(contacts);
    cout << "Wprowadz imie, po ktorym nastapi wyszukanie kontaktow: ";
    cin >> searchedFirstName;
    while (i < sizeOfContacts ) {
        if(contacts[i].firstName == searchedFirstName) {
            cout << contacts[i].contactId << endl;
            cout << contacts[i].firstName << endl;
            cout << contacts[i].lastName << endl;
            cout << contacts[i].phoneNumber << endl;
            cout << contacts[i].emailAddress << endl;
            cout << contacts[i].address << endl;
            cout << endl;
            hitCount++;
        }
        i++;
    }
    if (hitCount == 0) {
        cout << endl << "Nie znaleziono kontaktu!" << endl;
    } else {
        cout << "Liczba znaleznionych kontaktow: " << hitCount <<endl;
    }
    cout << "Aby kontynuowac, nacisnij dowolny klawisz...";
    getch();
}


void findContactByLastName(vector<Contact> contacts) {
    string searchedLastName;
    int hitCount = 0;
    int i = 0;
    int sizeOfContacts = measureVectorLength(contacts);
    cout << "Wprowadz nazwisko, po ktorym nastapi wyszukanie kontaktow: ";
    cin >> searchedLastName;
    while (i < sizeOfContacts ) {
        if (contacts[i].lastName == searchedLastName) {
            cout << contacts[i].contactId << endl;
            cout << contacts[i].firstName << endl;
            cout << contacts[i].lastName << endl;
            cout << contacts[i].phoneNumber << endl;
            cout << contacts[i].emailAddress << endl;
            cout << contacts[i].address << endl;
            cout << endl;
            hitCount++;
        }
        i++;
    }
    if (hitCount == 0) {
        cout << endl << "Nie znaleziono kontaktu!" << endl;
    } else {
        cout << "Liczba znaleznionych kontaktow: " << hitCount <<endl;
    }
    cout << "Aby kontynuowac, nacisnij dowolny klawisz...";
    getch();
}


void displayAllContacts(vector<Contact> contacts) {
    int sizeOfContacts = measureVectorLength(contacts);
    for(int i=0; i < sizeOfContacts; i++) {
        cout << contacts[i].contactId << endl;
        cout << contacts[i].firstName << endl;
        cout << contacts[i].lastName << endl;
        cout << contacts[i].phoneNumber << endl;
        cout << contacts[i].emailAddress << endl;
        cout << contacts[i].address << endl;
        cout << endl;
    }
    cout << "Liczba wyswietlonych kontaktow: " << sizeOfContacts << endl;
    cout << "Aby kontynuowac, nacisnij dowolny klawisz...";
    getch();
}


void editContactById(vector<Contact> &contacts) {
    char option;
    int identifier;
    string editedFirstName, editedLastName, editedPhoneNumber, editedEmailAddress, editedAddress;
    int sizeOfContacts = measureVectorLength(contacts);
    cout << "Podaj id kontaktu, dla ktorego chcesz zmodyfikowac dane: ";
    cin >> identifier;
    for (int k = 0; k < sizeOfContacts; k++) {
        if (contacts[k].contactId == identifier) {
            cout << "----- Edycja -----" << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Numer telefonu" << endl;
            cout << "4. Email" << endl;
            cout << "5. Adres" << endl;
            cout << "6. Powrot do menu glownego" << endl;
            cout << endl << "Wybierz opcje: ";
            cin >> option;
            switch (option) {
            case '1':
                cout << "Wprowadz imie: ";
                cin >> editedFirstName;
                for (int i = 0; i < sizeOfContacts; i++) {
                    if (contacts[i].contactId == identifier) {
                        contacts[i].firstName = editedFirstName;
                    }
                }
                rewriteContactsAfterEditing (contacts);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal zmieniony!";
                Sleep(1500);
                break;
            case '2':
                cout << "Wprowadz nazwisko: ";
                cin >> editedLastName;
                for (int i = 0; i < sizeOfContacts; i++) {
                    if (contacts[i].contactId == identifier) {
                        contacts[i].lastName = editedLastName;
                    }
                }
                rewriteContactsAfterEditing (contacts);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal zmieniony!";
                Sleep(1500);
                break;
            case '3':
                cout << "Wprowadz numer telefonu: ";
                cin.clear();
                cin.sync();
                getline(cin, editedPhoneNumber);
                for (int i = 0; i < sizeOfContacts; i++) {
                    if (contacts[i].contactId == identifier) {
                        contacts[i].phoneNumber = editedPhoneNumber;
                    }
                }
                rewriteContactsAfterEditing (contacts);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal zmieniony!";
                Sleep(1500);
                break;
            case '4':
                cout << "Wprowadz adres email: ";
                cin >> editedEmailAddress;
                for (int i = 0; i < sizeOfContacts; i++) {
                    if (contacts[i].contactId == identifier) {
                        contacts[i].emailAddress = editedEmailAddress;
                    }
                }
                rewriteContactsAfterEditing (contacts);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal zmieniony!";
                Sleep(1500);
                break;
            case '5':
                cout << "Wprowadz adres: ";
                cin.clear();
                cin.sync();
                getline(cin, editedAddress);
                for (int i = 0; i < sizeOfContacts; i++) {
                    if (contacts[i].contactId == identifier) {
                        contacts[i].address = editedAddress;
                    }
                }
                rewriteContactsAfterEditing (contacts);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal zmieniony!";
                Sleep(1500);
                break;
            case '6':
                break;
            }
        }
    }
}

void rewriteContactsAfterEditing(vector<Contact> &contacts) {
    int j = 0;
    string line;
    fstream originalExternalFile, temporaryExternalFile;
    vector <string> contactComponent;
    originalExternalFile.open("AddressBook.txt");
    temporaryExternalFile.open("AddressBook_temp.txt", ofstream::out | ofstream::trunc);
    if (originalExternalFile.good() == true) {
        while (!originalExternalFile.eof()) {
            getline(originalExternalFile, line);
            stringstream splitContact(line);
            string intermediate;
            while(getline(splitContact, intermediate, '|')) {
                contactComponent.push_back(intermediate);
            }
        }
        for(int i = 0; i < contactComponent.size(); i++) {
            string fraction = contactComponent[i];
            stringstream composeContact(fraction);
            int convertedId = 0;
            composeContact >> convertedId;
            if (convertedId == contacts[j].contactId) {
                temporaryExternalFile << contacts[j].contactId << "|" << contacts[j].userId << "|" << contacts[j].firstName << "|" << contacts[j].lastName << "|" << contacts[j].phoneNumber << "|" << contacts[j].emailAddress << "|" << contacts[j].address << "|" << endl;
                j++;
                i+=6;
            } else {
                temporaryExternalFile << contactComponent[i] << "|" << contactComponent[i+1] << "|" << contactComponent[i+2] << "|" << contactComponent[i+3] << "|" << contactComponent[i+4] << "|" << contactComponent[i+5] << "|" << contactComponent[i+6] << "|" << endl;
                i+=6;
            }

        }
    }
}

void copyContactsFromTempToOrigin() {
    ifstream inputFile("AddressBook_temp.txt", ios::in);
    ofstream outputFile("AddressBook.txt", ios::out | ios::trunc);
    outputFile << inputFile.rdbuf();
}


void deleteContactById(vector<Contact> &contacts) {
    fstream temporaryExternalFile;
    int identifier;
    int deletedContactId;
    cout << "Wprowadz id kontaktu, ktory nalezy usunac: ";
    cin >> identifier;
    char deleteConfirmation;

    vector<Contact>::iterator searchProperId = contacts.begin();
    while (searchProperId != contacts.end()) {
        if (searchProperId-> contactId == identifier) {
            cout << "Aby zatwierdzic operacje, wcisnij 't'" << endl;
            cin >> deleteConfirmation;
            if (deleteConfirmation == 't') {
                searchProperId = contacts.erase(searchProperId);
                rewriteContactsAfterRemoval (contacts, identifier);
                copyContactsFromTempToOrigin();
                cout << "Kontakt zostal usuniety!";
                Sleep(1500);
                break;
            } else {
                cout << "Operacja usuniecie niewlasciwie potwierdzona!" << endl;
                Sleep(1500);
                break;
            }
        } else {
            searchProperId++;
        }
    }
}


void rewriteContactsAfterRemoval(vector<Contact> &contacts, int identifier) {
    int j = 0;
    string line;
    fstream originalExternalFile, temporaryExternalFile;
    vector <string> contactComponent;
    originalExternalFile.open("AddressBook.txt");
    temporaryExternalFile.open("AddressBook_temp.txt", ofstream::out | ofstream::trunc);
    if (originalExternalFile.good() == true) {
        while (!originalExternalFile.eof()) {
            getline(originalExternalFile, line);
            stringstream splitContact(line);
            string intermediate;
            while(getline(splitContact, intermediate, '|')) {
                contactComponent.push_back(intermediate);
            }
        }
        for(int i = 0; i < contactComponent.size(); i++) {
            string fraction = contactComponent[i];
            stringstream composeContact(fraction);
            int convertedId = 0;
            composeContact >> convertedId;
            if ((convertedId == contacts[j].contactId) && (convertedId != identifier)) {
                temporaryExternalFile << contacts[j].contactId << "|" << contacts[j].userId << "|" << contacts[j].firstName << "|" << contacts[j].lastName << "|" << contacts[j].phoneNumber << "|" << contacts[j].emailAddress << "|" << contacts[j].address << "|" << endl;
                j++;
                i+=6;
            } else if (convertedId != identifier) {
                temporaryExternalFile << contactComponent[i] << "|" << contactComponent[i+1] << "|" << contactComponent[i+2] << "|" << contactComponent[i+3] << "|" << contactComponent[i+4] << "|" << contactComponent[i+5] << "|" << contactComponent[i+6] << "|" << endl;
                i+=6;
            } else {
                i+=6;
            }
        }
    }
}
