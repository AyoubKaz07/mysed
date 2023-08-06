# mysed
Writing my own version of the Unix command line tool sed (Super simple one)

### COMPILE
```gcc -g -o mysed mysed.c```
### OPTIONS
1- replace pattern e.g
```./mysed /pattern/replacement file.txt```
2- print lines double space
```./mysed -G file.txt```
3- strip trailing blank lines
```./mysed /^$/d file.txt```
4- output lines in range
```./mysed -n "m,np" file.txt``` from m to n range.
5- ouput lines with pattern
```./mysed -n pattern file.txt```
