//coded by Sud0ck3rs
#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>

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

        private:
            std::string cmd_Shell;
            std::string result_Shell;
            std::array<char, 128> buffer;
            bool check_Pipe;
            FILE *pipe;
    };
}

#endif
