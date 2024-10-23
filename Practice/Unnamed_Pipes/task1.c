#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
   const char input_file[] = "inputfile.txt";
   const char output_file[] = "outputfile.txt";


   // Open input file
   int input_fd = open(input_file, O_RDONLY);
   if (input_fd == -1) {
       perror("Error opening input file");
       exit(EXIT_FAILURE);
   }


   // Read contents into buffer
   char buffer[1024];
   ssize_t bytes_read = read(input_fd, buffer, 1024);
   if (bytes_read == -1) {
       perror("Error reading from input file");
       close(input_fd);
       exit(EXIT_FAILURE);
   }


   // Close the input file
   close(input_fd);


   // Open output file
   int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (output_fd == -1) {
       perror("Error opening/creating output file");
       exit(EXIT_FAILURE);
   }


   // Write buffered contents to output file
   ssize_t bytes_written = write(output_fd, buffer, bytes_read);
   if (bytes_written == -1) {
       perror("Error writing to output file");
       close(output_fd);
       exit(EXIT_FAILURE);
   }


   // Close the output file
   close(output_fd);


   return EXIT_SUCCESS;
}
