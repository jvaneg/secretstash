# secretstash
A security ctf puzzle I wrote for the final project of a security class I took in 2020.
* Exploit guide available [here](https://github.com/jvaneg/secretstash/blob/main/CPSC%20525%20Final%20-%20Secretstash%20Writeup%20-%20Joel%20van%20Egmond.pdf).

**NOTE:** This program was meant to be run on a specific server as a different user than the attacker. The attacker would have access to the source code, but not the keyfiles or secrets. They would interact with the program through a system binary.
Since it relies on certain memory properties of the server, it will probably not work when run anywhere else.
