# Cracking a simple password in a simple program
A while ago I used [XTreeGold](https://pt.wikipedia.org/wiki/Xtree) ([XTreeGold](https://winworldpc.com/product/xtree/gold-3x)) to view the binary of a program that was on a CD-ROM I bought at a newsstand.

This program had a password and I would just like to know that password to pass this program's dialog/step.

Then I used XTreeGold to open the binary and see the ASCII characters that could be converted to me.

Well, it was a simple program but I didn't have any knowledge about ASCII, Hexadecimal, Programming or even Disassembly.
The internet didn't exist for me back then and the only thing I had was curiosity, love of computing, and I knew that everything was code, just code executed by a machine (think as low a level as you can).

Moving forward and looking at the ASCII code, I started to recognize some common words. I felt a good feeling and started looking for the word "password" or something similar. Then I found a human word next to the string "password". So I tested it and it was the correct password. For a boy who just discovered the world of computers, it was a very big joy.

So after several years, I'm here to share with you a similar thing. Here I want to encourage you to study (and also learn for yourself) some simple things that can be the starting path for a big trail.

What will we do?
1. write a simple C program that will ask for a password.
2. view the binary code and verify what is there.
3. change a few bits and see what happens.
4. go beyond.

PS: I'm assuming you're using GNU/Linux, ok?

I'm also assuming you have the following packages already installed on your system: binutils, bsdmainutils, hexedit, gcc.

### Writing a simple program to ask for a password ([it's here](password.c))
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkPassword(char s[20])
{
    char *password = "jeiks"; //our password is here
    if ( strcmp(password, s) == 0 )
        return 1;
    else
        return 0;
}

void startTheSystem()
{
    int n1, n2;
    printf("Welcome to the system!!\n"
           "Please write 2 integers: ");
    scanf("%d %d", &n1, &n2);
    printf("The multiplcation of %d by %d is %d\n", n1, n2, n1*n2);
    printf("That's all folks!\n");
}

void quitError()
{
    printf("Sorry! Unfortunately you do not know the password!!\nBye bye!\n");
    exit(1);
}

int main()
{
    char password[20];

    printf("Password: ");
    scanf("%s", password);

    if ( checkPassword(password) )
        startTheSystem();
    else
        quitError();
}

```
Save it to *password.c*. Then you can build and run the binary with the following commands:
```sh
# building it:
$ gcc -o password.e password.c
# running it:
$ ./password.e
```
### Seeing the binary with human eyes (lol)
Let's extract some information from the binary. The following command will give you similar information I had in XTreeGold:
```
$ hexdump -C password.e > hexdump.txt
```
Open the *strings.txt* file and search for the password "jeiks".
```
Note: you can use other ways to view it instead of saving the output to a text file and opening it in a text editor.
you can use the pipe "|" and a program to read it, such as less, or more, or vim.
Examples:
$ hexdump -C password.e | less
$ hexdump -C password.e | more
$ hexdump -C password.e | vim -
```

You can also extract strings using this command:
```sh
$ strings password.e > strings.txt
```
Open the *strings.txt* file and search for the password "jeiks".

***It's so easy, right?***

Well, let's change a few lines to make it better.
Copy **password.c** to **password2.c** and change the following line:
```c
char *password = "jeiks";
```
To these lines:
```c
    char *password;
    password = (char*) malloc(6);
    password[0] = 'j';
    password[1] = 'e';
    password[2] = 'i';
    password[3] = 'k';
    password[4] = 's';
    password[5] = '\0';
```
Now try using the **hexdump** and **strings** commands again and look for the password "jeiks":
```sh
$ gcc -o password2.e password2.c
$ strings
```
Well... there are many things a person can do to hide or encrypt a password within a program. But as I told you, here is just a simple program.

Moving on... there is other information we can get from the binary.
Try:
```sh
$ objdump -d password2.e > objdump.txt
```
Check the *objdump.txt* file. You will have something like:
```asm
    ...
;(memory address) <function name/label>:
0000000000001000 <_init>:
;(Memory address/position): (binary code in hexadecimal) (assembly code with the command, registers, memery address)
    1000:   f3 0f 1e fa             endbr64
    1004:   48 83 ec 08             sub    $0x8,%rsp
    1008:   48 8b 05 d9 2f 00 00    mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__>
    100f:   48 85 c0                test   %rax,%rax
    1012:   74 02                   je     1016 <_init+0x16>
    1014:   ff d0                   callq  *%rax
    1016:   48 83 c4 08             add    $0x8,%rsp
    101a:   c3                      retq
    ....
```
If you don't know about assembly, I recommend this [starter guide](http://www.pedrofreire.com/tutor/ac-guiao.pdf) (It's in Portuguese, but it's easy to translate in Google Translate *=D*).

Note: The binary code has different sizes because it was disassembled as a [CISC](https://pt.wikipedia.org/wiki/CISC) and not as a [RISC](https://en.wikipedia.org/wiki/reduced_instruction_set_computer).

In the *objdump.txt* file, look for some strings you used in *password.c*. How do you interpret this?

You can see that there are several functions that GCC will insert in order for your program to work well.
But the functions you created are easy to find, right?

Let's check this function:
```asm
0000000000001209 <checkPassword>:
    1209:   f3 0f 1e fa             endbr64
    120d:   55                      push   %rbp
    120e:   48 89 e5                mov    %rsp,%rbp
    1211:   48 83 ec 20             sub    $0x20,%rsp
    ...
    1283:   b8 00 00 00 00          mov    $0x0,%eax
    1288:   c9                      leaveq 
    1289:   c3                      retq
```

If you look, you'll have a line like the following inside this function:
```asm
    127a:   75 07                   jne    1283 <checkPassword+0x7a>
```
In this example it is:
1. the instruction address: *127a*
2. its hexadecimal code: 75 07 (binary 0111 0101 0000 0111)
3. the translation from binary to assembly: jne 1283

Let's interpret the instruction:
```
jne 1283 is:
  Jump short "to 1283" if (condition result is) Not Equal .
Note that there is a reference after 1283: <checkPassword+0x7a>
  If you add 0x7a to the the checkPassword's address (0x1209),
  you will get 0x1283 (that is some lines bellow) (cmd: python -c 'print(hex(0x1209+0x7a))'):
    1283:   b8 00 00 00 00          mov    $0x0,%eax
```
Now search for "jne binary code", or "jump not equal", or "jne assembly" on the Internet.
There are several websites explaining about it ([example](https://faydoc.tripod.com/cpu/jne.htm)).

Check the *JNE* number and try to find the *JE* number (*Jump Equal*).
The simple idea now is to change *JNE* to *JE*, i.e., change our IF condition to accept wrong passwords instead of the correct one.

### Changing a few bits and seeing what happens

Great... you saw that *JNE* is *75* and that *JE* is *74*.
So, let's use this information and *hexedit* to change our binary code:
```sh
$ hexedit password2.e
```
1. Then type *ENTER* and complete the *0x* with the instruction address (here in our example it is *127a*)
2. Press ENTER again and the cursor will be placed on the desired code: 75 07
3. Move the cursor over the 5 and press 4. You will see that the number was modified from 5 to 4.
4. Now press F10 to exit and press Y to save the binary.

Check the program again with *objdump* and see what has changed.

Now run it again and see how it is working.

Congratulations, the **password.e** binary is *cracked* and you can log into the system using any password (except the correct one.. lol).

### Going beyound
Did you see that the *strcmp* function was there and that it was provided by GLIBC (GNU C Library)?

Take a look:
```asm
00000000000010e0 <strcmp@plt>:
    10e0:   f3 0f 1e fa             endbr64
    10e4:   f2 ff 25 cd 2e 00 00    bnd jmpq *0x2ecd(%rip)        # 3fb8 <strcmp@GLIBC_2.2.5>
    10eb:   0f 1f 44 00 00
```

Because of this, we can create a similar function.
Then we can trick the system into using our similar function instead of the one used in the program.

First, let's create our library:
1. We have to write the same signature of this function. To find out, use the man pages:
```sh
man 3 strcmp
```
and we will have:
```c
int strcmp(const char *s1, const char *s2);
```
2. Time to write our own similar function (choose a name to save it in, as [*strcmp.c*](strcmp.c)):
```c
#include <stdio.h>
int strcmp(const char *s1, const char *s2) //same signature
{
    //but different code:
    printf("==> The program is trying to compare '%s' with '%s'\n", s1, s2);
    return 0; //return 0 means that *s1 is equal to *s2
}

```
Build our own library with *strcmp* function:
```sh
$ gcc -shared -o strcmp.so -fPIC strcmp.c
```
Now, you can run the program using LD_PRELOAD.
It will load our *strcmp* function before loading the system function, thus our function will be used instead of the GLIBC function.
```sh
LD_PRELOAD=./strcmp.so ./password2.e
```
Now, type something (like AAAA) or just press ENTER and you will see the hidden program password:
```
Password: AAAA
==> The program is trying to compare 'jeiks' with 'AAAA'
Sorry! Unfortunately you do not know the password!!
Bye bye!
```

Now feel free to run the program without LD_PRELOAD and use the password you found there!

Have fun and seya! :D
