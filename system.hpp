//coded by Sud0ck3rs
#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>

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

            std::string Stdout()
            {
                return result_Shell;
            }

            void killProcessByName(const char *filename)
            {
                HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
                PROCESSENTRY32 pEntry;
                pEntry.dwSize = sizeof (pEntry);
                BOOL hRes = Process32First(hSnapShot, &pEntry);
                while (hRes)
                {
                    if (strcmp(pEntry.szExeFile, filename) == 0)
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
            System():
                Hours(0), Min(0), Sec(0), time("")
            {
                std::time( &currentTime );
                localTime = localtime( &currentTime );
            }

            ~System()
            {

            }

            void changeVolume()
            {
                INPUT ip={0};
                ip.type = INPUT_KEYBOARD;
                ip.ki.wVk = VK_VOLUME_UP;   //or VOLUME_DOWN or MUTE
                SendInput(1, &ip, sizeof(INPUT));
                ip.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &ip, sizeof(INPUT));
            }

            std::string get_OS_Time()
            {
                Hours = localTime->tm_hour;
                Min = localTime->tm_min;
                Sec = localTime->tm_sec;
                ss << Hours << ":" << Min << ":" << Sec;
                time = ss.str();
                ss.clear();
                return time;
            }

            int get_OS_Hours()
            {
                Hours = localTime->tm_hour;
                return Hours;
            }

            int get_OS_Min()
            {
                Min = localTime->tm_min;
                return Min;
            }

            int get_OS_Sec()
            {
                Sec = localTime->tm_sec;
                return Sec;
            }

        private:
            std::stringstream ss;
            std::string time;
            time_t currentTime;
            struct tm *localTime;

            int Hours;
            int Min;
            int Sec;
    };
}





#endif