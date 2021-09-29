#include<iostream>
#include<conio.h>
#include<string>
#include<fstream>
#include<windows.h>
#include<windowsx.h>
#include <sstream>
#include<vector>
#define UNICODE
#include "wtypes.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow1,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow1,UINT message,WPARAM wParam,LPARAM lParam);


#define createwindowbuttoninwindow1 1
#define createwindowbuttoninwindow2 2
#define ID_LOGIN 3
#define ID_FORGORPASS 4
#define ID_REGISTER 5
#define ID_USERNAMETEXTBOX 6
#define ID_PASSWORDTEXTBOX 7

HWND UsernameTextBox,PasswordTextBox;

bool login = false; 
int horizontal = 0;
int vertical = 0;

bool window1open,window2open=false;
bool windowclass1registeredbefore,windowclass2registeredbefore=false;

enum windowtoopenenumt {none,window2};

windowtoopenenumt windowtoopenenum=none;

void createwindow2(WNDCLASSEX& wc,HWND& hwnd,HINSTANCE hInst,int nShowCmd);

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

//console activation
void consoles()
{
#ifdef _WIN32
if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()){
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
}
#endif
}

//Return string according to given number delimeter
string returnString(string buffer, int num)
{
	int i,flag = 0,j;
	string upbuffer;
	cout<<buffer<<endl;
	string delimiter = "+";

	size_t pos = 0;
	string token;
	while ((pos = buffer.find(delimiter)) != std::string::npos && flag<=num) 
	{
	    token = buffer.substr(0, pos);
	    //cout << token << endl;
	    buffer.erase(0, pos + delimiter.length());
	    flag++;
	}
	return token;
}

