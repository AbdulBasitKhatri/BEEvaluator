#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

bool isChar(const char &c)
{
  return (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

typedef enum TokenType
{
  LP,
  RP,
  AND,
  OR,
  NOT,
  XOR,
  VAR
} TokenType;

typedef struct Token
{
  char repr;
  int ind;
  TokenType type;
} Token;

vector<Token> lex(const string &input)
{
  int ind = 0;
  char c = 1;
  vector<Token> tokens;
  while(c != 0 && ind < input.length())
  {
    c = input[ind];
    if(c == '(')
    {
      if(ind - 1 >= 0 && ind - 1 < input.length() && (isChar(input[ind - 1]) || input[ind - 1] == '\''))
      {
        tokens.push_back(Token{c, ind - 1, AND});
      }
      tokens.push_back(Token{c, ind, LP});
    }
    else if(c == ')')
    {
      tokens.push_back(Token{c, ind, RP});
    }
    else if(c == '*' || c == '.' || c == '^')
    {
      tokens.push_back(Token{c, ind, AND});
    }
    else if(c == '+')
    {
      tokens.push_back(Token{c, ind, OR});
    }
    else if(c == '\'')
    {
      tokens.push_back(Token{c, ind, NOT});
    }
    else if(c == '|')
    {
      tokens.push_back(Token{c, ind, XOR});
    }
    else if(isChar(c))
    {
      if(ind - 1 >= 0 && ind - 1 < input.length() && (input[ind - 1] == ')' || isChar(input[ind - 1]) || input[ind - 1] == '\''))
      {
        tokens.push_back(Token{c, ind - 1, AND});
      }
      tokens.push_back(Token{c, ind, VAR});
    }
    else
    {
      if(c != ' ')
      {
      cerr << "\n\nError: Illegal Character `" << c << "` at (1," << ind + 1 << ").";
      exit(1);
      }
    }
    ind++;
  }
  return tokens;
}

void printTokens(vector<Token> &tokens)
{
  for(const Token token : tokens)
  {
    cout << "REPR: " << token.repr << "\n";
    cout << "INDEX: " << token.ind << "\n";
    cout << "TOKEN TYPE: " << token.type << "\n\n";
  }
}

typedef struct Node
{
  Node *left;
  Node *right;
  Token token;
} Node;

void printNode(Node* node)
{
  cout << "{Type: " << node->token.repr;
  if(node->left != nullptr)
  {
    cout << ", Left: ";
    printNode(node->left);
  }
  if(node->right != nullptr){
    cout << ", Right: ";
    printNode(node->right);
  }
  cout << "}";
}

void printNodes(vector<Node*> &nodes)
{
  for(Node* node : nodes)
  {
    cout << "{Type: " << node->token.repr;
    if(node->left != nullptr)
    {
      cout << ", Left: ";
      printNode(node->left);
    }
    if(node->right != nullptr){
      cout << ", Right:";
      printNode(node->right);
    }
    cout << "}\n";
  }
}

Node* parseTerm(vector<Token> tokens, int &index);
Node* parseFactor(vector<Token> tokens, int &index);

Node* parseExpression(vector<Token>& tokens, int &index)
{
  Node* left = parseTerm(tokens, index);
  while(index < tokens.size() && (tokens.at(index).type == OR || tokens.at(index).type == XOR))
  {
    Token token = tokens.at(index++);
    Node* right = parseTerm(tokens, index);
    left = new Node{left, right, token};
  }
  return left;
}

Node* parseTerm(vector<Token> tokens, int &index)
{
  Node* left = parseFactor(tokens, index);
  while(index < tokens.size() && tokens.at(index).type == AND)
  {
    Token token = tokens.at(index++);
    Node* right = parseFactor(tokens, index);
    left = new Node{left, right, token};
  }
  return left;
}

Node* parseFactor(vector<Token> tokens, int &index)
{
  if(index < tokens.size() && tokens.at(index).type == VAR)
  {
    Token token = tokens.at(index++);
    Node* var = new Node{nullptr, nullptr, token};
    while(index < tokens.size() && tokens.at(index).type == NOT)
    {
      token = tokens.at(index++);
      Node* notNode = new Node{var, nullptr, token};
      var = notNode;
    }
    return var;
  }
  else if(index < tokens.size() && tokens.at(index).type == LP)
  {
    Node* expression = parseExpression(tokens,++index);
    Token token = tokens.at(index++);
    if(token.type != RP)
    {
      cerr << "\n\nParsing Error: Expected `)` at line(1," << token.ind + 1 << ")." << endl;
      delete expression;
      exit(1);
      return nullptr;
    }
    while(index < tokens.size() && tokens.at(index).type == NOT)
    {
      token = tokens.at(index++);
      Node* notNode = new Node{expression, nullptr, token};
      expression = notNode;
    }
    return expression;
  }
  else
  {
    if(index < tokens.size())
    {
      cerr << "\n\nParsing Error: Invalid Character `" << tokens.at(index).repr << "` at line(1," << tokens.at(index).ind + 1 << ")." << endl;
      exit(1);
    }
    return nullptr;
  }
}

vector<Node*> parse(vector<Token>& tokens)
{
    vector<Node*> nodes;
    int index = 0;
    while(index < tokens.size())
    {
      Node* node = parseExpression(tokens, index);
      nodes.push_back(node);
    }
    return nodes;
}

void generateCombinations(int varIndex, int nVars, const vector<char>& varNames, vector<int>& values, unordered_map<char, int>& currentCombination, vector<unordered_map<char, int>>& truthTable) {
    if(varIndex == nVars)
    {
      truthTable.push_back(currentCombination);
      return;
    }
    for(int value : values)
    {
      currentCombination[varNames[varIndex]] = value;
      generateCombinations(varIndex + 1, nVars, varNames, values, currentCombination, truthTable);
    }
}

vector<unordered_map<char, int>> generateTruthTable(int nVars, const vector<char>& varNames)
{
  vector<unordered_map<char, int>> truthTable;
  vector<int> values = {0, 1};
  unordered_map<char, int> initialCombination;
  generateCombinations(0, nVars, varNames, values, initialCombination, truthTable);
  return truthTable;
}

int evaluateOR(Node* node, unordered_map<char, int> &values);
int evaluateAND(Node* node, unordered_map<char, int> &values);
int evaluateNOT(Node* node, unordered_map<char, int> &values);
int evaluateXOR(Node* node, unordered_map<char, int> &values);

int evaluateANY(Node *node, unordered_map<char, int> &values)
{
  if(node->token.type == VAR)
  {
    return values.at(node->token.repr);
  }
  else if(node->token.type == OR)
  {
    return evaluateOR(node, values);
  }
  else if(node->token.type == AND)
  {
    return evaluateAND(node, values);
  }
  else if(node->token.type == NOT)
  {
    return evaluateNOT(node, values);
  }
  else if(node->token.type == XOR)
  {
    return evaluateXOR(node, values);
  }
  else
  {
    return 0;
  }
}

int evaluateOR(Node* node, unordered_map<char, int> &values)
{
  int left = evaluateANY(node->left, values);
  int right = evaluateANY(node->right, values);
  return left || right;
}

int evaluateAND(Node* node, unordered_map<char, int> &values)
{
  int left = evaluateANY(node->left, values);
  int right = evaluateANY(node->right, values);
  return left && right;
}

int evaluateNOT(Node* node, unordered_map<char, int> &values)
{
  int left = evaluateANY(node->left, values);
  return !left;
}

int evaluateXOR(Node* node, unordered_map<char, int> &values)
{
  int left = evaluateANY(node->left, values);
  int right = evaluateANY(node->right, values);
  return left ^ right;
}

int evaluate(vector<Node*> nodes, unordered_map<char, int> &values)
{
  int r = 0;
  for(Node* node : nodes)
  {
    if(node->token.type == AND)
    {
      if(node->left && node->right)
      {
        r = evaluateAND(node, values);
      }
    }
    else if(node->token.type == OR)
    {
      if(node->left && node->right)
      {
        r = evaluateOR(node, values);
      }
    }
    else if(node->token.type == XOR)
    {
      if(node->left && node->right)
      {
        r = evaluateXOR(node, values);
      }
    }
    else if(node->token.type == NOT)
    {
      if(node->left)
      {
        r = evaluateNOT(node, values);
      }
    }
    else if(node->token.type == VAR)
    {
      r = evaluateANY(node, values);
    }
    else
    {
      cerr << "\n\nError: Invalid Expression Given!";
      exit(1);
    }
  }
  return r;
}

void printHelp()
{
  cout << "\n|~| HELP:\n\n";
  cout << "|~| For AND operations:\n";
  cout << "--- Use the *, . (dot), or ^ operators.\n";
  cout << "--- For example: 'a * b' or 'a.b' is equivalent to 'a AND b'.\n";
  cout << "--- Additionally, 'ab' is also equivalent to 'a * b'.\n\n";
  cout << "|~| For OR operations:\n";
  cout << "--- Use the + operator.\n";
  cout << "--- For example: 'a + b' is equivalent to 'a OR b'.\n\n";
  cout << "|~| For NOT operations:\n";
  cout << "--- Use the ' (single quote) operator.\n";
  cout << "--- For example: a' is equivalent to 'NOT a'.\n\n";
  cout << "|~| For XOR operations:\n";
  cout << "--- Use the | (pipe) operator.\n";
  cout << "--- For example: 'a | b' is equivalent to 'a XOR b'.\n\n";
  cout << "|~| Operator Precedence:\n";
  cout << "--- Parenthesis > NOT > AND > OR = XOR\n\n";
  cout << "|~| Type 'quit' or 'exit' to quit or exit.\n";
}

int main()
{
  cout << "\n|~| BEEvaluator\n";
  cout << "--- [ Tool for generating truth tables by evaluating boolean expressions ]\n";
  cout << "--- Official github repo: (https://github.com/AbdulBasitKhatri/BEEvaluator)\n\n";
  cout << "|~| Type 'help' to display help, or 'quit' or 'exit' to quit or exit.\n";

  string input = "";
  while(1)
  {
    cout << "\n>> ";
    getline(cin, input);
    if(input == "exit" || input == "quit")
    {
      break;
    }
    else if(input == "")
    {
      continue;
    }
    else if(input == "help")
    {
      printHelp();
      continue;
    }
    vector<char> varNames;
    int nVars;
    vector<Token> tokens = lex(input);
    for(Token token : tokens)
    {
      if(token.type == VAR)
      {
        vector<char>::const_iterator it = find(varNames.begin(), varNames.end(), token.repr);
        if (it == varNames.end())
        {
          varNames.push_back(token.repr);
        }
      }
    }
    sort(varNames.begin(),varNames.end());
    nVars = varNames.size();
    vector<unordered_map<char, int>> truthTable = generateTruthTable(nVars, varNames);
    vector<Node*> nodes = parse(tokens);
    cout << "\nTruth Table: " << endl;
    for(unordered_map<char, int> row : truthTable)
    {
      for(int i = 0; i < nVars; i++)
      {
        cout << " | " << varNames[i] << ": " << row.at(varNames[i]);
      }
      cout << " | result: " << evaluate(nodes,row) << " | " << endl;
    }
  }
  return 0;
}
