// using c++20
#include <iostream>
#include <cassert>
#include <string>
#include <cctype>

using namespace std;

template<typename T>
class StackADT {
public:
    virtual bool isEmpty() const = 0;

    virtual void push(const T& value) = 0;
    virtual T peek() const = 0;
    virtual bool pop() = 0;
};

constexpr int MIN_ARRAY_SIZE = 64;

template<typename T, int N>
class ArrayStack final : StackADT<T> {
private:
    int topIndex;
    int array[N]{};
public:
    ArrayStack() : topIndex(-1) {
        static_assert(N >= MIN_ARRAY_SIZE);
    }

    bool isEmpty() const override {
        return topIndex < 0;
    }

    void push(const T& value) override {
        if (topIndex == N) {
            throw std::length_error("Max array exceeded.");
        }
        // TODO move semantics
        topIndex += 1;
        array[topIndex] = value;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::logic_error("Peek on empty ArrayStack.");
        }

        return array[topIndex];
    }

    bool pop() override {
        if (isEmpty()) {
            return false;
        }
        topIndex -= 1;
        return true;
    }
};

void testArrayStack() {
    ArrayStack<int, MIN_ARRAY_SIZE> stack0;
    assert(stack0.isEmpty());
    stack0.push(10);
    assert(stack0.peek() == 10);
    stack0.push(20);
    assert(stack0.peek() == 20);
    assert(stack0.pop());
    assert(stack0.peek() == 10);
    assert(stack0.pop());
    assert(stack0.isEmpty());
}

template<typename T>
class Node {
private:
    T value;
    Node* next;

public:
    Node(T value) : value(value), next(nullptr) {}
    Node(T value, Node* next) : value(value), next(next) {}

    T getValue() const {
        return value;
    }

    Node* getNext() const {
        return next;
    }

    void setNext(Node* n) {
        next = n;
    }

    void setValue(const T& v) {
        value = v;
    }
};

template<typename T>
class ListStack : public StackADT<T> {
private:
    Node<T>* top;
public:
    ListStack() : top(nullptr) {}
    ~ListStack() {
        while (pop()) {}
    }

    // Copy constructor
    ListStack(const ListStack& other) {
        if (other.top == nullptr) {
            top = nullptr;
            return;
        }
        top = new Node<T>(other.top->getValue());
        Node<T>* lastCopy = top;
        Node<T>* nodeToCopy = other.top->getNext();
        while (nodeToCopy != nullptr) {
            Node<T>* newCopy = new Node<T>(nodeToCopy->getValue());
            nodeToCopy = nodeToCopy->getNext();
            lastCopy->setNext(newCopy);
            lastCopy = newCopy;
        }
        lastCopy->setNext(nullptr);
    }

    // Move constructor, Hint: Don't forget to make a "hollow" data structure.
    ListStack(ListStack&& other) noexcept {
        top = other.top;
        other.top = nullptr;
    }

    bool isEmpty() const override {
        return top == nullptr;
    }

    void push(const T& value) override {
        Node<T>* newNode;
        newNode = new Node<T>(value, top);
        top = newNode;
        newNode = nullptr;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::logic_error("Peek on empty ListStack.");
        }
        return top->getValue();
    }

    bool pop() override {
        if (isEmpty()) {
            return false;
        }
        
        Node<T>* oldTop = top;
        top = top->getNext();
        delete oldTop;
        oldTop = nullptr;
        return true;
    }
};

void testListStack() {
    ListStack<int> stack0;
    assert(stack0.isEmpty());
    stack0.push(10);
    assert(stack0.peek() == 10);
    stack0.push(20);
    assert(stack0.peek() == 20);
    assert(stack0.pop());
    assert(stack0.peek() == 10);
    assert(stack0.pop());
    assert(stack0.isEmpty());

    // Test copy constructor
    stack0.push(1);
    stack0.push(2);
    stack0.push(3);

    ListStack<int> stack1(stack0);
    assert(!stack0.isEmpty());
    assert(stack0.peek() == 3);
    assert(!stack1.isEmpty());
    assert(stack1.peek() == 3);

    // Test move constructor.
    ListStack<int> stack2(std::move(stack0));
    assert(stack0.isEmpty());
    assert(!stack2.isEmpty());
    assert(stack2.peek() == 3);
}

