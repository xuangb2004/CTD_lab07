#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    FILE *file1 = fopen(argv[1], "rb");
    if (file1 == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }

    FILE *file2 = fopen(argv[2], "rb");
    if (file2 == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[2]);
        fclose(file1);
        return 1;
    }

    // --- 1. So sánh kích thước file (Size Comparison) ---
    fseek(file1, 0, SEEK_END);
    long size1 = ftell(file1);
    rewind(file1);

    fseek(file2, 0, SEEK_END);
    long size2 = ftell(file2);
    rewind(file2);

    if (size1 != size2) {
        printf("Files differ in size: '%s' (%ld bytes) vs '%s' (%ld bytes)\n", 
               argv[1], size1, argv[2], size2);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    // --- 2. So sánh từng byte (Byte-by-byte Comparison) ---
    int byte_count = 0;
    unsigned char byte1, byte2;
    
    while (fread(&byte1, 1, 1, file1) == 1 && fread(&byte2, 1, 1, file2) == 1) {
        if (byte1 != byte2) {
            printf("Files differ at byte %d: 0x%02X vs 0x%02X\n", byte_count, byte1, byte2);
            fclose(file1);
            fclose(file2);
            return 1;
        }
        byte_count++;
    }

    printf("Files are identical with %d bytes compared.\n", byte_count);

    fclose(file1);
    fclose(file2);
    return 0;
}