# aturt13 Dojo
aturt13's dojo for pwn.college. The main purpose of this dojo is to extend Blue Belt material from pwn.college (e.g. *Blind Format String Exploits* could be thought of as an extension of *Format String Exploits*).

## Modules
The dojo currently contains 2 modules: *Blind Format String Exploits* and *Fuzzing*.

### Blind Format String Exploits
[*Blind Format String Exploits*](https://pwn.college/aturt13-dojo~4fac9429/blindfmt/) are an extension of [*Format String Exploits*](https://pwn.college/software-exploitation/format-string-exploits/). I was inspired primarily by a challenge from JustCTF, which was linked to an amazing [exploit](https://www.synacktiv.com/en/publications/exploiting-a-blind-format-string-vulnerability-in-modern-binaries-a-case-study-from) from Pwn2Own 2024. I decided to attempt it on my own prior to reading the writeup and made a module to share this interesting challenge with others.

*Blind Format String Exploits* can also be viewed as a demonstration of how an exploit can leverage copying using the `%*` format specifier to hijack control flow. Although the slides from *Format String Exploits* mention this capability, there is no challenge testing the student's understanding of it.

### Fuzzing
[*Fuzzing*](https://pwn.college/aturt13-dojo~4fac9429/fuzzing/) is a module focused primarily on teaching [AFL++](https://github.com/AFLplusplus/AFLplusplus) syntax in the pwn.college challenge-based style. Shortly after publishing the first challenges, the [*Fuzz*](https://pwn.college/fuzz~c7f7b8c2/) dojo emerged which may serve better for the purpose of learning fuzzing. Nevertheless, some challenges from the Fuzzing module remain interesting to in the exploitation sense.
