# MiniShell

MiniShell is a lightweight, custom Linux shell implementation written in C++. It provides basic shell functionalities, including executing commands, input/output redirection, and support for background processes.

---

## 📌 Features

* **Command Execution**: Supports running system commands directly from the shell.
* **Input/Output Redirection**: Allows the use of `>` and `<` for redirecting standard output and input.
* **Background Processing**: Execute processes in the background using `&`.
* **Signal Handling**: Manages signals for proper termination of background processes.

---

## 🚀 Getting Started

### Prerequisites

Ensure you have `g++` installed:

```bash
sudo apt-get update
sudo apt-get install g++
```

---

### Build the Project

To compile the MiniShell:

```bash
g++ -o minishell MiniShell.cc
```

This generates an executable named `minishell`.

---

### Run the Shell

To start the shell, simply execute:

```bash
./minishell
```

You should now be in the MiniShell environment.

---

## 💡 Usage Examples

* **Basic Command:**

  ```bash
  ls -al
  ```

* **Redirect Output:**

  ```bash
  ls -al > output.txt
  ```

* **Redirect Input:**

  ```bash
  wc -l < input.txt
  ```

* **Background Process:**

  ```bash
  ./long_running_task &
  ```

* **Exit the Shell:**

  ```bash
  exit
  ```

---

## 🤝 Contributing

Feel free to fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

---

## 📄 License

This project is licensed under the MIT License.
