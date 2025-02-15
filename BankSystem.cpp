using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

const string filename = "BankClients.txt";

enum Actions {
    SHOW = 1,
    ADD = 2,
    DELETE = 3,
    UPDATE = 4,
    FIND = 5,
    TRANSACTION = 6,
    EXIT = 7,

};

enum Transaction {
    DIPOSIT = 1,
    WITHDRAW = 2,
    TOTAL_BALANCE = 3,
    MAIN_MENU = 4
};

vector<string> splitString(string text, string spar) {
    vector<string> fragments;
    size_t sparZone;
    size_t start = 0;

    while ((sparZone = text.find(spar, start)) != string::npos) {
        fragments.push_back(text.substr(start, sparZone - start));
        start = sparZone + spar.length();
    }
    if (start < text.length()) {
        fragments.push_back(text.substr(start));
    }

    return fragments;
}

string inputInfo(string message) {
    string info;
    cout << message;
    cin >> ws;
    getline(cin, info);

    return info;
}

struct sClient {
    string AccountNum = "";
    string PinCode = "";
    string Name = "";
    string PhoneNum = "";
    int AccountBalance = 0;
};

bool alreadyExist(vector <sClient>& myClients, string AccountNumber) {
    for (sClient& client : myClients) {
        if (client.AccountNum == AccountNumber) {
            return 1;
            break;
        }
    }

    return 0;
}

sClient fillClientInfo(vector <sClient>& myClients) {
    sClient newClient;
    newClient.AccountNum = inputInfo("Enter Account Num : ");
    while (alreadyExist(myClients, newClient.AccountNum)) {
        cout << "Account Number Already Exist, ";
        newClient.AccountNum = inputInfo("Enter New Account Num : ");
    }
    newClient.PinCode = inputInfo("Enter Pin Code : ");
    newClient.Name = inputInfo("Enter Client Name : ");
    newClient.PhoneNum = inputInfo("Enter Phone Number : ");
    newClient.AccountBalance = stoi(inputInfo("Enter Account Balance : "));

    return newClient;
}

string recordToLine(sClient Client, string spar = "##//##") {
    string infoLine = "";
    infoLine = Client.AccountNum + spar + Client.PinCode + spar + Client.Name + spar + Client.PhoneNum + spar + to_string(Client.AccountBalance);

    return infoLine;
}

sClient lineToRecord(string line, string spar = "##//##") {
    sClient Client;
    vector<string> lineInfo;
    lineInfo = splitString(line, spar);

    Client.AccountNum = lineInfo[0];
    Client.PinCode = lineInfo[1];
    Client.Name = lineInfo[2];
    Client.PhoneNum = lineInfo[3];
    Client.AccountBalance = stoi(lineInfo[4]);

    return Client;
}

void printClientInfo(sClient Client) {
    cout << "\n-----------------------------------------------------\n";
    cout << "Account Number : " << Client.AccountNum << endl;
    cout << "Pin Code : " << Client.PinCode << endl;
    cout << "Client Name : " << Client.Name << endl;
    cout << "Phone Number : " << Client.PhoneNum << endl;
    cout << "Account Balance : " << Client.AccountBalance;
    cout << "\n-----------------------------------------------------\n\n";
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNum;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.PhoneNum;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector <sClient>& myClients)
{
    cout << "\n\t\t\t\t\tClient List (" << myClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : myClients)
    {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}


void AddClientLineToFile(string ClientLine, const string& filename) {
    fstream myFile;
    myFile.open(filename, ios::out | ios::app);
    if (myFile.is_open()) {
        myFile << ClientLine << endl;
        cout << "\nClient successfully Added\n";
        myFile.close();
    }
}

void AddNewClient(vector <sClient>& myClients) {
    sClient newClient;
    newClient = fillClientInfo(myClients);
    AddClientLineToFile(recordToLine(newClient), filename);
}

vector<sClient> loadDataFromFile(const string& filename) {
    vector<sClient> vClients;
    ifstream myFile(filename);
    if (myFile.is_open()) {
        string line;
        while (getline(myFile, line)) {
            vClients.push_back(lineToRecord(line));
        }

        myFile.close();
    }

    return vClients;
}

void AddClients(vector<sClient>& myClients) {
    int addMore = 0;
    cout << "\n-----------------------------------------------------\n";
    cout << "\t\t Add New Clients Screen";
    cout << "\n-----------------------------------------------------\n\n";
    cout << "Adding New Clients : \n\n";
    do {
        AddNewClient(myClients);
        myClients = loadDataFromFile(filename);
        cout << "\nDo you want to add more clients ? Yes[1] / No[0] : ";
        cin >> addMore;

    } while (addMore == 1);
}