bool areCurleyBracesMatched(const string& inputString) {
    ArrayStack<char, MIN_ARRAY_SIZE> stack;
    for (int i = 0; i < inputString.length(); i++) {
        char thisChar = inputString[i];
        if (thisChar == '{') {
            stack.push(thisChar);
        }
        else if (thisChar == '}') {
            if (stack.isEmpty()) return false;
            else stack.pop();
        }
    }

    if (not stack.isEmpty()) return false;
    return true;
}

void testAreCurleyBracesMatched() {
    assert(areCurleyBracesMatched(""));
    assert(areCurleyBracesMatched("{}"));
    assert(areCurleyBracesMatched("a{bc}d"));
    assert(!areCurleyBracesMatched("{"));
    assert(!areCurleyBracesMatched("}"));
    assert(!areCurleyBracesMatched("a{b{c}"));
};

bool isPalindrome(const string& inputString) {
    int i = 0;
    int l = inputString.length();
    ArrayStack<char, MIN_ARRAY_SIZE> stack;
    while (i < l / 2) {
        stack.push(inputString[i]);
        i++;
    }
    if (l % 2 == 1) i++;
    while (i < l) {
        if (stack.peek() != inputString[i]) return false;
        stack.pop();
        i++;
    }
    return true;
}

void testIsPalindrome() {
    assert(isPalindrome(""));
    assert(isPalindrome("a"));
    assert(isPalindrome("aa"));
    assert(isPalindrome("aba"));
    assert(isPalindrome("abba"));
    assert(!isPalindrome("ab"));
    assert(!isPalindrome("abaa"));
}

string reversedString(const string& inputString) {
    ListStack<char> wordStack;
    for (int i = 0; i < inputString.length(); i++) {
        wordStack.push(inputString[i]);
    }
    string outputString;
    while (not wordStack.isEmpty()) {
        outputString += wordStack.peek();
        wordStack.pop();
    }
    return outputString;
}

void testReversedString() {
    assert(reversedString("").empty());
    assert(reversedString("a") == "a");
    assert(reversedString("ab") == "ba");
    assert(reversedString("abc") == "cba");
}

// Helper for infixToPostFix.
int precedence(char op) {
    if (op == '*' || op == '/') {
        return 2;
    }
    else {
        assert(op == '+' || op == '-');
        return 1;
    }
}

// Helper for infixToPostFix.
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// Helper for infixToPostFix.
bool isOperand(char ch) {
    return isalpha(ch);
}

string infixToPostFix(const string& infix) {
    ArrayStack<char, MIN_ARRAY_SIZE> opStack;
    string postfix;
    for (int i = 0; i < infix.length(); i++) {
        char thisChar = infix[i];
        if (isOperand(thisChar)) {
            postfix += thisChar;
        }
        else if (thisChar == '(') {
            opStack.push(thisChar);
        }
        else if (isOperator(thisChar)) {
            while (not opStack.isEmpty() and opStack.peek() != '(' and precedence(thisChar) <= precedence(opStack.peek())) {
                postfix += opStack.peek();
                opStack.pop();
            }
            opStack.push(thisChar);
        }
        else if (thisChar == ')') {
            while (opStack.peek() != '(') {
                postfix += opStack.peek();
                opStack.pop();
            }
            opStack.pop();
        }
    }

    while (not opStack.isEmpty()) {
        postfix += opStack.peek();
        opStack.pop();
    }
    return postfix;
}

void testInfixToPostFix() {
    assert(infixToPostFix("").empty());

    assert(infixToPostFix("a") == "a");
    assert(infixToPostFix("a+b") == "ab+");
    assert(infixToPostFix("a*b") == "ab*");

    assert(infixToPostFix("a+b*c") == "abc*+");
    assert(infixToPostFix("a+(b*c)") == "abc*+");
    assert(infixToPostFix("(a+(b*c))") == "abc*+");

    assert(infixToPostFix("(a+b)*c") == "ab+c*");
    assert(infixToPostFix("((a+b)*c)") == "ab+c*");

    assert(infixToPostFix("a*b+c") == "ab*c+");
    assert(infixToPostFix("(a*b)+c") == "ab*c+");
    assert(infixToPostFix("((a*b)+c)") == "ab*c+");
}

