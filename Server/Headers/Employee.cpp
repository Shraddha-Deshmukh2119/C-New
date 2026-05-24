#include "Employee.h"
#include "Complaint_E.h"

void Employee::home()
{

    bool go = true;
    int ans;
    string str;

    while(go)
    {
        (*server).Rec(str);
        ans = stoi(str);

        switch (ans)
        {
        case 1:
            buy(empFile);
            break;
        
        case 2:
            complain();
            break;
        
        case 3:
            profile(empFile, this->lookup);
            break;
            
        case 4:
            go = false;
            break;
    
        default:
            cout << " Invalid choice!";
            cout << " Press any key to go enter again...\n";
            getch();
            break;
        }
    }
}

void Employee::complain()
{
    Complaint_E complaint("complaint.dat");
    bool Go = true;
    int ans;
    string str;

    while(Go)
    {
        (*server).Rec(str);
        ans = stoi(str);

        switch (ans)
        {
        case 1:
            {
                char name[500];
                Complaint readIn;

                (*server).Rec(str);
                strcpy(name, str.c_str());
                ostringstream ss;

                if (complaint.see(readIn, name, true))
                {
                    (*server).Send("TRUE");
                    complaint.Complaint_Base::view(readIn, ss);
                    (*server).Send(ss.str());
                }
                else
                    (*server).Send("FALSE");
            }
            break;
        
        case 2:
            {
                char name[500];
                Complaint readIn;

                (*server).Rec(str);
                strcpy(name, str.c_str());
                ostringstream ss;

                if (complaint.see(readIn, name, false))
                {
                    (*server).Send("TRUE");
                    complaint.Complaint_Base::view(readIn, ss);
                    (*server).Send(ss.str());
                }
                else
                    (*server).Send("FALSE");
            }
            break;
        
        case 3:
            {
                char name[500], ans[500];

                (*server).Rec(str);
                strcpy(name, str.c_str());
                (*server).Rec(str);
                strcpy(ans, str.c_str());

                if (complaint.answer(ans, name))
                    (*server).Send("TRUE");
                else
                    (*server).Send("FALSE");
            }
            break;
        
        case 4:
        {
            ostringstream ss;
            complaint.view(false, ss);
            (*server).Send(ss.str());
            break;
        }
        
        case 5:
        {
            ostringstream ss;
            complaint.view(true, ss);
            (*server).Send(ss.str());
            break;
        }
        
        case 6:
            complaint.clear();
            break;
        
        case 7:
            Go = false;
            break;
        
        default:
            cerr << " Invalid input!\n";
            fflush(stdin);
            getch();
            break;
        }
    }
}