int searchByAccNum(vector<sClient>& myClients, string AccountNum) {
    int len = myClients.size();
    for (int i = 0; i < len; i++) {
        if (AccountNum == myClients[i].AccountNum) {
            return i;
            break;
        }
    }

    return -1;
}

void findClient(vector<sClient>& myClients) {
    string AccountNum = inputInfo("Enter Account Num : ");
    int Account = searchByAccNum(myClients, AccountNum);
    if (Account == -1) {
        cout << "\nAccount Number (" << AccountNum << ") Not Found\n";
    }
    else {
        printClientInfo(myClients[Account]);
    }
}

void saveClientsToFile(vector<sClient>& myClients, const string& filename) {
    ofstream myFile(filename, ios::out);
    if (myFile.is_open()) {
        for (sClient& client : myClients) {
            myFile << recordToLine(client) << endl;
        }
        myFile.close();
    }
}


void deleteClient(vector<sClient>& myClients) {
    string AccountNum = inputInfo("Enter Account Num : ");
    int Account = searchByAccNum(myClients, AccountNum);
    if (Account == -1) {
        cout << "\nAccount Number (" << AccountNum << ") Not Found\n";
    }
    else {
        int del = 0;
        printClientInfo(myClients[Account]);
        cout << "Are you sure you want to delete this client? Yes[1] / No[0] : ";
        cin >> del;
        if (del == 1) {
            myClients.erase(myClients.begin() + Account);
            saveClientsToFile(myClients, filename);
            cout << "\nClient Account Deleted Successfully\n";


        }
    }
}

void updateClient(vector<sClient>& myClients) {
    string AccountNum = inputInfo("Enter Account Num : ");
    int Account = searchByAccNum(myClients, AccountNum);
    if (Account == -1) {
        cout << "\nAccount Number (" << AccountNum << ") Not Found\n";
    }
    else {
        printClientInfo(myClients[Account]);
        myClients[Account].PinCode = inputInfo("Enter Pin Code : ");
        myClients[Account].Name = inputInfo("Enter Client Name : ");
        myClients[Account].PhoneNum = inputInfo("Enter Phone Number : ");
        myClients[Account].AccountBalance = stoi(inputInfo("Enter Account Balance : "));

        cout << "\nClient Account Updated Successfully\n";
        saveClientsToFile(myClients, filename);
    }


}

short actionChoice(short from, short to) {
    short choice;
    cout << "Choose What Do you want To do ? [" << from << " to " << to << "] : ";
    cin >> choice;
    while (choice > 7 or choice < 1) {
        cout << "Invalid Input, Enter A number between" << from << " to " << to << " : ";
        cin >> choice;
    }

    return choice;
}

void BackToMainMenu(vector <sClient>& myClients);
void BackToTransMenu(vector <sClient>& myClients);
void transactionMenu(vector <sClient>& myClients);

void dipositAction(vector <sClient>& myClients) {
    string AccountNum = inputInfo("Enter Account Num : ");
    int Account = searchByAccNum(myClients, AccountNum);
    if (Account == -1) {
        cout << "\nAccount Number (" << AccountNum << ") Not Found\n";
    }
    else {
        printClientInfo(myClients[Account]);
        int amount, continu;
        cout << "Please enter diposit amount : ";
        cin >> amount;
        

        cout << "Are you sure you want to perform this transaction ? Yes[1]/No[0] : ";
        cin >> continu;
        if (continu == 1) {
            myClients[Account].AccountBalance += amount;
            saveClientsToFile(myClients, filename);
            cout << "\nClient Account Updated Successfully\n";
            cout << "New Balance : " << myClients[Account].AccountBalance;
            
        }
    }
}

void endScreen() {
    cout << "\n=================== END SCREEN ====================\n";
    cout << "\n========== THANK YOU FOR USING OUR BANK ===========\n\n\n";
}

