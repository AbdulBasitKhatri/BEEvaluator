#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

bool isChar(char &c)
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
    }
  }
  return r;
}

int main()
{
  string input = "";
  while(1)
  {
    cout << "\n>> ";
    getline(cin, input);
    if(input == "exit" || input == "quit")
    {
      break;
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