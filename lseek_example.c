/**
 * Description: // C program to read nth byte of a file and
		// copy it to another file using lseek
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/18/2023
 * Creation date: 03/18/2023
 **/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void func(char arr[], int n)
{
    // Open the file for READ only.
    int f_read = open("start.txt", O_RDONLY);

    // Open the file for WRITE and READ only.
    int f_write = open("end.txt", O_WRONLY | O_CREAT, 0777);

    int count = 0;
    while (read(f_read, arr, 1))
    {
        // to write the 1st byte of the input file in
        // the output file
        if (count < n)
        {
            // SEEK_CUR specifies that
            // the offset provided is relative to the
            // current file position
            lseek (f_read, n, SEEK_CUR);
            write (f_write, arr, 1);
            count = n;
        }

            // After the nth byte (now taking the alternate
            // nth byte)
        else
        {
            count = n; 
            lseek(f_read, count, SEEK_CUR);
            write(f_write, arr, 1);
        }
    }
    close(f_write);
    close(f_read);
}

// Driver code
int main()
{
    char arr[100];
    int n;
    n = 2;
    // Calling for the function
    func(arr, n);
    return 0;
}