void excuteMainAcions(int choice, vector <sClient>& myClients) {
    switch (choice)
    {
    case Actions::SHOW:
        system("cls");
        PrintAllClientsData(myClients);
        BackToMainMenu(myClients);
        break;
    case Actions::ADD:
        system("cls");
        AddClients(myClients);
        BackToMainMenu(myClients);
        break;
    case Actions::DELETE:
        system("cls");
        deleteClient(myClients);
        BackToMainMenu(myClients);
        break;
    case Actions::UPDATE:
        system("cls");
        updateClient(myClients);
        BackToMainMenu(myClients);
        break;
    case Actions::FIND:
        system("cls");
        findClient(myClients);
        BackToMainMenu(myClients);
        break;
    case Actions::TRANSACTION:
        system("cls");
        transactionMenu(myClients);
        BackToTransMenu(myClients);
        break;
    case Actions::EXIT:
        system("cls");
        endScreen();
        break;
    }
}

void menuScreen(vector <sClient>& myClients) {
    cout << "\n==========================================\n";
    cout << "\t\tMenu Screen";
    cout << "\n==========================================\n";
    cout << "\n\t[1] Show Client List.";
    cout << "\n\t[2] Add New Client.";
    cout << "\n\t[3] Delete Client.";
    cout << "\n\t[4] Update Client Info.";
    cout << "\n\t[5] Find Client.";
    cout << "\n\t[6] Transaction.";
    cout << "\n\t[7] Exit.\n";
    cout << "\n==========================================\n";
    excuteMainAcions(actionChoice(1 , 7), myClients);

}

void TotalBalance(vector <sClient>& myClients) {
    int totalBalance = 0;
    cout << "\n\t\t\t\t\tClient List (" << myClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : myClients)
    {
        cout << "| " << setw(15) << left << Client.AccountNum;
        cout << "| " << setw(40) << left << Client.Name;
        cout << "| " << setw(12) << left << Client.AccountBalance;
        totalBalance = totalBalance + Client.AccountBalance;
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\tTotal Balances = " << totalBalance << endl;
}

void withdrawAction(vector <sClient>& myClients) {
    string AccountNum = inputInfo("Enter Account Num : ");
    int Account = searchByAccNum(myClients, AccountNum);
    if (Account == -1) {
        cout << "\nAccount Number (" << AccountNum << ") Not Found\n";
    }
    else {
        printClientInfo(myClients[Account]);
        int AccountBalance = myClients[Account].AccountBalance;
        int amount, continu;
        cout << "Please enter withdraw amount : ";
        do {
            
            cin >> amount;
            if (amount > AccountBalance) {
                cout << "Amount Exceeds the balance you can withdraw up to " << AccountBalance << endl;
                cout << "Please enter a valid withdraw amount : ";
            }
        } while (amount > AccountBalance);
        
        

        cout << "Are you sure you want to perform this transaction ? Yes[1]/No[0] : ";
        cin >> continu;
        if (continu == 1) {
            myClients[Account].AccountBalance -= amount;
            saveClientsToFile(myClients, filename);
            cout << "\nClient Account Updated Successfully\n";
            cout << "New Balance : " << myClients[Account].AccountBalance;

        }
    }
}

void excuteTransAcions(int choice, vector <sClient>& myClients) {
    switch (choice) {
    case Transaction::DIPOSIT:
        system("cls");
        dipositAction(myClients);
        BackToTransMenu(myClients);
        break;
    case Transaction::WITHDRAW:
        system("cls");
        withdrawAction(myClients);
        BackToTransMenu(myClients);
        break;
    case Transaction::TOTAL_BALANCE:
        system("cls");
        TotalBalance(myClients);
        BackToTransMenu(myClients);
        break;
    case Transaction::MAIN_MENU:
        system("cls");
        menuScreen(myClients);
        break;
    }
}

void transactionMenu(vector <sClient>& myClients) {
    cout << "\n==========================================\n";
    cout << "\t    Transaction Screen";
    cout << "\n==========================================\n";
    cout << "\n\t[1] Deposit.";
    cout << "\n\t[2] Withdraw.";
    cout << "\n\t[3] Total Balances.";
    cout << "\n\t[4] Main Menu.";
    cout << "\n\n==========================================\n";
    excuteTransAcions(actionChoice(1, 4), myClients);

}

void BackToMainMenu(vector <sClient>& myClients) {
    cout << "\n\n";
    system("pause");
    system("cls");
    menuScreen(myClients);
}

void BackToTransMenu(vector <sClient>& myClients) {
    cout << "\n\n";
    system("pause");
    system("cls");
    transactionMenu(myClients);
}





int main()
{
    // AddClients();
    vector<sClient> myClients = loadDataFromFile(filename);
    // PrintAllClientsData(myClients);
    // AddClients(myClients);
    // PrintAllClientsData(myClients);
    menuScreen(myClients);
}
