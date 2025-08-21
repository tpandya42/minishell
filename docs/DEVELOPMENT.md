## Git

### Commit messages
- git add only the file that you have edited (add file.c)
- commit messages
    - feat -> feature
    - fix -> bug fix
    - docs -> documentation
    - style -> formatting, lint stuff
- commit message example:
    "fix(main.c): fixed x initialization in the main function"
- Git Graph for visualization

### Git Workflow Guide

📄 See [GIT_WORKFLOW_GUIDE.md](./GIT_WORKFLOW_GUIDE.md) for detailed steps on syncing branches.

## 🛠️ Features

### In process

- [ ] **Component**: XXX <X>
- [ ] 
- [ ] 
- [ ] 

### Done
- 
- 
-

## 🐞 Bugs / Issues

- [ ] XXX
- [ ] XXX
- [ ] XXX

## 📚 Test Cases

| Command                            | Expected Output                          |
|-----------------------------------|------------------------------------------|
| `echo "Hello $USER"`              | Output with `$USER` expanded              |
| `cd .. && pwd`                    | Directory changes and prints path         |
| `ls | grep minishell`             | Pipe functionality test                   |
| `cat << EOF`                      | Heredoc reads until delimiter `EOF`       |


## 💡 Ideas & Notes
- Improve xxx
- Move variable xxx
- Refactor xxx 
- Add xxx
- check xxx

## 🛠️ Backlog

```
| Phase                           | Key Milestones                                               |
| ------------------------------- | ------------------------------------------------------------ |
| 1. Research & Understanding     | - Complete reading project specs and documentation           |
|                                 | - Define project scope and assign roles                      |
| 2. Set-up                       | - Initialize repo and folder structure                       |
|                                 | - Working Makefile that compiles without errors              |
| 3. MVP Development              | - Basic lexer implemented                                    |
|                                 | - Basic parser building simple command structures            |
|                                 | - Simple execution of external commands without redirections |
| 4. Parallel Development         | - Minimal built-in commands implemented                      |
|                                 | - Redirections and pipes working                             |
|                                 | - Basic environment variable handling                        |
|                                 | - Signal handling implemented                                |
| 5. Integration & Error Handling | - Components merged without conflicts                        |
|                                 | - Basic error handling in place                              |
| 6. Testing & Norminette         | - Unit and functional tests written and passing              |
|                                 | - Norminette passed without errors                           |
|                                 | - Memory leaks checked and fixed                             |
| 7. Final Refactor & Cleanup     | - Code cleaned and optimized                                 |
|                                 | - Documentation updated                                      |
| 8. Evaluation Preparation       | - Final tests passed                                         |
|                                 | - README and DEVELOPMENT.md finalized                        |
|                                 | - Submission and defense preparation                         |

```
## Core Components Requirementes

### 1. Lexing (tokenization)
- Split raw input into tokens (words, operators, quoted strings).

- Preserve quoting (single and double quotes) for correct parsing later.

- Identify metacharacters (|, <, >, >>, <<, etc.).

- Handle special characters carefully (e.g., do not split inside quotes).

### 2. Parsing (syntax analysis)
- Build a structured command representation (AST or similar).

- Group tokens into commands, arguments, redirections, and pipelines.

- Recognize and properly nest pipes and redirections.

- Detect syntax errors early (e.g., unclosed quotes).

### 3. Execution
- Search and launch executables using PATH or absolute/relative paths.

- Handle command chaining through pipes (|).

- Manage redirections (<, >, >>, <<) as specified.

- Wait for child processes and return proper exit statuses.

### 4. Built-in commands
- Implement mandatory built-ins: echo [-n], cd, pwd, export, unset, env, exit.

- Built-ins run inside the shell process without fork unless required.

- Correctly handle built-in command options and edge cases.

### 5. Signal handling
- Handle Ctrl-C to interrupt current input and print new prompt.

- Handle Ctrl-D to exit shell.

- Ignore Ctrl-\ signals as Bash does.

- Use at most one global variable to indicate signal number.

### 6. Environment management
- Handle Ctrl-C to interrupt current input and print new prompt.

- Handle Ctrl-D to exit shell.

- Ignore Ctrl-\ signals as Bash does.

- Use at most one global variable to indicate signal number.

### 7. Redirections & Pipes
- Implement input (<) and output (>) redirections.

- Implement append output (>>) redirection.

- Support heredoc (<<) with delimiter input.

- Connect commands with pipes (|) passing output to input of next.

## Bash command examples
- what are pipes, how does heredoc work etc.
  
## Limitations
- things to work on in the future

## Resources
- links or books that helped you
