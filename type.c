#include <stdio.h>
#include <windows.h>

int main(int cmdc, char *cmda[]) {

    int i;
    HANDLE  hFile;
    DWORD   dwBytesRead;
    wchar_t buffer[2048];
    char    cBuffer[2048];
    HANDLE  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int     argcw;
    LPWSTR  *argvw = CommandLineToArgvW(GetCommandLineW(), &argcw);

    /* No arguments */
    if (argcw < 2) {
        fputs("The syntax of the command is incorrect.\n", stderr);
        return 1;
    }

    /* /? switch */
    for (i = 1; i < cmdc; i++) {
        if (cmda[i][0] == '/' && cmda[i][1] == '?') {
            puts("Displays the contents of a text file or files.\n\n"
                 "TYPE [drive:][path]filename");
            return 1;
        }
    }

    /* Bogus switches */
    for (i = 1; i < cmdc; i++) {
        if (cmda[i][0] == '/') {
            fputs("The syntax of the command is incorrect.\n", stderr);
            return 1;
        }
    }

    /* Process files */
    for (i = 1; i < cmdc; i++) {

        /* Attempt to open (wide-character name) file */
        if ((hFile = CreateFileW(argvw[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
            fputs("The system cannot find the file specified.\n", stderr);
            /* Specify file when multiple passed in */
            if (cmdc > 2) fprintf(stderr, "Error occurred while processing: %s.\n", cmda[i]);
            fflush(stdout);
            continue;
        }

        /* Display filename, and blank lines, when there's multiple files */
        if (cmdc > 2) fprintf(stderr, "\n%s\n\n\n", cmda[i]);
        fflush(stderr);

        /* Check byte-order mark for UTF-16 [little-endian] encoding */
        ReadFile(hFile, buffer, sizeof (wchar_t), &dwBytesRead, NULL);
        /* UTF16le encoding ? */
        if (buffer[0] == 0xFEFF) {
            while ((ReadFile(hFile, buffer, sizeof buffer/2, &dwBytesRead, NULL), dwBytesRead)) {
                WriteConsoleW(hStdOut, buffer, wcslen(buffer), NULL, NULL)
                || WideCharToMultiByte(CP_OEMCP, 0, buffer, sizeof buffer/2, cBuffer, sizeof cBuffer, NULL, NULL)
                && WriteFile(hStdOut, cBuffer, dwBytesRead/2, NULL, NULL)
                && FlushFileBuffers(hStdOut);
            }

        /* ASCII encoding ? */
        } else {
            /* Rewind file pointer */
            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
            while((ReadFile(hFile, cBuffer, sizeof cBuffer, &dwBytesRead, NULL)), dwBytesRead) {
                WriteFile(hStdOut, cBuffer, dwBytesRead, NULL, NULL);
                FlushFileBuffers(hStdOut);
            }
        }

        /* Close file */
        CloseHandle(hFile);
    }

    return 0;
}