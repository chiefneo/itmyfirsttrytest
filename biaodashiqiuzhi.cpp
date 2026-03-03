#include<iostream>
#include<vector>
#include<stack>
#include<string>
#include<cctype>
#include<cmath>
using namespace std;

int getPriority(char op){
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

pair<vector<string>, vector<string>> infixToPostfixPrefix(const string& infix){
    vector<string> postfix;
    stack<char> opStack;
    stack<vector<string>> prefixStack;
    int n= infix.size();
    int i=0;

    while(i<n){
        if(infix[i]=='#'){
            break;
        }else if(isdigit(infix[i])){
            int j=i;
            while(j<n && isdigit(infix[j])) j++;
            string num= infix.substr(i, j-i);
            postfix.push_back(num);
            prefixStack.push({num});
            i=j;
        }else if(infix[i]=='('){
            opStack.push(infix[i]);
            i++;
        }else if(infix[i]==')'){
            while(!opStack.empty()&&opStack.top()!='('){
                char op= opStack.top();
                opStack.pop();
                postfix.push_back(string(1, op));

                vector<string> right= prefixStack.top(); prefixStack.pop();
                vector<string> left= prefixStack.top(); prefixStack.pop();
                vector<string> newPrefix;
                newPrefix.push_back(string(1, op));
                newPrefix.insert(newPrefix.end(), left.begin(), left.end());
                newPrefix.insert(newPrefix.end(), right.begin(), right.end());
                prefixStack.push(newPrefix);
            }
            opStack.pop();
            i++;
        }else{// 处理运算符（+-*/）
            while(!opStack.empty()&&opStack.top()!='('&& getPriority(opStack.top())>=getPriority(infix[i])){
                char op= opStack.top();
                opStack.pop();

                postfix.push_back(string(1, op));

                vector<string> right= prefixStack.top(); prefixStack.pop();
                vector<string> left= prefixStack.top(); prefixStack.pop();
                vector<string> newPrefix;
                newPrefix.push_back(string(1, op));
                newPrefix.insert(newPrefix.end(), left.begin(), left.end());
                newPrefix.insert(newPrefix.end(), right.begin(), right.end());
                prefixStack.push(newPrefix);
            }
            opStack.push(infix[i]);
            i++;
        }
    }
    while(!opStack.empty()){
        char op= opStack.top();
        opStack.pop();

        postfix.push_back(string(1, op));

        vector<string> right= prefixStack.top(); prefixStack.pop();
        vector<string> left= prefixStack.top(); prefixStack.pop();
        vector<string> newPrefix;
        newPrefix.push_back(string(1, op));
        newPrefix.insert(newPrefix.end(), left.begin(), left.end());
        newPrefix.insert(newPrefix.end(), right.begin(), right.end());
        prefixStack.push(newPrefix);
    }

    return {postfix, prefixStack.top()};
}

int evaluatePostfix(const vector<string>& postfix) {
    stack<int> valStack;
    for (const string& token : postfix) {
        if (isdigit(token[0])) { // 操作数入栈
            valStack.push(stoi(token));
        } else { // 运算符：弹出两个操作数计算
            int right = valStack.top();
            valStack.pop();
            int left = valStack.top();
            valStack.pop();
            int res;
            switch (token[0]) {
                case '+': res = left + right; break;
                case '-': res = left - right; break;
                case '*': res = left * right; break;
                case '/': res = left / right; break; // 题目默认无除零错误
            }
            valStack.push(res);
        }
    }
    return valStack.top(); // 最终栈顶为结果
}

void printVector(const vector<string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (i > 0) cout << " ";
        cout << vec[i];
    }
    cout << endl;
}

int main() {
    string infix;
    getline(cin, infix); 

    // 中缀转后缀+前缀
    auto res = infixToPostfixPrefix(infix);
    vector<string> postfix = res.first;
    vector<string> prefix = res.second;

    // 后缀求值
    int result = evaluatePostfix(postfix);

    // 按要求输出
    cout << result << endl;
    printVector(prefix);
    printVector(postfix);

    return 0;
}