#include <string>
#include <cstdint>
#include <sstream>
#include <iostream>

template <class T>
struct Stack {
    int64_t stack_p{-1};
    int64_t r_factor{};
    int64_t size{};
    T* stack = nullptr;
};

template <class T>
void init(Stack<T>& st, int64_t size, int64_t r_fact) {
    st.r_factor = r_fact;
    st.size = size;
    st.stack = new T[size];
    st.stack_p = -1;
}

template <class T>
void resize(Stack<T>& st, int64_t new_size) {
    T* old = st.stack;
    int64_t old_size = st.size;
    st.stack = new T[new_size];
    st.size = new_size;
    int64_t n = old_size < new_size ? old_size : new_size;
    for (int64_t i = 0; i < n; ++i) st.stack[i] = old[i];
    if (st.stack_p >= new_size) st.stack_p = new_size - 1;
    delete[] old;
}

template <class T>
void clear(Stack<T>& st) { st.stack_p = -1; }

template <class T>
bool empty(Stack<T>& st) { return st.stack_p == -1; }

template <class T>
T top(Stack<T>& st) { return st.stack[st.stack_p]; }

template <class T>
void push(Stack<T>& st, T var) {
    if (st.stack_p + 1 >= st.size)
        resize(st, st.size + st.r_factor * st.size);
    st.stack[++st.stack_p] = var;
}

template <class T>
T pop(Stack<T>& st) { return st.stack[st.stack_p--]; }

int64_t expa(int64_t num, int64_t degree) {
    int64_t ans{1};
    for (int64_t i{}; i < degree; ++i) ans *= num;
    return ans;
}

int64_t Math(int64_t a, int64_t b, const char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return expa(a, b);
        default: return 0;
    }
}

int16_t VIP(char sign) {
    if (sign == '+' || sign == '-') return 1;
    if (sign == '*' || sign == '/') return 2;
    if (sign == '^') return 3;
    return 0;
}

int64_t RPN(std::string expres) {
    Stack<char> msigns;
    init(msigns, (int64_t)expres.size(), 2);
    Stack<int64_t> digits;
    init(digits, (int64_t)expres.size(), 2);

    std::stringstream ss(expres);
    std::string buff;
    while (ss >> buff) {
        bool is_number = isdigit((unsigned char)buff[0]) ||
                         (buff[0] == '-' && buff.size() > 1 &&
                          isdigit((unsigned char)buff[1]));
        if (is_number) {
            push(digits, (int64_t)std::stoll(buff));
        } else if (buff == "(") {
            push(msigns, '(');
        } else if (buff == ")") {
            while (!empty(msigns) && top(msigns) != '(') {
                int64_t val2 = pop(digits);
                int64_t val1 = pop(digits);
                char op = pop(msigns);
                push(digits, Math(val1, val2, op));
            }
            if (!empty(msigns)) pop(msigns);
        } else {
            char current_op = buff[0];
            // '^' правоассоциативен: выталкиваем только строго больший приоритет
            while (!empty(msigns) && top(msigns) != '(' &&
                   (VIP(top(msigns)) > VIP(current_op) ||
                    (VIP(top(msigns)) == VIP(current_op) && current_op != '^'))) {
                int64_t val2 = pop(digits);
                int64_t val1 = pop(digits);
                char op = pop(msigns);
                push(digits, Math(val1, val2, op));
            }
            push(msigns, current_op);
        }
    }

    while (!empty(msigns)) {
        int64_t val2 = pop(digits);
        int64_t val1 = pop(digits);
        char op = pop(msigns);
        push(digits, Math(val1, val2, op));
    }

    return pop(digits);
}

int main() {
    std::string expres = "20 * 2 + 3 * ( 4 - 2 ) ^ 2";
    std::cout << RPN(expres) << '\n';
    return 0;
}
