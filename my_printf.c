#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// binary decimal octal hexadecimal look in to it.

int print_octal(unsigned int arg){
    int num_printed = 0;
    if(arg == 0){
        num_printed += putchar('0');
    } else {
        char buf[32] = {0};
        int i = 0;
        while(arg != 0){
            buf[i+1] = '0' + (arg & 7);
            arg >>= 3;
        }
        for (; i > 0; i--){
            num_printed += putchar(buf[i-1]);
        }
    }
    return num_printed;
}


int print_hex(unsigned int arg){
    int num_printed = 0;
    if(arg == 0){
        num_printed += putchar('0');
    } else {
        char buf[32];
        int i = 0;
        while(arg != 0){
            int digit = arg & 15;
            if(digit < 10){
                buf[i++] = '0' + digit;
            } else {
                buf[i++] = 'a' + digit - 10;
            }
            arg >>= 4;
        }
        for(; i > 0; i--){
            num_printed += putchar(buf[i-1]);
        }
    }
    return num_printed;
}


int print_unsigned_decimal(unsigned int arg){
    int num_printed = 0;
    if(arg == 0){
        num_printed += putchar('0');
    } else {
        char buf[32];
        int i = 0;
        while( arg != 0){
            buf[i++] = '0' + (arg % 10);
            arg /= 10;
        }
        for (; i > 0; i--){
            num_printed += putchar(buf[i - 1]);
        }
    }
    return num_printed;
}


int print_pointer(void* arg) {
    int num_printed = 0;
    num_printed += putchar('0');
    num_printed += putchar('x');
    num_printed += print_hex((uintptr_t)arg);
    return num_printed;
}


int print_string(char* str){
    int len = 0;
    while(*str != '\0'){
        putchar(*str);
        str++;
        len++;
    }
    return 0;
}

int my_printf(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);

    int byte_count = 0;
    
    while (*format != '\0') {
        switch(*format) {
            case '%': {
                ++format;
                switch(*format) {
                    case 'c': {
                        char c = va_arg(arglist, int);
                        putchar(c);
                        ++byte_count;
                        break;
                    }
                    case 'd': {
                    int i = va_arg(arglist, int);
                    if (i == 0) {
                        putchar('0');
                        ++byte_count;
                    }
                    if (i < 0) {
                        putchar('-');
                        ++byte_count;
                        i = -i;
                    }
                    int j = i, digits = 0;
                    while (j > 0) {
                        j /= 10;
                        ++digits;
                    }
                    char str[digits + 1];
                    str[digits] = '\0';
                    for (int k = digits - 1; k >= 0; --k) {
                        str[k] = i % 10 + '0';
                        i /= 10;
                    }
                    for (int k = 0; str[k] != '\0'; ++k) {
                        putchar(str[k]);
                        ++byte_count;
                    }
                    break;
                    }
                    
                    case 's': {
                        char* s = va_arg(arglist, char*);
                        if(s == NULL){
                            const char* null_str = "(null)";
                            while(*null_str != '\0'){
                                putchar(*null_str);
                                ++byte_count;
                                ++null_str;
                            }
                        } else{
                        while (*s != '\0') {
                            putchar(*s);
                            ++byte_count;
                            ++s;
                        }
                    }
                    break;
                }

                case 'o': {
                    unsigned int num = va_arg(arglist, unsigned int);
                    char octal_str[21]; // enough to hold the octal string of a 64-bit integer
                    int i = 0;
                    do {
                        octal_str[i++] = (num % 8) + '0';
                        num /= 8;
                    } while (num > 0);
                        while (i > 0) {
                        putchar(octal_str[--i]);
                        ++byte_count;
                    }
                    break;
                }

                case 'u': {
                    unsigned int num = va_arg(arglist, unsigned int);
                    int len = 0;
                    if (num == 0) {
                        len = 1;
                    }
                    else {
                        unsigned int temp = num;
                        while (temp != 0) {
                            len++;
                            temp /= 10;
                        }
                    }
                    char buffer[14] = {0};
                    int byte_count = 0;
                    for (int i = len - 1; i >= 0; i--) {
                        buffer[i] = num % 10 + '0';
                        num /= 10;
                        byte_count += sizeof(buffer[i]);
                    }
                    buffer[len] = '!'; 
                    buffer[len+1] = '\n'; 
                    byte_count += 2; 
                    for (int i = 0; i < len+2; i++) { 
                        putchar(buffer[i]);
                    }
                    return byte_count;
                    break;
                }

                case 'x': {
                    unsigned int num = va_arg(arglist, unsigned int);
                    char hex_str[9]; // enough to hold the hexadecimal string of a 32-bit integer
                    int i = 0;
                    do {
                        int digit = num % 16;
                        if (digit < 10) {
                            hex_str[i++] = digit + '0';
                        } else {
                            hex_str[i++] = digit - 10 + 'a';
                        }
                        num /= 16;
                    } while (num > 0);
                    while (i > 0) {
                        putchar(hex_str[--i]);
                        ++byte_count;
                    }
                    break;
                }



                default:
                    return -1;
            }
            break;
        }
            default: {
                putchar(*format);
                ++byte_count;
                break;
            }
        }
        ++format;
    }
    va_end(arglist);
    return byte_count;
}



int main(){

my_printf("Hello %s!\n", "QWASAR.IO" );
my_printf("%c%c%c%c%c\n", "H", "E", "L", "L", "O");
my_printf("%s\n", "Hello World!");
my_printf("%c!\n", "H");
}
