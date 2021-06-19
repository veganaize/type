#include <windows.h>

#define OUTPUT(out, wbuf, cbuf) \
        ( WriteConsoleW((out), (wbuf), wcslen((wbuf)), NULL, NULL) \
        || WideCharToMultiByte(CP_OEMCP, 0, (wbuf), -1, (cbuf), sizeof (cbuf), NULL, NULL) \
        && WriteFile((out), (cbuf), strlen((cbuf)), NULL, NULL) ); \
        FlushFileBuffers((out))

int main() {

    INT     i, argcw;
    CHAR    cbuffer[8192];
    WCHAR   wbuffer[8192];
    DWORD   dwByteCount;
    HANDLE  hFile;
    HANDLE  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE  hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    LPWSTR  *argvw = CommandLineToArgvW(GetCommandLineW(), &argcw);

    /* No arguments */
    if (argcw < 2) {
        OUTPUT(hStdErr, L"The syntax of the command is incorrect.\r\n", cbuffer);
        return 1;
    }

    /* /? switch */
    for (i = 1; i < argcw; i++) {
        if (argvw[i][0] == '/' && argvw[i][1] == '?') {
            OUTPUT(hStdOut, L"Displays the contents of a text file or files.\r\n\r\n"
                            L"TYPE [drive:][path]filename\r\n", cbuffer);
            return 1;
        }
    }

    /* Bogus switches */
    for (i = 1; i < argcw; i++) {
        if (argvw[i][0] == '/') {
            OUTPUT(hStdErr, L"The syntax of the command is incorrect.\r\n", cbuffer);
            return 1;
        }
    }

    /* Process files */
    for (i = 1; i < argcw; i++) {

        /* Attempt to open (wide-character name) file */
        if ((hFile = CreateFileW(argvw[i],
                                 GENERIC_READ,
                                 FILE_SHARE_READ,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL)
                ) == INVALID_HANDLE_VALUE) {
            OUTPUT(hStdErr, L"The system cannot find the file specified.\r\n", cbuffer);
            /* Specify file when multiple passed in */
            if (argcw > 2) {
                OUTPUT(hStdErr, L"Error occurred while processing: ", cbuffer);
                OUTPUT(hStdErr, argvw[i], cbuffer);
                OUTPUT(hStdErr, L".\r\n", cbuffer);
            }
            continue;
        }

        /* Display filename, and blank lines, when there's multiple files */
        if (argcw > 2) {
            OUTPUT(hStdErr, L"\r\n", cbuffer);
            OUTPUT(hStdErr, argvw[i], cbuffer);
            OUTPUT(hStdErr, L"\r\n\r\n\r\n", cbuffer);
        }

        /* Check byte-order mark for UTF-16 [little-endian] encoding */
        ReadFile(hFile, wbuffer, sizeof (wchar_t), &dwByteCount, NULL);
        /* UTF16le encoding ? */
        if (wbuffer[0] == 0xFEFF) {
            while ((ReadFile(hFile, wbuffer, sizeof wbuffer/2, &dwByteCount, NULL), dwByteCount)) {
                OUTPUT(hStdOut, wbuffer, cbuffer);
            }

        /* ASCII encoding ? */
        } else {
            /* Rewind file pointer */
            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
            while((ReadFile(hFile, cbuffer, sizeof cbuffer, &dwByteCount, NULL)), dwByteCount) {
                WriteFile(hStdOut, cbuffer, dwByteCount, NULL, NULL);
                FlushFileBuffers(hStdOut);
            }
        }

        /* Close file */
        CloseHandle(hFile);
    }

    return 0;
}