class QueensBoard {
private:
    int queens[8][2]; // spaces containing queens
    int topIndex = -1; // top of the "stack" (can't use an actual stack, but queens will be added in a stack-like fashion)
public:
    // returns true if given space has a queen
    bool hasQueen(int x, int y) {
        for (int i = 0; i <= topIndex; i++) {
            if (queens[i][0] == x and queens[i][1] == y) {
                return true;
            }
        }
        return false;
    }

    // adds a queen to the given space; throws an exception if there is a queen there
    void addQueen(int x, int y) {
        if (hasQueen(x, y)) throw logic_error("Adding queen on an occupied square");
        topIndex += 1;
        queens[topIndex][0] = x;
        queens[topIndex][1] = y;
        
    }

    // removes the queen at the given space; throws an exception if there is no queen there
    void removeQueen(int x, int y) {
        for (int i = 0; i <= topIndex; i++) {
            if (queens[i][0] == x and queens[i][1] == y) {
                queens[i][0] = queens[topIndex][0];
                queens[i][1] = queens[topIndex][1];
                topIndex -= 1;
                return;
            }
        }
        throw logic_error("Removing queen from an empty square");
    }

    // returns true if selected space is attacked by a queen
    bool isAttacked(int x, int y) {
        for (int i = 0; i <= topIndex; i++) {
            if (queens[i][0] == x || queens[i][1] == y || abs(queens[i][0] - x) == abs(queens[i][1] - y)) {
                return true;
            }
        }
        return false;
    }

    // returns list of all spaces containing queens in algebraic notation
    string listQueens() {
        string out;
        for (int i = 0; i <= topIndex; i++) {
            out += char(queens[i][0] + int('a'));
            out += to_string(queens[i][1] + 1);
            out += "\n";
        }
        return out;
    }

    // returns ascii representation of the full chessboard, with empty spaces as . and queens as Q
    string toAscii() {
        string out;
        for (int i = 0; i < 8; i++) {
            out += to_string(i + 1);
            for (int j = 0; j < 8; j++) {
                if (hasQueen(i, j)) {
                    out += 'Q';
                }
                else {
                    out += '.';
                }
            }
            out += '\n';
        }
        out += " ABCDEFGH";
        return out;
    }
};

void testQueensBoard() {
    QueensBoard testBoard;
    testBoard.addQueen(1, 5);
    testBoard.addQueen(5, 5);
    testBoard.addQueen(6, 2);
    testBoard.addQueen(2, 1);
    testBoard.removeQueen(6, 2);
    assert(testBoard.hasQueen(1, 5));
    assert(!testBoard.hasQueen(4, 3));
    assert(!testBoard.hasQueen(6, 2));
    assert(testBoard.isAttacked(2, 5));
    assert(!testBoard.isAttacked(6, 7));
    assert(testBoard.isAttacked(3, 0));
    assert(testBoard.listQueens() == "b6\nf6\nc2\n");
}

// returns list of eight queens in algebraic chess notation
string solveEightQueens() {
    QueensBoard board;
    ListStack<int> queensYStack;
    int currentX = 0;
    int currentY = 0;
    while (currentX < 8) {
        // backtrack if all 8 squares in the column have been checked
        if (currentY >= 8) {
            currentX -= 1;
            currentY = queensYStack.peek();
            board.removeQueen(currentX, currentY);
            queensYStack.pop();
        }
        // if currently checked square is not attacked, place a queen
        else if (!board.isAttacked(currentX, currentY)) {
            board.addQueen(currentX, currentY); 
            queensYStack.push(currentY);
            currentX += 1;
            currentY = -1;
        }
        currentY += 1; // making sure Y is advanced after either a backtrack or an attacked square
    }

    return board.toAscii();
}

int main() {
    testArrayStack();
    testListStack();
    testAreCurleyBracesMatched();
    testIsPalindrome();
    testReversedString();
    testInfixToPostFix();

    testQueensBoard();
    cout << solveEightQueens();
    return 0;
}
