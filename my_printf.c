#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

typedef int(*handler_func_t)(va_list);

struct format_specifier{
    char spec;
    handler_func_t handler;
};

int handle_c(va_list arglist){
    int byte_count = 0;
    char c = va_arg(arglist, int);
    putchar(c);
    ++byte_count;
    return byte_count;
}

int handle_d(va_list arglist){
    int byte_count = 0;
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
    return byte_count;
}

int handle_s(va_list arglist){
    int byte_count = 0;
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
        return byte_count;
}

int handle_o(va_list arglist){
    int byte_count = 0;
    unsigned int num = va_arg(arglist, unsigned int);
    char octal_str[22]; // enough to hold the octal string of a 64-bit integer
    int i = 0;
    do {
        octal_str[i++] = (num % 8) + '0';
        num /= 8;
    } while (num > 0);
    while (i > 0) {
        putchar(octal_str[--i]);
        ++byte_count;
    }
    return byte_count;
}

int handle_u(va_list arglist){
    int byte_count = 0;
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
    char buffer[52] = {0};
    
    for (int i = len - 1; i >= 0; i--) {
        buffer[i] = num % 10 + '0';
        num /= 10;
        byte_count++;
    }
    buffer[len] = '\0';
    

    for (int i = 0; i < len; i++) {
        putchar(buffer[i]);
    }
    return byte_count;
}

int handle_x(va_list arglist){
    int byte_count = 0;
    unsigned int num = va_arg(arglist, unsigned int);
    char hex_str[9]; // enough to hold the hexadecimal string of a 32-bit integer
    int i = 0;
    do {
        int digit = num % 16;
        if (digit < 10) {
            hex_str[i++] = digit + '0';
        } else {
            hex_str[i++] = digit - 10 + 'A';
            //hex_str[i++] = digit - 10 + 'a';
        }
        num /= 16;
    } while (num > 0);
    while (i > 0) {
        putchar(hex_str[--i]);
        ++byte_count;
    }
    return byte_count;
}

int handle_p(va_list arglist) {
    uintptr_t ptr = va_arg(arglist, uintptr_t);
    char hex_str[17] = {0}; // enough to hold the hexadecimal string of a 64-bit pointer
    int i = 16;
    do {
        hex_str[--i] = "0123456789abcdef"[ptr & 0xF];
        ptr >>= 4;
    } while (ptr);
    // write the pointer address with the 0x prefix and without leading zeros
    char output[18] = "0x";
    int j = 2;
    while (i < 16) {
        output[j++] = hex_str[i++];
    }
    write(STDOUT_FILENO, output, j);
    return j;
}

static const struct format_specifier format_handlers[] = {
    {'c', handle_c},
    {'d', handle_d},
    {'s', handle_s},
    {'o', handle_o},
    {'u', handle_u},
    {'x', handle_x},
    {'p', handle_p},
};

int my_printf(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    int byte_count = 0;
    while (*format != '\0') {
        if (*format == '%') {
            ++format;
            for (size_t i = 0; i < sizeof(format_handlers) / sizeof(format_handlers[0]); ++i) {
                if (format_handlers[i].spec == *format) {
                    byte_count += format_handlers[i].handler(arglist);
                    break;
                }
            }
        } else {
            putchar(*format);
            ++byte_count;
        }
        ++format;
    }
    va_end(arglist);
    return byte_count;
    
}

int main(){
     my_printf("the byte count is %p\n", 25);
}