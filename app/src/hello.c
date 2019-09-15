
void uart_init(void);
void uart_sendBytes(const char* string, unsigned length);
void uart_print(const char* const m);

void uart_print(const char* const m)
{
    unsigned size = 0;
    const char* p = m;
    while (*p) {
        size++;
        p++;
    }

    uart_sendBytes(m, size);
}

int main()
{
    uart_print("Hello, world!\n\r");

    return 0;
}
