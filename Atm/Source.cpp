#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
enum enTransactionsMenueOptions { eQuickWithdraw = 1, eNormalWithdraw = 2, eDepoist = 3, eCheckBalance = 4 ,eLogout = 5 };
enum enQuickWithdrawScreen {e20=20,e50=50,e100=100,e200=200,e400=400,e600=600,e800=800,e1000=1000,eExit=9};

const string ClientsFileName = "Clients.txt";

void Login();
void ShowAtmMainMenueScreen();
void ShowQuickWithdrawMenu();


struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrnetClient;
void SaveClientChanges(const sClient& UpdatedClient);
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByUsernameAndPincode(string Username, string Password, sClient& User)
{

    vector <sClient> vClientss = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient U : vClientss)
    {

        if (U.AccountNumber == Username && U.PinCode == Password)
        {
            User = U;
            return true;
        }

    }
    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

bool DepositBalanceToClientByAccountNumber( double Amount, sClient &CurrentClient)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
      
       CurrentClient.AccountBalance += Amount; 
      
      string Line = ConvertRecordToLine(CurrentClient);
      AddDataLineToFile(ClientsFileName, Line);
      vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
      SaveClientChanges(CurrentClient);
       cout << "\n\nDone Successfully. New balance is: " << CurrentClient.AccountBalance;
      
       return true;
         
    }
    return false;

    
}

void ShowNormalWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\t Normal Withdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    


    
    

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    
    //Validate that the amount does not exceeds the balance
    while (Amount > CurrnetClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrnetClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber( Amount * -1, CurrnetClient);
    
}

void ShowDepoistScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\t Depoist Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    double Amount = 0;
    cout << "\nPlease enter Depoist amount? ";
    cin >> Amount;


    //Validate that the amount does not exceeds the balance
    

    DepositBalanceToClientByAccountNumber(Amount , CurrnetClient);
    //chaange
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadQuickWithdrawMenuOptions()
{
    cout << "Choose what do you want to do? [1 to 9]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void GoBackToAtmMainMenu()
{
    cout << "\n\nPress any key to go back to Atm Main Menu...";
    system("pause>0");
    ShowAtmMainMenueScreen();
}

short QuickWithdrawAmount(short  QuickWithdrawAmount)
{
   

   
    switch (QuickWithdrawAmount)
    {
    case 1:
    {
       return 20;
    }

    case 2:
    {
        return 50;
    }

    case 3:
    {
        return 100;
    }
    case 4:
    {
        return 200;
    }
    case 5:
    {
        return 400;
    }
    case 6:
    {
        return 600;
    }
    case 7:
    {
        return 800;
    }
    case 8:
    {
        return 1000;
    }
    
    }
   
    
}
void PerformQuickWithdrawMenuOption(short QuickWithdrawAmountt)
{
    if (QuickWithdrawAmountt == 9) return;
    short WithdrawAmount = QuickWithdrawAmount(QuickWithdrawAmountt);
    if (WithdrawAmount > CurrnetClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
            cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawMenu();
    }
    CurrnetClient.AccountBalance -= WithdrawAmount;
    cout << "\n\nDone Successfully. New balance is: " << CurrnetClient.AccountBalance;
    string Line = ConvertRecordToLine(CurrnetClient);
    AddDataLineToFile(ClientsFileName, Line);
    SaveClientChanges(CurrnetClient);
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
}

void ShowQuickWithdrawMenu()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tQuick Withdraw Screen";
    cout << "\n-----------------------------------\n";
    cout << "[1] 20 \t[2] 50\n";
    cout << "[3] 100\t[4] 200\n";
    cout << "[5] 400\t[6] 600\n";
    cout << "[7] 800\t[8] 1000\n";
    cout << "[9] Exit\n";
    cout << "\n\nDone Successfully. New balance is: " << CurrnetClient.AccountBalance;
    PerformQuickWithdrawMenuOption((enQuickWithdrawScreen)ReadQuickWithdrawMenuOptions());
}
void ShowCheckBalanceScreen()
{
    system("cls");

    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "Your Total Balance is : "<< CurrnetClient.AccountBalance;
}
void PerfromAtmMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawMenu();
        GoBackToAtmMainMenu();
        break;
    }

    case enTransactionsMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToAtmMainMenu();
        break;
    }


    case enTransactionsMenueOptions::eDepoist:
    {
        system("cls");
        ShowDepoistScreen();
        GoBackToAtmMainMenu();
        break;
    }


    case enTransactionsMenueOptions::eCheckBalance:
    {

        ShowCheckBalanceScreen();
        GoBackToAtmMainMenu();
        break;

    }
    case enTransactionsMenueOptions::eLogout:
    {
        Login();
    }

    }

}

void ShowAtmMainMenueScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tAtm Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Witdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Depiost.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] LogOut.\n";
    cout << "===========================================\n";
    PerfromAtmMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

bool  LoadUserInfo(string Username, string Password)
{

    if (FindClientByUsernameAndPincode(Username, Password, CurrnetClient))
        return true;
    else
        return false;

}

void Login()
{
    bool LoginFaild = false;

    string Username, Password;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowAtmMainMenueScreen();

}
void SaveClientChanges(const sClient& UpdatedClient) {
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient& Client : vClients) {
        if (Client.AccountNumber == UpdatedClient.AccountNumber) {
            Client.AccountBalance = UpdatedClient.AccountBalance;
        }
    }

    // Rewrite the file with updated data
    SaveCleintsDataToFile(ClientsFileName, vClients);
}


int main()

{
    Login();

    system("pause>0");
    return 0;
}