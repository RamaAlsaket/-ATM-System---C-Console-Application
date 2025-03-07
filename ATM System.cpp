#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
const string bankFile = "bankFile.txt";
const string UsersFileName = "Users.txt";
enum enClinetChoice {
    eQucikWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3,
    eCheckBalance = 4, eExit = 5
};
struct clinetData {
    string accNumber;
    string pinCode;
    string name;
    string mobileNumber;
    double balance;
    bool markToDelete = false;
};
void menu();
clinetData currentClint;
void menu();
void login();
void ShowQuickWithdrawScreen();
void ShowNormalWithDrawScreen();
void ShowDepositScreen();
void ShowCheckBalanceScreen();
vector <string> splitString(string s1, string dellem) {
    vector <string> vString;
    int pos = 0;
    string word;
    while ((pos = s1.find(dellem)) != std::string::npos)
    {
        word = s1.substr(0, pos);
        if (word != "") {
            vString.push_back(word);
        }
        s1.erase(0, pos + dellem.length());
    }
    if (s1 != "") {
        vString.push_back(s1);
    }
    return vString;
}
clinetData converitLineToRecord(string line, string seperater = "#//#") {
    clinetData client;
    vector <string> vClinetData;
    vClinetData = splitString(line, seperater);
    client.accNumber = vClinetData[0];
    client.pinCode = vClinetData[1];
    client.name = vClinetData[2];
    client.mobileNumber = vClinetData[3];
    client.balance = stod(vClinetData[4]);
    return client;
}
string convertRecordToLine(clinetData client, string seperater = "#//#") {
    string StClient = "";
    StClient += client.accNumber + seperater;
    StClient += client.pinCode + seperater;
    StClient += client.name + seperater;
    StClient += client.mobileNumber + seperater;
    StClient += to_string(client.balance);
    return StClient;
}
vector <clinetData> loadClientDataFile(string filename) {
    vector <clinetData> veClient;
    fstream myfile;
    myfile.open(filename, ios::in);
    if (myfile.is_open()) {
        string line;
        clinetData sClient;
        while (getline(myfile, line)){
            sClient = converitLineToRecord(line);
            veClient.push_back(sClient);
        }
        myfile.close();
    }
    return veClient;
}
bool findClintByAccountNumberAndPassword(string accountNumber, string pass, clinetData &client) {
    vector <clinetData> veClient = loadClientDataFile(bankFile);
    for (clinetData C : veClient) {
        if (C.accNumber == accountNumber && C.pinCode == pass) {
            client = C;
            return true;
        }
    }
    return false;
}
bool loadClintFile(string username, string pass, clinetData& clint = currentClint) {

    if (findClintByAccountNumberAndPassword(username, pass, currentClint)) {
        return true;
    }
    else {
        return false;
    }

}
vector <clinetData> saveClientData(string fileName, vector<clinetData> vClient) {
    fstream myfile;
    myfile.open(fileName, ios::out);
    string dataLine;
    if (myfile.is_open()) {
        for(clinetData C : vClient) {
            if (C.markToDelete == false) {
                dataLine = convertRecordToLine(C);
                myfile << dataLine << endl;
            }
        }
        myfile.close();
    }
    return vClient;
}
bool depositBalanceToClientByAccountNumber(string accNumber, double amount, vector<clinetData> vClient) {
    char answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        for (clinetData& C : vClient) {
            if (C.accNumber == accNumber) {
                C.balance += amount;
                saveClientData(bankFile, vClient);
                cout << "\n\nDone Successfully. New balance is: " << C.balance;
                return true;
            }
        }
        return false;
    }
}
int readQuickWithDrawOption() {
    int choice = 0;
    while (choice < 1 || choice > 9) {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> choice;
    }
    return choice;
}
int getQuickWithDrawAmount(int withDrawOption) {
    switch (withDrawOption)
    {
    case 1:
        return  20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}
void PerfromQuickWithdrawOption(int option) {
    if (option == 9) {
        return;
    }
    int withDrawBalance = getQuickWithDrawAmount(option);
    if (withDrawBalance == currentClint.balance) {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }
    vector <clinetData> vClient = loadClientDataFile(bankFile);
    depositBalanceToClientByAccountNumber(currentClint.accNumber, withDrawBalance * -1, vClient);
    currentClint.balance -= withDrawBalance;
}
double readOptionAmount() {
    double Amount;
    cout << "\nEnter a positive Deposit Amount? ";

    cin >> Amount;
    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    }
    return Amount;
}
void PerfromDepositOption() {
    double depositAmount = readOptionAmount();
    vector <clinetData> vClient = loadClientDataFile(bankFile);
    depositBalanceToClientByAccountNumber(currentClint.accNumber, depositAmount, vClient);
    currentClint.accNumber += depositAmount;
}
int readWithDrawAmount() {
    int amount;
    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> amount;
    while (amount % 5 != 0) {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> amount;
    }
    return amount;
}
void PerfromNormalWithdrawOption() {
    int withDrawBalance = readWithDrawAmount();
    if (withDrawBalance > currentClint.balance) {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithDrawScreen();
        return;
    }
    vector <clinetData> vClient = loadClientDataFile(bankFile);
    depositBalanceToClientByAccountNumber(currentClint.accNumber, withDrawBalance * -1, vClient);
    currentClint.balance -= withDrawBalance;
}
void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    PerfromDepositOption();

}
void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << currentClint.balance << "\n";

}
void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    PerfromNormalWithdrawOption();
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << currentClint.balance;

    PerfromQuickWithdrawOption(readQuickWithDrawOption());
}
int readInfo() {
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}
void GoBackToMainMenue() {
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    menu();
}
void performClintNeed(enClinetChoice choice) {
    switch (choice) {
    case enClinetChoice::eQucikWithdraw: 
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enClinetChoice::eNormalWithDraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;

    }
    case enClinetChoice::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;

    }
    case enClinetChoice::eCheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;

    }
    
    case enClinetChoice::eExit:
    {
        system("cls");
        return;

    }
    }
}
void menu() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    performClintNeed((enClinetChoice)readInfo());
}
void login() {
    bool loginFailed = false;
    string accountNumber, pass;

    do {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (loginFailed) {
            cout << "Invalid Account Number/Password!\n";
        }

        cout << "Enter Account Number? ";
        cin >> ws;
        getline(cin, accountNumber);

        cout << "Enter Password? ";
        getline(cin, pass);

        loginFailed = !loadClintFile(accountNumber, pass);

    } while (loginFailed);

    menu();
}
int main()
{
	login();
	system("cls");
	return 0;
}