//login LOGIC
bool userlogin(string un, string pswd)
{
	int flag = 0;
	string buffer;
	string ubuffer,pbuffer;
	string filename;
	//strcpy(filename,un);
	filename = un;
	int len = filename.length();
	filename[len] = '\0';
	filename = filename + ".txt";
	cout<<filename;
	ifstream fobj(filename.c_str());
	getline(fobj,buffer);
	fobj.close();
		ubuffer = returnString(buffer,0);
		pbuffer = returnString(buffer,1);
		cout<<un<<endl<<pswd;
		cout<<ubuffer<<endl<<pbuffer;
		int result = ubuffer.compare(un);
		int result1 = pbuffer.compare(pswd);
		cout<<result<<result1;
		if(result == 0 &&  result1 == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
}

//register a user
int registerUser(string un, string pswd)
{
	fs::create_directory(un)?
	cout<<"Successful Directory":
	cout<<"Unsuccesful Directory";

}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd)
{
	consoles();
    bool endprogram=false;
    WNDCLASSEX windowclassforwindow2;   
    HWND handleforwindow2;
	

   	GetDesktopResolution(horizontal, vertical);
   	std::cout<<horizontal<<" "<<vertical;
	
    //create window 1
    MSG msg;
    WNDCLASSEX windowclassforwindow1;
    ZeroMemory(&windowclassforwindow1,sizeof(WNDCLASSEX));
    windowclassforwindow1.cbClsExtra=NULL;
    windowclassforwindow1.cbSize=sizeof(WNDCLASSEX);
    windowclassforwindow1.cbWndExtra=NULL;
    windowclassforwindow1.hbrBackground=(HBRUSH)COLOR_WINDOW;
    windowclassforwindow1.hCursor=LoadCursor(NULL,IDC_ARROW);
    windowclassforwindow1.hIcon=NULL;
    windowclassforwindow1.hIconSm=NULL;
    windowclassforwindow1.hInstance=hInst;
    windowclassforwindow1.lpfnWndProc=(WNDPROC)windowprocessforwindow1;
    windowclassforwindow1.lpszClassName= "window class 1";
    windowclassforwindow1.lpszMenuName=NULL;
    windowclassforwindow1.style=CS_HREDRAW|CS_VREDRAW|WS_DISABLED;

    if(!RegisterClassEx(&windowclassforwindow1))
    {
        int nResult=GetLastError();
        MessageBox(NULL,
            "Window class creation failed",
            "Window Class Failed",
            MB_ICONERROR);
    }

    HWND handleforwindow1=CreateWindowEx(NULL,
            windowclassforwindow1.lpszClassName,
            "Window 1",
            WS_OVERLAPPEDWINDOW|WS_DISABLED,
            0,
            0,
            horizontal,
            vertical,
            NULL,
            NULL,
            hInst,
            NULL                /* No Window Creation data */
			);

    if(!handleforwindow1)
    {
        int nResult=GetLastError();

        MessageBox(NULL,
            "Window creation failed",
            "Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(handleforwindow1,nShowCmd);
    bool endloop=false;
    while (endloop==false)
	 {
        if (GetMessage(&msg,NULL,0,0));
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    		if(window2open == false && !login)
    		{
                createwindow2(windowclassforwindow2,handleforwindow2,hInst,nShowCmd);
                	                
				window2open = true;
			}
			else
			{
				
			}		
        windowtoopenenum=none;
        if(login)
        {
        	EnableWindow(handleforwindow1,true);
		}
    }
    if (window1open==false && window2open==false)
        endloop=true;

    
    MessageBox(NULL,
            "All Windows are closed.  Program will now close.",
            "Message",
            MB_ICONINFORMATION);

}

void createwindow2(WNDCLASSEX& wc,HWND& hwnd,HINSTANCE hInst,int nShowCmd) {
    if (windowclass2registeredbefore==false) {
    ZeroMemory(&wc,sizeof(WNDCLASSEX));
    wc.cbClsExtra=NULL;
    wc.cbSize=sizeof(WNDCLASSEX);
    wc.cbWndExtra=NULL;
    wc.hbrBackground=(HBRUSH)COLOR_WINDOW;
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hIcon=NULL;
    wc.hIconSm=NULL;
    wc.hInstance=hInst;
    wc.lpfnWndProc=(WNDPROC)windowprocessforwindow2;
    wc.lpszClassName="wc2";
    wc.lpszMenuName=NULL;
    wc.style=CS_HREDRAW|CS_VREDRAW;

    if(!RegisterClassEx(&wc))
    {
        int nResult=GetLastError();
        MessageBox(NULL,
            "Window class creation failed",
            "Window Class Failed",
            MB_ICONERROR);
    }
    else
        windowclass2registeredbefore=true;
    } 
    hwnd=CreateWindowEx(NULL,
            wc.lpszClassName,
            "Window 2",
            WS_OVERLAPPEDWINDOW,
            200,
            170,
            640,
            480,
            NULL,
            NULL,
            hInst,
            NULL                /* No Window Creation data */
);

    if(!hwnd)
    {
        int nResult=GetLastError();

        MessageBox(NULL,
            "Window creation failed",
            "Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hwnd,nShowCmd);
}


// windows process functions

LRESULT CALLBACK windowprocessforwindow1(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) 
{	

//Center positions for button
	RECT rectClient, rectWindow;
	GetClientRect(hwnd, &rectClient);
    GetWindowRect(hwnd, &rectWindow);
	int posx, posy, topx, topy;
    	posx = horizontal/2;
    	posy = vertical/2;
    	topx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
    	topy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;

    switch(message) {
        case WM_CREATE:
                window1open=true;
                CreateWindowEx(NULL,
                "BUTTON",
                "Exit",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                posx,
                posy,
                150,
                24,
                hwnd,
                (HMENU)createwindowbuttoninwindow1,
                GetModuleHandle(NULL),
                NULL);
            break;
            case WM_DESTROY:
                window1open=false;
                break;
        case WM_COMMAND:
            switch LOWORD(wParam) 
			{
                case createwindowbuttoninwindow1:
                    exit(0);
                    break;
            }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);

}

LRESULT CALLBACK windowprocessforwindow2(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) 
{
	string un;
	string pswd;
//Center positions for button
	RECT rectClient, rectWindow;
	GetClientRect(hwnd, &rectClient);
    GetWindowRect(hwnd, &rectWindow);
	int posx, posy, topx, topy;
    	posx = 250;
    	posy = 240;
    	topx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
    	topy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;	
	
    switch(message) 
	{
        case WM_CREATE:
                window2open=true;
                CreateWindowEx(NULL,
                "BUTTON",
                "Exit",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                posx+160,
                posy,
                150,
                24,
                hwnd,
                (HMENU)createwindowbuttoninwindow2,
                GetModuleHandle(NULL),
                NULL);
                
                //Login Button
                  CreateWindowEx(NULL,
                "BUTTON",
                "Login",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                posx-160,
                posy,
                150,
                24,
                hwnd,
                (HMENU)ID_LOGIN,
                GetModuleHandle(NULL),
                NULL);
                
                //Register Button
                  CreateWindowEx(NULL,
                "BUTTON",
                "Register",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                posx,
                posy+30,
                150,
                24,
                hwnd,
                (HMENU)ID_REGISTER,
                GetModuleHandle(NULL),
                NULL);               
                
                //Forgot Password
                  CreateWindowEx(NULL,
                "BUTTON",
                "Forgot Password",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                posx,
                posy,
                150,
                24,
                hwnd,
                (HMENU)ID_FORGORPASS,
                GetModuleHandle(NULL),
                NULL); 
				
				//Username TextBox
				UsernameTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Username"),
							WS_VISIBLE | WS_CHILD | WS_BORDER,
							posx, posy-150 ,200, 25, 
							hwnd, (HMENU) 101, 
							(HINSTANCE) NULL, NULL
							);
							
				//password TEXTBOX
				PasswordTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Passsword"),
							WS_VISIBLE | WS_CHILD | WS_BORDER,
							posx, posy-50 ,200, 25, 
							hwnd, (HMENU) 101, 
							(HINSTANCE) NULL, NULL
							);				               
            break;
        case WM_DESTROY:
            	//exit(0);
            		DestroyWindow(hwnd);
                window2open=false;
                break;
        case WM_COMMAND:
            switch LOWORD(wParam) 
			{
				case ID_REGISTER:
						registerUser(un,pswd);
					break;
				//opening adn closing window 2 logic
                case createwindowbuttoninwindow2:
                    //windowtoopenenum=window3;
					
	    		MessageBox(NULL,
	            "All Windows are closed.  Program will now close.",
	            "Message",
	            MB_ICONINFORMATION);
                    exit(0);
                    break;
                    
                //login case
                case ID_LOGIN:
                	//Get usernmae
    				//string un; // The data the user has typed will be stored here
					un.resize( GetWindowTextLength( UsernameTextBox ) + 1,'\0' ); // resize the string so iit can contain the text stored in the edit-control.
					GetWindowText(UsernameTextBox, LPSTR(un.c_str()), GetWindowTextLength(UsernameTextBox) + 1); // Getting the data the user typed*/
					un.resize(un.length()-1);
					cout<<un;
					
					//Get password
    				//string pswd; // The data the user has typed will be stored here
					pswd.resize( GetWindowTextLength( PasswordTextBox ) + 1,'\0' ); // resize the string so iit can contain the text stored in the edit-control.
					GetWindowText(PasswordTextBox, LPSTR(pswd.c_str()), GetWindowTextLength(PasswordTextBox) + 1); // Getting the data the user typed
					pswd.resize(pswd.length()-1);
					cout<<pswd;
					
					//if logging in or not
                	login = userlogin(un,pswd);
                	if(login == true)
                	{
                		DestroyWindow(hwnd);
					}
                	break;
    		}
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
