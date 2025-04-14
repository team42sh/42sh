# MINISHELL2 - A Simple UNIX Shell in C

## Description
Build a simple UNIX command interpreter (`mysh`) similar to **TCSH**. It handles basic commands, error handling, and supports a few built-in commands like `cd`, `setenv`, `unsetenv`, `env`, and `exit`.

## Installation

1. Clone the repo:
   - `git clone https://github.com/EpitechPGEPromo2029/B-PSU-200-MLH-2-1-minishell1-matteo.tutti.git`
   
2. Navigate to the project folder:
   - `cd B-PSU-200-MLH-2-1-minishell1-matteo.tutti`
   
3. Compile the project:
   - `make`

4. Run the shell:
   - `./mysh`

## Usage

- The prompt is displayed as a custom colored. Type your commands and hit Enter to execute.
  
  Example:
  - `<PROMPT> ls -l /usr /var`

## Built-ins

- [x] **cd**: Change directory
- [x] **setenv**: Set an environment variable
- [x] **unsetenv**: Unset an environment variable
- [x] **env**: Display environment variables
- [x] **exit**: Exit the shell
- [x] **alias**: Alias a string to another string / Show aliases / Show 1 alias
- [x] **unalias**: Remove an alias

## Error Handling

- [x] Command not found: Display an error message.
- [x] Exit status matches the executed command.
- [x] Show any signals if signaled.

## Misc

- [x] Usage of $? to see last exit status.
- [x] Usage of $PATH or $VARIABLE in the prompt.

## Line shell additonnal contents

- [x] Delimiter ';' to execute multiples commands.
- [x] Pipes and multiples pipes.
- [x] Right redirection (create and append).
- [x] Left redirection (read file).
- [ ] Left append (read user inputs).

---

## W.I.P. / Planned

- [ ] Arrows history commands.
- [ ] Usage of ${PATH} or ${VARIABLE} in the prompt.

## License

This project is licensed under the [MIT License](LICENSE).
