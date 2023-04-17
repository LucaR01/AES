# AES
A project about the Advanced Encryption Standard (AES) for an assignment at the university _Alma Mater Studiorum of Bologna_.

![GitHub repo size](https://img.shields.io/github/repo-size/LucaR01/AES?style=for-the-badge)
[![GitHub issues](https://img.shields.io/github/issues/LucaR01/AES?style=for-the-badge)](https://github.com/LucaR01/AES/issues)
[![Lines of code](https://img.shields.io/tokei/lines/github.com/LucaR01/AES?style=for-the-badge)](https://github.com/LucaR01/AES/tree/main/code)
[![GitHub last commit](https://img.shields.io/github/last-commit/LucaR01/AES?style=for-the-badge)](https://github.com/LucaR01/AES/graphs/commit-activity)

<hr>

The C++ implementation has both a gui and a console application:

By default, the console application will be launched, but if it has been launched with the `--gui` or `-g` argument, then the gui application will run.

## Arguments

There are multiple arguments that can be passed to the program, all of these are optional. Not providing them will not affect the result of the software.

The available arguments:
- `--help` or `-h`: Show help screen.
- `--version`: Show version.
- `aes <AES>`: Either AES 128, AES 192 or AES 256.
- `-o FILE` or `--output=FILE`: Output File [default: output.txt].
- `-i <file>`, `--input <file>`: Input File.
- `-m <message>`, `--message <message>`: Input message (plaintext or ciphertext).
- `--mode <mode>`: Input Mode ECB, CBC, CFB.
- `-p <padding>` or `--padding <padding>`: Input Padding. NO_PADDING, 1-0-Padding, ...
- `--iv=IV`: Input iv.
- `-k <key>` or `--key <key>`: Input key.

Only one between these two can be used:
- `--console` or `-c`: launch the console application. (Either this or `--gui` or `-g` can be used)
- `--gui` or `-g`: launch the gui application. (Either this or `--console` or `-c` can be used)

Only one of the three can be used for these:
- `--operation <operation>`: Input Operation. Encrypt, Decrypt.
- `--encryption`: Encryption.
- `--decryption`: Decryption.

Example:
  `--gui --aes="AES 256" --mode=CFB --message="secret message" -p "1-0-Padding" --encryption -i "input.txt" -o "output2.txt" --key="secret key"`

<hr>

## GUI Application

The GUI has a little issue, because the library I've used, **ImGUI** seems doesn't support the possibility of showing unsigned char, but only char which makes the GUI not working since the algorithm uses unsigned char.

<img src="https://github.com/LucaR01/AES/blob/main/screens/gui_encrypt_message.PNG" width="30%" heigth="30%" /> <img src="https://github.com/LucaR01/AES/blob/main/screens/gui_decrypt_message.PNG" width="30%" heigth="30%" />

It is possible to encrypt/decrypt files as well, but there are a few issues here as well, which will not make it always work properly.

<img src="https://github.com/LucaR01/AES/blob/main/screens/gui_encrypt_file.PNG" width="30%" heigth="30%" /> <img src="https://github.com/LucaR01/AES/blob/main/screens/gui_decrypt_file.PNG" width="30%" heigth="30%" />

<hr>

## Console Application

The console application will request the user various input unless they've been already inserted as command arguments. The console application faces similar issues with the GUI which makes it half working.

<img src="https://github.com/LucaR01/AES/blob/main/screens/console_operazione.PNG" width="20%" heigth="20%" />

<hr>

## Java

The Java implementation works just fine but doesn't have a console or a GUI application.

<hr>
