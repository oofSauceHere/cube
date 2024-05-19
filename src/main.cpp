#include "matrixFunctions.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

void storeMatrix(unordered_map<string, Matrix> &store, string &name, Matrix mat)
{
    if (store.find(name) != store.end())
    {
        cout << "A matrix with that name already exists! Do you want to overwrite it? (Y/n): ";
        string ans;
        cin >> ans;
        if (ans == "Y")
        {
            store.erase(name);
            store.insert(make_pair(name, mat));
        }
        cout << "\n";
    }
    else
        // note that creating a matrix and moving it to the store is slow.
        // pointers may be better, but matrices created in this if statement
        // will go out of scope, so we'd need to dynamically allocate.
        store.insert(make_pair(name, mat));
}

int main()
{
    unordered_map<string, Matrix> store;
    cout << "Enter a command: \n";

    string com;
    while (1)
    {
        cin.clear();

        cout << "> ";
        cin >> com;
        cout << "\n";

        if (com == "help")
        {
            cout << "wip !!\n";
        }
        else if (com == "crt" || com == "create")
        {
            string name;
            cin >> name;

            if (!cin.fail())
            {
                storeMatrix(store, name, readMatrix());
            }
        }
        else if (com == "prt" || com == "print")
        {
            string name;
            cin >> name;

            if (!cin.fail())
            {
                cout << store.at(name) << "\n";
            }
        }
        else if (com == "add")
        {
            string line;
            getline(cin, line);
            stringstream ss(line);

            // the reason this is nested is because matrices A and B will go out of scope otherwise.
            // i tried to use pointers to keep the code cleaner, but it didnt work. i guess there's
            // something im missing.
            string name;
            if (ss >> name)
            {
                Matrix &A = store.at(name);
                if (ss >> name)
                {
                    Matrix &B = store.at(name);
                    Matrix C = A + B;
                    cout << C << "\n";

                    if (!ss.eof())
                    {
                        ss >> name;
                        if (!ss.fail() && name == "->")
                        {
                            if (ss >> name)
                                storeMatrix(store, name, C);
                            else
                                cout << "invalid command\n";
                        }
                    }
                }
                else
                    cout << "invalid command\n";
            }
            else
                cout << "invalid command\n";
        }
        else if (com == "inv" || com == "invert")
        {
            string line;
            getline(cin, line);
            stringstream ss(line);

            string name;
            if (ss >> name)
            {
                Matrix B = store.at(name).inverse(); // uh oh
                cout << B << endl;

                if (!ss.eof())
                {
                    ss >> name;
                    if (!ss.fail() && name == "->")
                    {
                        if (ss >> name)
                            storeMatrix(store, name, B);
                        else
                            cout << "invalid command\n";
                    }
                }
            }
            else
                cout << "invalid command\n";
        }
        else if (com == "ls" || com == "list")
        {
            cout << "[ ";
            for (pair<string, Matrix> elem : store)
                cout << elem.first << " ";
            cout << "]\n\n";
        }
        else
        {
            cout << "quitting ...\n";
            return 0;
        }
    }

    return 0;
}