# Boolean Expression Evaluator

This is a boolean expression evaluator implemented in C++ that supports various boolean algebraic operations such as AND, OR, NOT, XOR, and prints the truth table.

## Background

During my study of Digital Logic Design (DLD), I encountered challenges when dealing with complex and large boolean expressions. Manually evaluating these expressions and creating truth tables was not only time-consuming but also prone to errors. I realized the need for a tool that could handle this task efficiently and accurately.

Motivated by this need, I embarked on the journey of developing a boolean expression evaluator. My goal was to create a tool that could parse and evaluate complex boolean expressions quickly, making it easier to understand their logical behavior.

By developing this evaluator, I not only addressed the challenges I faced in my DLD studies but also created a valuable tool that can benefit others facing similar challenges. Now, evaluating complex boolean expressions and generating truth tables for them takes less than a second, eliminating the need for manual labor and reducing the likelihood of errors.

## Learning Experience

Throughout the development process, I encountered various challenges and learned valuable lessons. Some of the key learnings include:

- **Algorithm Design**: Designing algorithms to parse and evaluate boolean expressions efficiently required careful planning and consideration of edge cases.
- **Data Structures**: Choosing appropriate data structures, such as vectors and trees, was crucial for implementing the expression parser and evaluator.
- **Debugging Techniques**: Debugging complex code involving recursive algorithms and data manipulation taught me effective debugging strategies and the importance of thorough testing.
- **Optimization**: Optimizing code for performance and memory usage helped me improve the efficiency of the boolean expression evaluator.

## Features

- Supports boolean operators: `AND (*, . (dot) , ^), OR (+), NOT ('), XOR (|)`
- Parses boolean algebraic expressions
- Generates and prints the truth table for the given expression

## Usage

1. Clone the repository: `git clone https://github.com/AbdulBasitKhatri/BEEvaluator.git`
2. Compile the code: `cd BEEvaluator && g++ -o BEEvaluator main.cpp` (The provided binary is compiled with `g++ (Debian 13.2.0-13) 13.2.0`)
3. Run the program with a boolean expression as an argument: `./BEEvaluator`
4. It will ask for expression, enter any boolean algebraic expression.
5. Truth table will be generated.
6. To quit program just write `quit` or `exit` .

## Example

`A AND B OR C` should be written as `a . b + c` [no/extra whitespace will also work]

#### Result

`Truth Table: `

` | a: 0 | b: 0 | c: 0 | result: 0 | `

` | a: 0 | b: 0 | c: 1 | result: 1 | `

` | a: 0 | b: 1 | c: 0 | result: 0 | `

` | a: 0 | b: 1 | c: 1 | result: 1 | `

` | a: 1 | b: 0 | c: 0 | result: 0 | `

` | a: 1 | b: 0 | c: 1 | result: 1 | `

` | a: 1 | b: 1 | c: 0 | result: 1 | `

` | a: 1 | b: 1 | c: 1 | result: 1 | `

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
